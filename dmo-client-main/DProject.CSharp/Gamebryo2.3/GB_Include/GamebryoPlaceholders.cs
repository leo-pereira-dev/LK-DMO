namespace DProject.Gamebryo;

public static class NiRuntime
{
    public static void NiInit()
    {
    }

    public static void NiShutdown()
    {
    }
}

public readonly record struct NiPoint3(float X, float Y, float Z)
{
    public static readonly NiPoint3 Zero = new(0.0f, 0.0f, 0.0f);

    public static NiPoint3 operator +(NiPoint3 left, NiPoint3 right) =>
        new(left.X + right.X, left.Y + right.Y, left.Z + right.Z);

    public static NiPoint3 operator -(NiPoint3 left, NiPoint3 right) =>
        new(left.X - right.X, left.Y - right.Y, left.Z - right.Z);

    public static NiPoint3 operator *(NiPoint3 point, float scale) =>
        new(point.X * scale, point.Y * scale, point.Z * scale);

    public float Length() => MathF.Sqrt((X * X) + (Y * Y) + (Z * Z));

    public NiPoint3 Unitize()
    {
        float length = Length();
        return length <= 0.0001f ? Zero : new NiPoint3(X / length, Y / length, Z / length);
    }

    public float Dot(NiPoint3 other) => (X * other.X) + (Y * other.Y) + (Z * other.Z);

    public NiPoint3 WithZ(float z) => new(X, Y, z);
}

public sealed record NiColor(float R, float G, float B)
{
    public static readonly NiColor White = new(1.0f, 1.0f, 1.0f);
}

public sealed record NiColorA(float R, float G, float B, float A)
{
    public static readonly NiColorA White = new(1.0f, 1.0f, 1.0f, 1.0f);
}

public class NiRefObject
{
    private int _refCount = 1;

    public uint GetRefCount() => (uint)Math.Max(0, _refCount);
    public void IncRefCount() => _refCount++;
    public void DecRefCount()
    {
        if (_refCount > 0)
        {
            _refCount--;
        }
    }
}

public class NiObject : NiRefObject
{
    public string Name { get; private set; } = string.Empty;

    public virtual NiObject Clone()
    {
        return (NiObject)MemberwiseClone();
    }

    public virtual void SetName(string name)
    {
        Name = name;
    }
}

public class NiAVObject : NiObject
{
    public NiPoint3 Translate { get; private set; } = NiPoint3.Zero;
    public NiPoint3 Scale { get; private set; } = new(1.0f, 1.0f, 1.0f);
    public bool AppCulled { get; private set; }

    public virtual void SetTranslate(NiPoint3 translate)
    {
        Translate = translate;
    }

    public virtual NiPoint3 GetTranslate() => Translate;

    public virtual void SetScale(float scale)
    {
        Scale = new NiPoint3(scale, scale, scale);
    }

    public virtual void SetAppCulled(bool culled)
    {
        AppCulled = culled;
    }

    public virtual void Update(float time)
    {
    }
}

public class NiNode : NiAVObject
{
    private readonly List<NiAVObject> _children = [];

    public IReadOnlyList<NiAVObject> Children => _children;

    public void AttachChild(NiAVObject child)
    {
        ArgumentNullException.ThrowIfNull(child);
        _children.Add(child);
    }

    public bool DetachChild(NiAVObject child) => _children.Remove(child);

    public NiAVObject? GetAt(int index)
    {
        return index >= 0 && index < _children.Count ? _children[index] : null;
    }

    public override void Update(float time)
    {
        base.Update(time);
        foreach (NiAVObject child in _children)
        {
            child.Update(time);
        }
    }
}

public class NiCamera : NiAVObject
{
    public float FieldOfView { get; private set; } = 60.0f;
    public float NearPlane { get; private set; } = 1.0f;
    public float FarPlane { get; private set; } = 10000.0f;

    public void SetViewFrustum(float fieldOfView, float nearPlane, float farPlane)
    {
        FieldOfView = fieldOfView;
        NearPlane = nearPlane;
        FarPlane = farPlane;
    }
}

public class NiLight : NiAVObject
{
    public NiColor DiffuseColor { get; private set; } = NiColor.White;
    public float Dimmer { get; private set; } = 1.0f;

    public void SetDiffuseColor(NiColor color)
    {
        DiffuseColor = color;
    }

    public void SetDimmer(float dimmer)
    {
        Dimmer = dimmer;
    }
}

public sealed class NiRenderer : IDisposable
{
    public bool IsDeviceLost { get; private set; }
    public bool IsFrameActive { get; private set; }

    public string GetLastErrorString() => string.Empty;

    public bool Create() => true;

    public bool BeginFrame()
    {
        if (IsDeviceLost)
        {
            return false;
        }

        IsFrameActive = true;
        return true;
    }

    public void Render()
    {
    }

    public void EndFrame()
    {
        IsFrameActive = false;
    }

    public void DisplayFrame()
    {
    }

    public void MarkDeviceLost()
    {
        IsDeviceLost = true;
    }

    public bool ResetDevice()
    {
        IsDeviceLost = false;
        return true;
    }

    public void Dispose()
    {
    }
}

public class NiFile
{
    public enum OpenMode
    {
        READ_ONLY,
        WRITE_ONLY,
        APPEND_ONLY
    }

    public NiFile(string path, OpenMode mode, uint bufferSize = 0)
    {
        Path = path;
        Mode = mode;
        BufferSize = bufferSize;
    }

    public string Path { get; }
    public OpenMode Mode { get; }
    public uint BufferSize { get; }
}

public class NiTexture : NiObject
{
    public enum PixelLayout
    {
        PALETTIZED_8,
        HIGH_COLOR_16,
        TRUE_COLOR_32,
        COMPRESSED,
        BUMPMAP,
        DEFAULT
    }

    public enum MipMapFlag
    {
        NO,
        YES,
        DEFAULT
    }

    public sealed class FormatPrefs
    {
        public PixelLayout m_ePixelLayout { get; set; } = PixelLayout.DEFAULT;
        public MipMapFlag m_eMipMapped { get; set; } = MipMapFlag.DEFAULT;
    }

    public static bool MipmapByDefault { get; private set; }
    public string Path { get; protected set; } = string.Empty;
    public FormatPrefs Preferences { get; protected set; } = new();

    public static void SetMipmapByDefault(bool value)
    {
        MipmapByDefault = value;
    }
}

public class NiSourceTexture : NiTexture
{
    public static bool UseMipmapping { get; private set; }
    public static bool UsePreloading { get; private set; }
    public static bool DestroyAppDataFlag { get; private set; }

    public static void SetUseMipmapping(bool value) => UseMipmapping = value;
    public static void SetUsePreloading(bool value) => UsePreloading = value;
    public static void SetDestroyAppDataFlag(bool value) => DestroyAppDataFlag = value;

    public static NiSourceTexture Create(string path, FormatPrefs? prefs = null)
    {
        return new NiSourceTexture
        {
            Path = path,
            Preferences = prefs ?? new FormatPrefs()
        };
    }

    public void DestroyAppPixelData()
    {
    }
}

public sealed class NiTexturingProperty : NiObject
{
    private readonly Dictionary<int, NiTexture> _textures = new();

    public IReadOnlyDictionary<int, NiTexture> Textures => _textures;

    public void SetBaseTexture(NiTexture texture)
    {
        _textures[0] = texture;
    }

    public NiTexture? GetBaseTexture()
    {
        return _textures.TryGetValue(0, out NiTexture? texture) ? texture : null;
    }
}

public static class NiMaterial
{
    public static string DefaultWorkingDirectory { get; private set; } = string.Empty;

    public static void SetDefaultWorkingDirectory(string path)
    {
        DefaultWorkingDirectory = path;
    }
}

public sealed class NiShader : NiObject
{
    public string SourcePath { get; private set; } = string.Empty;
    public bool IsCompiled { get; private set; }

    public bool Compile(string sourcePath)
    {
        SourcePath = sourcePath;
        IsCompiled = true;
        return true;
    }
}

public sealed class NiMaterialProperty : NiObject
{
    public NiColorA Diffuse { get; private set; } = NiColorA.White;
    public NiColorA Ambient { get; private set; } = NiColorA.White;
    public NiShader? Shader { get; private set; }

    public void SetDiffuseColor(NiColorA color)
    {
        Diffuse = color;
    }

    public void SetAmbientColor(NiColorA color)
    {
        Ambient = color;
    }

    public void SetShader(NiShader shader)
    {
        Shader = shader;
    }
}

public sealed class NiShaderMaterialPipeline
{
    private readonly Dictionary<string, NiShader> _shaders = new(StringComparer.OrdinalIgnoreCase);

    public IReadOnlyDictionary<string, NiShader> Shaders => _shaders;

    public NiShader GetOrCreateShader(string sourcePath)
    {
        if (_shaders.TryGetValue(sourcePath, out NiShader? shader))
        {
            return shader;
        }

        shader = new NiShader();
        shader.Compile(sourcePath);
        _shaders[sourcePath] = shader;
        return shader;
    }

    public void Reset()
    {
        _shaders.Clear();
    }
}

public abstract class NiTimeController : NiObject
{
    public const int APP_INIT = 0;
    public const int LOOP = 1;

    public bool Active { get; private set; } = true;
    public float Time { get; private set; }

    public virtual void Start(float time)
    {
        Active = true;
        Time = time;
    }

    public virtual void Stop()
    {
        Active = false;
    }

    public virtual void Update(float time)
    {
        Time = time;
    }
}

public sealed class NiAnimation : NiTimeController
{
    public string AnimationName { get; private set; } = string.Empty;
    public int LoopCount { get; private set; }

    public void Configure(string animationName, int loopCount = 0)
    {
        AnimationName = animationName;
        LoopCount = loopCount;
    }
}

public sealed class NiMilesAudioSystem
{
    public bool IsStarted { get; private set; }
    public string RedistDirectory { get; private set; } = string.Empty;

    public bool Startup(string redistDirectory)
    {
        RedistDirectory = redistDirectory;
        IsStarted = true;
        return true;
    }

    public void Shutdown()
    {
        IsStarted = false;
    }
}

public sealed class NiMilesAudioSDM
{
    public NiMilesAudioSystem AudioSystem { get; } = new();

    public bool Init(string redistDirectory)
    {
        return AudioSystem.Startup(redistDirectory);
    }

    public void Shutdown()
    {
        AudioSystem.Shutdown();
    }
}

public static class NiDX9Renderer
{
    public enum FrameBufferFormat
    {
        FBFMT_UNKNOWN
    }

    public enum DepthStencilFormat
    {
        DSFMT_UNKNOWN
    }
}
