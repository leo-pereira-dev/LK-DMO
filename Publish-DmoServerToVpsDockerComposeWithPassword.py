import argparse
import fnmatch
import os
import stat
import sys
import tarfile
import time
from pathlib import Path

import paramiko


EXCLUDE_NAMES = {
    ".git",
    ".vs",
    ".codex-tmp",
    "logs",
    "deploy-backups",
}


def info(message: str) -> None:
    print(message, flush=True)


def fail(message: str) -> None:
    print(message, file=sys.stderr, flush=True)
    sys.exit(1)


def should_exclude(path: Path, root: Path) -> bool:
    rel_parts = path.relative_to(root).parts
    rel_parts_lower = tuple(part.lower() for part in rel_parts)

    if "bins" in rel_parts_lower:
        return False

    if any(part in EXCLUDE_NAMES for part in rel_parts):
        return True

    if any(part.lower() in {"bin", "obj"} for part in rel_parts):
        return True

    name = path.name
    patterns = [
        "*.user",
        "*.suo",
        "*.pdb",
        "*.nupkg",
    ]
    return any(fnmatch.fnmatch(name, pattern) for pattern in patterns)


def create_archive(local_source: Path, archive_path: Path) -> None:
    if archive_path.exists():
        archive_path.unlink()

    info(f"Creating archive: {archive_path}")
    with tarfile.open(archive_path, "w:gz", compresslevel=3) as tar:
        for path in local_source.rglob("*"):
            if should_exclude(path, local_source):
                continue
            arcname = path.relative_to(local_source).as_posix()
            tar.add(path, arcname=arcname, recursive=False)

    size_mb = archive_path.stat().st_size / (1024 * 1024)
    info(f"Archive size: {size_mb:.1f} MB")


def find_latest_archive(tmp_root: Path) -> Path | None:
    archives = sorted(
        tmp_root.glob("dmo-server-main-postgres-*.tar.gz"),
        key=lambda p: p.stat().st_mtime,
        reverse=True,
    )
    return archives[0] if archives else None


def read_password(password_file: Path) -> str:
    if not password_file.exists():
        fail(f"Password file not found: {password_file}")
    password = password_file.read_text(encoding="utf-8-sig").strip().lstrip("\ufeff")
    if not password or password == "COLE_A_SENHA_AQUI":
        fail(f"Password file is empty or still has the placeholder: {password_file}")
    return password


def connect(host: str, user: str, password: str) -> paramiko.SSHClient:
    info(f"Connecting to {user}@{host}...")
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    try:
        client.connect(
            hostname=host,
            username=user,
            password=password,
            timeout=25,
            banner_timeout=25,
            auth_timeout=25,
            look_for_keys=False,
            allow_agent=False,
        )
    except paramiko.ssh_exception.AuthenticationException:
        client.close()
        sock = paramiko.Transport((host, 22))
        sock.start_client(timeout=25)

        def interactive_handler(title, instructions, prompts):
            return [password for _prompt, _echo in prompts]

        sock.auth_interactive(user, interactive_handler)
        if not sock.is_authenticated():
            sock.close()
            raise
        client = paramiko.SSHClient()
        client._transport = sock
    return client


def upload_file(sftp: paramiko.SFTPClient, local: Path, remote: str) -> None:
    size = local.stat().st_size
    sent_at = {"last": 0.0}

    def progress(sent: int, total: int) -> None:
        now = time.time()
        if now - sent_at["last"] >= 5 or sent == total:
            sent_at["last"] = now
            pct = (sent / total * 100) if total else 100
            info(f"Uploading {local.name}: {pct:.1f}% ({sent}/{total} bytes)")

    info(f"Uploading {local.name} to {remote}")
    sftp.put(str(local), remote, callback=progress if size else None)


def run_remote(client: paramiko.SSHClient, command: str, password: str) -> int:
    info("Running remote deploy...")
    transport = client.get_transport()
    if transport is None:
        fail("SSH transport is not available.")

    channel = transport.open_session()
    channel.get_pty()
    channel.exec_command(command)
    sudo_prompt = "[sudo-password-prompt]"

    while True:
        while channel.recv_ready():
            data = channel.recv(4096)
            if sudo_prompt.encode() in data:
                data = data.replace(sudo_prompt.encode(), b"")
                channel.send(password + "\n")
            sys.stdout.buffer.write(data)
            sys.stdout.buffer.flush()
        while channel.recv_stderr_ready():
            data = channel.recv_stderr(4096)
            if sudo_prompt.encode() in data:
                data = data.replace(sudo_prompt.encode(), b"")
                channel.send(password + "\n")
            sys.stderr.buffer.write(data)
            sys.stderr.buffer.flush()
        if channel.exit_status_ready():
            break
        time.sleep(0.2)

    while channel.recv_ready():
        data = channel.recv(4096)
        if sudo_prompt.encode() in data:
            data = data.replace(sudo_prompt.encode(), b"")
        sys.stdout.buffer.write(data)
    while channel.recv_stderr_ready():
        data = channel.recv_stderr(4096)
        if sudo_prompt.encode() in data:
            data = data.replace(sudo_prompt.encode(), b"")
        sys.stderr.buffer.write(data)
    sys.stdout.buffer.flush()
    sys.stderr.buffer.flush()
    return channel.recv_exit_status()


def build_remote_script() -> str:
    return """#!/usr/bin/env bash
set -euo pipefail

REMOTE_ROOT="$1"
ARCHIVE="$2"
ENV_FILE="$3"
BUILD_MODE="$4"
shift 4
SERVICES=("$@")

STAMP="$(date +%Y%m%d-%H%M%S)"
REMOTE_PARENT="$(dirname "$REMOTE_ROOT")"
TMP_ROOT="${REMOTE_ROOT}.deploy-${STAMP}"
BACKUP_ROOT="${REMOTE_PARENT}/deploy-backups/dmo-server-main-postgres-${STAMP}"

if [ ! -f "$ARCHIVE" ]; then
  echo "Archive not found: $ARCHIVE" >&2
  exit 1
fi

rm -rf "$TMP_ROOT"
mkdir -p "$TMP_ROOT"
tar -xzf "$ARCHIVE" -C "$TMP_ROOT"

if [ -f "${REMOTE_ROOT}/${ENV_FILE}" ] && [ ! -f "${TMP_ROOT}/${ENV_FILE}" ]; then
  cp "${REMOTE_ROOT}/${ENV_FILE}" "${TMP_ROOT}/${ENV_FILE}"
fi

if [ ! -f "${TMP_ROOT}/${ENV_FILE}" ]; then
  echo "Required compose env file not found after staging: ${TMP_ROOT}/${ENV_FILE}" >&2
  exit 1
fi

mkdir -p "${REMOTE_PARENT}/deploy-backups"

if [ -d "$REMOTE_ROOT" ]; then
  mv "$REMOTE_ROOT" "$BACKUP_ROOT"
fi

mv "$TMP_ROOT" "$REMOTE_ROOT"
cd "$REMOTE_ROOT"

if [ "$BUILD_MODE" = "build" ]; then
  COMPOSE_ARGS=(up -d --build --no-deps)
else
  COMPOSE_ARGS=(up -d --no-deps)
fi

sudo -S -p '[sudo-password-prompt]' docker compose --env-file "$ENV_FILE" "${COMPOSE_ARGS[@]}" "${SERVICES[@]}"
sudo -S -p '[sudo-password-prompt]' docker compose --env-file "$ENV_FILE" ps "${SERVICES[@]}"

rm -f "$ARCHIVE" "$0"
echo "Backup kept at: $BACKUP_ROOT"
"""


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--host", required=True)
    parser.add_argument("--user", required=True)
    parser.add_argument("--password-file", required=True)
    parser.add_argument("--local-source", required=True)
    parser.add_argument("--remote-root", required=True)
    parser.add_argument("--compose-env", default=".env.production")
    parser.add_argument("--service", action="append", default=[])
    parser.add_argument("--no-build", action="store_true")
    parser.add_argument("--reuse-latest-archive", action="store_true")
    parser.add_argument("--check-connection", action="store_true")
    args = parser.parse_args()

    workspace = Path(__file__).resolve().parent
    local_source = Path(args.local_source).resolve()
    password_file = Path(args.password_file).resolve()
    tmp_root = workspace / ".codex-tmp"
    tmp_root.mkdir(exist_ok=True)

    if not str(local_source).lower().startswith(str(workspace).lower()):
        fail(f"Refusing to publish a source outside this workspace: {local_source}")

    compose_file = local_source / "docker-compose.yml"
    if not compose_file.exists():
        fail(f"docker-compose.yml not found: {compose_file}")

    services = args.service or ["account", "character", "game"]
    if "postgres" in services:
        fail("Refusing to deploy postgres from this helper. Use app services only.")

    password = read_password(password_file)
    if args.check_connection:
        client = connect(args.host, args.user, password)
        client.close()
        info("Connection OK.")
        return

    if args.reuse_latest_archive:
        archive_path = find_latest_archive(tmp_root)
        if archive_path is None:
            fail("No reusable archive found in .codex-tmp.")
        info(f"Reusing archive: {archive_path}")
    else:
        stamp = time.strftime("%Y%m%d-%H%M%S")
        archive_path = tmp_root / f"dmo-server-main-postgres-{stamp}.tar.gz"
        create_archive(local_source, archive_path)

    remote_archive = f"/tmp/{archive_path.name}"
    remote_script = f"/tmp/publish-dmo-server-{int(time.time())}.sh"

    client = connect(args.host, args.user, password)
    try:
        sftp = client.open_sftp()
        try:
            script_bytes = build_remote_script().encode("utf-8")
            with sftp.file(remote_script, "wb") as remote:
                remote.write(script_bytes)
            sftp.chmod(remote_script, stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR)
            upload_file(sftp, archive_path, remote_archive)
        finally:
            sftp.close()

        build_mode = "nobuild" if args.no_build else "build"
        quoted_services = " ".join("'" + service.replace("'", "'\\''") + "'" for service in services)
        command = (
            f"bash '{remote_script}' "
            f"'{args.remote_root}' '{remote_archive}' '{args.compose_env}' '{build_mode}' "
            f"{quoted_services}"
        )
        exit_code = run_remote(client, command, password)
        if exit_code != 0:
            fail(f"Remote deploy failed with exit code {exit_code}.")
    finally:
        client.close()

    info("Publish complete.")


if __name__ == "__main__":
    main()
