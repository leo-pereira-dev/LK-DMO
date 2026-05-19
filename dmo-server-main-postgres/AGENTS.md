# AGENTS.md - DMO Server Work Guide

## Bug Investigation Rules

For every DMO bug, explicitly use this checklist before proposing or implementing a fix:

- Packet mismatches
- Race conditions
- Null checks
- Game logic
- Pipelines
- Overflow

When the issue involves client/server contracts, packets, loading, map transitions, UI rendering, movement, mobs, or crashes, the unpacked/decompiled client dump at `F:\Juegos\DMO\unpacked_exe_all` is a required source of truth. Compare the dump's client behavior against server packet processors, server packet writers, client source, database/bin assets, and live logs before deciding root cause.

Do not treat the dump as optional context. For packet and lifecycle bugs, start by finding the relevant pseudocode/functions in the dump and mapping them to the server flow.

## Process Handling

If build, copy, deploy, or validation is blocked because DMO servers/processes are running and locking files, stop the blocking server processes and continue instead of asking the user to do it.
