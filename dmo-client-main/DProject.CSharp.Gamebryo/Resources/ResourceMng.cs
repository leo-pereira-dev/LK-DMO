namespace DProject.Resources
{
    public static class ResourceHash
    {
        public static ulong GetHashCode(string value)
        {
            const ulong offset = 14695981039346656037UL;
            const ulong prime = 1099511628211UL;

            ulong hash = offset;
            foreach (char c in value.AsSpan().Trim())
            {
                hash ^= char.ToUpperInvariant(c);
                hash *= prime;
            }

            return hash;
        }
    }
}

namespace GUI
{
    using DProject.Gamebryo;

    public sealed class Texture
    {
        private int _refCount = 1;

        public Texture(string fileName)
        {
            m_strName = fileName;
            m_HashCode = DProject.Resources.ResourceHash.GetHashCode(fileName);
            var prefs = new NiTexture.FormatPrefs
            {
                m_ePixelLayout = NiTexture.PixelLayout.COMPRESSED,
                m_eMipMapped = NiTexture.MipMapFlag.NO
            };
            m_NiTexture = NiSourceTexture.Create(fileName, prefs);
        }

        public ulong m_HashCode { get; private set; }
        public string m_strName { get; private set; }
        public NiTexture? m_NiTexture { get; private set; }

        public NiTexture? GetTexture() => m_NiTexture;
        public uint GetRefCountEx() => (uint)Math.Max(0, _refCount);
        public ulong GetHashCodeEx() => m_HashCode;
        public string GetFileName() => m_strName;
        public void AddRef() => _refCount++;

        public void ReleaseRef()
        {
            if (_refCount > 0)
            {
                _refCount--;
            }
        }
    }
}

namespace DProject.Resources
{
    using GUI;

    public sealed class TextureMng
    {
        private readonly Dictionary<ulong, Texture> _listTexture = new();

        public IReadOnlyDictionary<ulong, Texture> Textures => _listTexture;

        public void Init()
        {
        }

        public void End()
        {
            RemoveAllResource();
        }

        public Texture GetResource(string strFilename)
        {
            ulong hashCode = ResourceHash.GetHashCode(strFilename);
            if (_listTexture.TryGetValue(hashCode, out Texture? texture))
            {
                texture.AddRef();
                return texture;
            }

            return CreateResource(strFilename);
        }

        public void RemoveResource(Texture? hashCode)
        {
            if (hashCode is null)
            {
                return;
            }

            ulong key = hashCode.GetHashCodeEx();
            if (!_listTexture.TryGetValue(key, out Texture? texture))
            {
                return;
            }

            texture.ReleaseRef();
            if (texture.GetRefCountEx() <= 1)
            {
                _listTexture.Remove(key);
            }
        }

        public void CleanUpTexture()
        {
            foreach (ulong key in _listTexture
                .Where(static pair => pair.Value.GetRefCountEx() <= 1)
                .Select(static pair => pair.Key)
                .ToArray())
            {
                _listTexture.Remove(key);
            }
        }

        public void RemoveAllResource()
        {
            _listTexture.Clear();
        }

        public int GetSize() => _listTexture.Count;

        private Texture CreateResource(string strFilename)
        {
            var texture = new Texture(strFilename);
            _listTexture[texture.GetHashCodeEx()] = texture;
            return texture;
        }
    }

    public sealed class ResourceMng
    {
        private readonly TextureMng _textureMng = new();

        public TextureMng TextureMng => _textureMng;

        public void Init()
        {
            _textureMng.Init();
        }

        public void End()
        {
            RemoveAllResource();
        }

        public void CleanUpResource()
        {
            _textureMng.CleanUpTexture();
        }

        public int GetTextureResourceSize() => _textureMng.GetSize();
        public Texture GetTexture(string strFilename) => _textureMng.GetResource(strFilename);
        public Texture GetTexture(ReadOnlySpan<char> strFilename) => GetTexture(strFilename.ToString());
        public void RemoveTexture(Texture? pRemove) => _textureMng.RemoveResource(pRemove);
        public void RemoveAllResource() => _textureMng.RemoveAllResource();
    }
}
