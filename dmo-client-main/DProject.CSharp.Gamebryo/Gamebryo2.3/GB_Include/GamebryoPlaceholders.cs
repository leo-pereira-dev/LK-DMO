using Gb = GamebryoSharp;

namespace DProject.Gamebryo;

public static class GamebryoSharpIntegration
{
    public static bool IsEnabled => true;

    public static readonly IReadOnlySet<string> IntegratedCompleteTypes = new HashSet<string>(StringComparer.Ordinal)
    {
        nameof(NiRuntime),
        "NiInitOptions",
        "NiStaticDataManager",
        "NiMemManager",
        nameof(NiPoint2),
        nameof(NiPoint3),
        nameof(NiMatrix3),
        nameof(NiQuaternion),
        nameof(NiTransform),
        nameof(NiPlane),
        nameof(NiBound),
        nameof(NiFrustum),
        nameof(NiColor),
        nameof(NiColorA),
        nameof(NiFile),
        nameof(NiFilename),
        nameof(NiSearchPath),
        nameof(NiTexture),
        nameof(NiTexturingProperty),
        nameof(NiMaterialProperty),
        nameof(NiCamera),
        nameof(NiLight),
        nameof(NiAmbientLight),
        nameof(NiDirectionalLight),
        nameof(NiPointLight),
        nameof(NiSpotLight),
        nameof(NiRefObject),
        nameof(NiProperty),
        nameof(NiPropertyState),
        nameof(NiAlphaProperty),
        nameof(NiZBufferProperty),
        nameof(NiStencilProperty),
        nameof(NiVertexColorProperty),
        nameof(NiFogProperty),
        nameof(NiRendererSpecificProperty),
        nameof(Ni2DBuffer),
        nameof(NiDepthStencilBuffer),
        nameof(NiRenderTargetGroup),
        nameof(NiVisibleArray),
        nameof(NiCullingProcess),
        nameof(NiRenderView),
        nameof(Ni2DRenderView),
        nameof(Ni3DRenderView),
        nameof(NiDefaultClickRenderStep)
    };

    public static readonly IReadOnlyDictionary<string, Type> NativeCompleteTypes = new Dictionary<string, Type>(StringComparer.Ordinal)
    {
        ["NiInitOptions"] = typeof(Gb.NiInitOptions),
        ["NiStaticDataManager"] = typeof(Gb.NiStaticDataManager),
        ["NiMemManager"] = typeof(Gb.NiMemManager),
        [nameof(NiPoint2)] = typeof(Gb.NiPoint2),
        [nameof(NiPoint3)] = typeof(Gb.NiPoint3),
        [nameof(NiMatrix3)] = typeof(Gb.NiMatrix3),
        [nameof(NiQuaternion)] = typeof(Gb.NiQuaternion),
        [nameof(NiTransform)] = typeof(Gb.NiTransform),
        [nameof(NiPlane)] = typeof(Gb.NiPlane),
        [nameof(NiBound)] = typeof(Gb.NiBound),
        [nameof(NiFrustum)] = typeof(Gb.NiFrustum),
        [nameof(NiColor)] = typeof(Gb.NiColor),
        [nameof(NiColorA)] = typeof(Gb.NiColorA),
        [nameof(NiFile)] = typeof(Gb.NiFile),
        [nameof(NiFilename)] = typeof(Gb.NiFilename),
        [nameof(NiSearchPath)] = typeof(Gb.NiSearchPath),
        [nameof(NiTexture)] = typeof(Gb.NiTexture),
        [nameof(NiTexturingProperty)] = typeof(Gb.NiTexturingProperty),
        [nameof(NiMaterialProperty)] = typeof(Gb.NiMaterialProperty),
        [nameof(NiCamera)] = typeof(Gb.NiCamera),
        [nameof(NiLight)] = typeof(Gb.NiLight),
        [nameof(NiAmbientLight)] = typeof(Gb.NiAmbientLight),
        [nameof(NiDirectionalLight)] = typeof(Gb.NiDirectionalLight),
        [nameof(NiPointLight)] = typeof(Gb.NiPointLight),
        [nameof(NiSpotLight)] = typeof(Gb.NiSpotLight),
        [nameof(NiRefObject)] = typeof(Gb.NiRefObject),
        [nameof(NiProperty)] = typeof(Gb.NiProperty),
        [nameof(NiPropertyState)] = typeof(Gb.NiPropertyState),
        [nameof(NiAlphaProperty)] = typeof(Gb.NiAlphaProperty),
        [nameof(NiZBufferProperty)] = typeof(Gb.NiZBufferProperty),
        [nameof(NiStencilProperty)] = typeof(Gb.NiStencilProperty),
        [nameof(NiVertexColorProperty)] = typeof(Gb.NiVertexColorProperty),
        [nameof(NiFogProperty)] = typeof(Gb.NiFogProperty),
        [nameof(NiRendererSpecificProperty)] = typeof(Gb.NiRendererSpecificProperty),
        [nameof(Ni2DBuffer)] = typeof(Gb.Ni2DBuffer),
        [nameof(NiDepthStencilBuffer)] = typeof(Gb.NiDepthStencilBuffer),
        [nameof(NiRenderTargetGroup)] = typeof(Gb.NiRenderTargetGroup),
        [nameof(NiVisibleArray)] = typeof(Gb.NiVisibleArray),
        [nameof(NiCullingProcess)] = typeof(Gb.NiCullingProcess),
        [nameof(NiRenderView)] = typeof(Gb.NiRenderView),
        [nameof(Ni2DRenderView)] = typeof(Gb.Ni2DRenderView),
        [nameof(Ni3DRenderView)] = typeof(Gb.Ni3DRenderView),
        [nameof(NiDefaultClickRenderStep)] = typeof(Gb.NiDefaultClickRenderStep)
    };

    public static bool TryGetNativeType(string name, out Type? nativeType) =>
        NativeCompleteTypes.TryGetValue(name, out nativeType);
}

public static class NiRuntime
{
    private static bool _initialized;

    public static void NiInit()
    {
        if (_initialized)
        {
            return;
        }

        Gb.NiStaticDataManager.Init(new Gb.NiInitOptions());
        _initialized = true;
    }

    public static void NiShutdown()
    {
        if (!_initialized)
        {
            return;
        }

        Gb.NiStaticDataManager.Shutdown();
        _initialized = false;
    }
}

public readonly record struct NiPoint2(float X, float Y)
{
    public static readonly NiPoint2 Zero = new(0.0f, 0.0f);

    public Gb.NiPoint2 ToGamebryoSharp() => new(X, Y);
    public static NiPoint2 FromGamebryoSharp(Gb.NiPoint2 value) => new(value.X, value.Y);

    public static explicit operator Gb.NiPoint2(NiPoint2 value) => value.ToGamebryoSharp();
    public static explicit operator NiPoint2(Gb.NiPoint2 value) => FromGamebryoSharp(value);

    public static NiPoint2 operator +(NiPoint2 left, NiPoint2 right) =>
        FromGamebryoSharp(left.ToGamebryoSharp() + right.ToGamebryoSharp());

    public static NiPoint2 operator -(NiPoint2 left, NiPoint2 right) =>
        FromGamebryoSharp(left.ToGamebryoSharp() - right.ToGamebryoSharp());

    public static NiPoint2 operator *(NiPoint2 point, float scale) =>
        FromGamebryoSharp(point.ToGamebryoSharp() * scale);

    public float Length() => ToGamebryoSharp().Length();

    public float Dot(NiPoint2 other) => ToGamebryoSharp().Dot(other.ToGamebryoSharp());
}

public readonly record struct NiPoint3(float X, float Y, float Z)
{
    public static readonly NiPoint3 Zero = new(0.0f, 0.0f, 0.0f);

    public Gb.NiPoint3 ToGamebryoSharp() => new(X, Y, Z);
    public static NiPoint3 FromGamebryoSharp(Gb.NiPoint3 value) => new(value.X, value.Y, value.Z);

    public static explicit operator Gb.NiPoint3(NiPoint3 value) => value.ToGamebryoSharp();
    public static explicit operator NiPoint3(Gb.NiPoint3 value) => FromGamebryoSharp(value);

    public static NiPoint3 operator +(NiPoint3 left, NiPoint3 right) =>
        FromGamebryoSharp(left.ToGamebryoSharp() + right.ToGamebryoSharp());

    public static NiPoint3 operator -(NiPoint3 left, NiPoint3 right) =>
        FromGamebryoSharp(left.ToGamebryoSharp() - right.ToGamebryoSharp());

    public static NiPoint3 operator *(NiPoint3 point, float scale) =>
        FromGamebryoSharp(point.ToGamebryoSharp() * scale);

    public float Length() => ToGamebryoSharp().Length();

    public NiPoint3 Unitize()
    {
        Gb.NiPoint3 value = ToGamebryoSharp();
        value.Unitize();
        return FromGamebryoSharp(value);
    }

    public float Dot(NiPoint3 other) => ToGamebryoSharp().Dot(other.ToGamebryoSharp());

    public NiPoint3 WithZ(float z) => new(X, Y, z);
}

public sealed record NiColor(float R, float G, float B)
{
    public static readonly NiColor White = new(1.0f, 1.0f, 1.0f);

    public Gb.NiColor ToGamebryoSharp() => new(R, G, B);
    public static NiColor FromGamebryoSharp(Gb.NiColor value) => new(value.R, value.G, value.B);
}

public sealed record NiColorA(float R, float G, float B, float A)
{
    public static readonly NiColorA White = new(1.0f, 1.0f, 1.0f, 1.0f);

    public Gb.NiColorA ToGamebryoSharp() => new(R, G, B, A);
    public Gb.NiColor ToGamebryoSharpColor() => new(R, G, B);
    public static NiColorA FromGamebryoSharp(Gb.NiColorA value) => new(value.R, value.G, value.B, value.A);
    public static NiColorA FromGamebryoSharp(Gb.NiColor value, float alpha = 1.0f) => new(value.R, value.G, value.B, alpha);
}

public readonly record struct NiMatrix3(Gb.NiMatrix3 Native)
{
    public static readonly NiMatrix3 Identity = new(Gb.NiMatrix3.IDENTITY);
    public static readonly NiMatrix3 Zero = new(Gb.NiMatrix3.ZERO);

    public NiMatrix3()
        : this(Gb.NiMatrix3.IDENTITY)
    {
    }

    public NiMatrix3(NiPoint3 col0, NiPoint3 col1, NiPoint3 col2)
        : this(new Gb.NiMatrix3(col0.ToGamebryoSharp(), col1.ToGamebryoSharp(), col2.ToGamebryoSharp()))
    {
    }

    public Gb.NiMatrix3 ToGamebryoSharp() => Native;
    public static NiMatrix3 FromGamebryoSharp(Gb.NiMatrix3 value) => new(value);
}

public readonly record struct NiQuaternion(Gb.NiQuaternion Native)
{
    public static readonly NiQuaternion Identity = new(Gb.NiQuaternion.IDENTITY);

    public NiQuaternion(float w, float x, float y, float z)
        : this(new Gb.NiQuaternion(w, x, y, z))
    {
    }

    public NiQuaternion(float angle, NiPoint3 axis)
        : this(new Gb.NiQuaternion(angle, axis.ToGamebryoSharp()))
    {
    }

    public Gb.NiQuaternion ToGamebryoSharp() => Native;
    public static NiQuaternion FromGamebryoSharp(Gb.NiQuaternion value) => new(value);
}

public readonly record struct NiTransform(Gb.NiTransform Native)
{
    public static readonly NiTransform Identity = new(Gb.NiTransform.IDENTITY);

    public NiTransform()
        : this(Gb.NiTransform.IDENTITY)
    {
    }

    public NiTransform(NiMatrix3 rotate, NiPoint3 translate, float scale = 1.0f)
        : this(new Gb.NiTransform
        {
            m_Rotate = rotate.ToGamebryoSharp(),
            m_Translate = translate.ToGamebryoSharp(),
            m_fScale = scale
        })
    {
    }

    public NiMatrix3 Rotate => NiMatrix3.FromGamebryoSharp(Native.m_Rotate);
    public NiPoint3 Translate => NiPoint3.FromGamebryoSharp(Native.m_Translate);
    public float Scale => Native.m_fScale;

    public Gb.NiTransform ToGamebryoSharp() => Native;
    public static NiTransform FromGamebryoSharp(Gb.NiTransform value) => new(value);
}

public readonly record struct NiPlane(Gb.NiPlane Native)
{
    public NiPlane()
        : this(new Gb.NiPlane())
    {
    }

    public NiPlane(NiPoint3 normal, float constant)
        : this(new Gb.NiPlane(normal.ToGamebryoSharp(), constant))
    {
    }

    public NiPlane(NiPoint3 normal, NiPoint3 point)
        : this(new Gb.NiPlane(normal.ToGamebryoSharp(), point.ToGamebryoSharp()))
    {
    }

    public Gb.NiPlane ToGamebryoSharp() => Native;
    public static NiPlane FromGamebryoSharp(Gb.NiPlane value) => new(value);
}

public readonly record struct NiBound(Gb.NiBound Native)
{
    public NiBound()
        : this(new Gb.NiBound())
    {
    }

    public NiBound(NiPoint3 center, float radius)
        : this(new Gb.NiBound
        {
            Center = center.ToGamebryoSharp(),
            Radius = radius
        })
    {
    }

    public NiPoint3 Center => NiPoint3.FromGamebryoSharp(Native.Center);
    public float Radius => Native.Radius;

    public Gb.NiBound ToGamebryoSharp() => Native;
    public static NiBound FromGamebryoSharp(Gb.NiBound value) => new(value);
}

public sealed class NiFrustum
{
    public NiFrustum(float left, float right, float top, float bottom, float nearPlane, float farPlane, bool ortho = false)
    {
        Left = left;
        Right = right;
        Top = top;
        Bottom = bottom;
        NearPlane = nearPlane;
        FarPlane = farPlane;
        Ortho = ortho;
    }

    public float Left { get; }
    public float Right { get; }
    public float Top { get; }
    public float Bottom { get; }
    public float NearPlane { get; }
    public float FarPlane { get; }
    public bool Ortho { get; }

    public Gb.NiFrustum ToGamebryoSharp() => new(Left, Right, Top, Bottom, NearPlane, FarPlane, Ortho);

    public static NiFrustum FromGamebryoSharp(Gb.NiFrustum value) =>
        new(value.m_fLeft, value.m_fRight, value.m_fTop, value.m_fBottom, value.m_fNear, value.m_fFar, value.m_bOrtho);
}

public class NiRefObject
{
    private int _fallbackRefCount = 1;

    protected NiRefObject()
    {
    }

    protected NiRefObject(Gb.NiRefObject? native)
    {
        NativeRefObject = native;
        if (NativeRefObject is not null && NativeRefObject.GetRefCount() == 0)
        {
            NativeRefObject.IncRefCount();
        }
    }

    public Gb.NiRefObject? NativeRefObject { get; protected set; }

    public uint GetRefCount() => NativeRefObject?.GetRefCount() ?? (uint)Math.Max(0, _fallbackRefCount);

    public void IncRefCount()
    {
        if (NativeRefObject is not null)
        {
            NativeRefObject.IncRefCount();
            return;
        }

        _fallbackRefCount++;
    }

    public void DecRefCount()
    {
        if (NativeRefObject is not null)
        {
            NativeRefObject.DecRefCount();
            return;
        }

        if (_fallbackRefCount > 0)
        {
            _fallbackRefCount--;
        }
    }
}

public class NiObject : NiRefObject
{
    public NiObject()
    {
    }

    protected NiObject(Gb.NiObject? native)
        : base(native)
    {
        NativeObject = native;
    }

    public string Name { get; private set; } = string.Empty;
    public Gb.NiObject? NativeObject { get; protected set; }
    public object? NativeGamebryoObject => NativeObject ?? NativeRefObject;

    public virtual NiObject Clone()
    {
        return NativeObject is null
            ? (NiObject)MemberwiseClone()
            : new NiObject(NativeObject.Clone());
    }

    public virtual void SetName(string name)
    {
        Name = name;
        if (NativeObject is Gb.NiObjectNET objectNet)
        {
            objectNet.SetName(name);
        }
    }
}

public class NiAVObject : NiObject
{
    public NiAVObject()
    {
    }

    protected NiAVObject(Gb.NiAVObject native)
        : base(native)
    {
        NativeAVObject = native;
        Translate = NiPoint3.FromGamebryoSharp(native.GetTranslate());
        Scale = new NiPoint3(native.GetScale(), native.GetScale(), native.GetScale());
        AppCulled = native.GetAppCulled();
    }

    public NiPoint3 Translate { get; private set; } = NiPoint3.Zero;
    public NiPoint3 Scale { get; private set; } = new(1.0f, 1.0f, 1.0f);
    public bool AppCulled { get; private set; }
    public Gb.NiAVObject? NativeAVObject { get; protected set; }

    public virtual void SetTranslate(NiPoint3 translate)
    {
        Translate = translate;
        NativeAVObject?.SetTranslate(translate.ToGamebryoSharp());
    }

    public virtual NiPoint3 GetTranslate()
    {
        return NativeAVObject is null
            ? Translate
            : NiPoint3.FromGamebryoSharp(NativeAVObject.GetTranslate());
    }

    public virtual void SetScale(float scale)
    {
        Scale = new NiPoint3(scale, scale, scale);
        NativeAVObject?.SetScale(scale);
    }

    public virtual void SetAppCulled(bool culled)
    {
        AppCulled = culled;
        NativeAVObject?.SetAppCulled(culled);
    }

    public virtual void Update(float time)
    {
        NativeAVObject?.Update(time);
    }
}

public class NiNode : NiAVObject
{
    private readonly List<NiAVObject> _children = [];

    public NiNode()
    {
    }

    public IReadOnlyList<NiAVObject> Children => _children;
    public Gb.NiNode? NativeNode { get; }

    public void AttachChild(NiAVObject child)
    {
        ArgumentNullException.ThrowIfNull(child);
        _children.Add(child);
        if (NativeNode is not null && child.NativeAVObject is not null)
        {
            NativeNode.AttachChild(child.NativeAVObject);
        }
    }

    public bool DetachChild(NiAVObject child)
    {
        if (NativeNode is not null && child.NativeAVObject is not null)
        {
            NativeNode.DetachChild(child.NativeAVObject);
        }

        return _children.Remove(child);
    }

    public NiAVObject? GetAt(int index)
    {
        return index >= 0 && index < _children.Count ? _children[index] : null;
    }

    public override void Update(float time)
    {
        NativeNode?.Update(time);
        foreach (NiAVObject child in _children)
        {
            child.Update(time);
        }
    }
}

public class NiCamera : NiAVObject
{
    public NiCamera()
        : this(new Gb.NiCamera())
    {
    }

    private NiCamera(Gb.NiCamera native)
        : base(native)
    {
        NativeCamera = native;
    }

    public float FieldOfView { get; private set; } = 60.0f;
    public float NearPlane { get; private set; } = 1.0f;
    public float FarPlane { get; private set; } = 10000.0f;
    public Gb.NiCamera NativeCamera { get; }

    public void SetViewFrustum(float fieldOfView, float nearPlane, float farPlane)
    {
        FieldOfView = fieldOfView;
        NearPlane = Math.Max(0.1f, nearPlane);
        FarPlane = Math.Max(NearPlane + 1.0f, farPlane);

        float halfRadians = FieldOfView * MathF.PI / 360.0f;
        float top = MathF.Tan(halfRadians) * NearPlane;
        float right = top * (4.0f / 3.0f);
        NativeCamera.SetViewFrustum(new Gb.NiFrustum(-right, right, top, -top, NearPlane, FarPlane));
    }
}

public class NiLight : NiAVObject
{
    public NiLight()
        : this(new Gb.NiDirectionalLight())
    {
    }

    protected NiLight(Gb.NiLight native)
        : base(native)
    {
        NativeLight = native;
    }

    public NiColor DiffuseColor { get; private set; } = NiColor.White;
    public float Dimmer { get; private set; } = 1.0f;
    public Gb.NiLight NativeLight { get; }

    public void SetDiffuseColor(NiColor color)
    {
        DiffuseColor = color;
        NativeLight.SetDiffuseColor(color.ToGamebryoSharp());
    }

    public void SetDimmer(float dimmer)
    {
        Dimmer = dimmer;
        NativeLight.SetDimmer(dimmer);
    }
}

public sealed class NiAmbientLight : NiLight
{
    public NiAmbientLight()
        : base(new Gb.NiAmbientLight())
    {
        NativeAmbientLight = (Gb.NiAmbientLight)NativeLight;
    }

    public Gb.NiAmbientLight NativeAmbientLight { get; }
}

public sealed class NiDirectionalLight : NiLight
{
    public NiDirectionalLight()
        : base(new Gb.NiDirectionalLight())
    {
        NativeDirectionalLight = (Gb.NiDirectionalLight)NativeLight;
    }

    public Gb.NiDirectionalLight NativeDirectionalLight { get; }
}

public class NiPointLight : NiLight
{
    public NiPointLight()
        : this(new Gb.NiPointLight())
    {
    }

    protected NiPointLight(Gb.NiPointLight native)
        : base(native)
    {
        NativePointLight = native;
    }

    public Gb.NiPointLight NativePointLight { get; }

    public void SetConstantAttenuation(float value) => NativePointLight.SetConstantAttenuation(value);
    public void SetLinearAttenuation(float value) => NativePointLight.SetLinearAttenuation(value);
    public void SetQuadraticAttenuation(float value) => NativePointLight.SetQuadraticAttenuation(value);
}

public sealed class NiSpotLight : NiPointLight
{
    public NiSpotLight()
        : base(new Gb.NiSpotLight())
    {
        NativeSpotLight = (Gb.NiSpotLight)NativePointLight;
    }

    public Gb.NiSpotLight NativeSpotLight { get; }

    public void SetSpotAngle(float value) => NativeSpotLight.SetSpotAngle(value);
    public void SetInnerSpotAngle(float value) => NativeSpotLight.SetInnerSpotAngle(value);
    public void SetSpotExponent(float value) => NativeSpotLight.SetSpotExponent(value);
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

public readonly record struct NiRectF(float Left, float Right, float Top, float Bottom)
{
    public Gb.NiRect<float> ToGamebryoSharp() => new(Left, Right, Top, Bottom);
}

public class Ni2DBuffer : NiObject
{
    protected Ni2DBuffer(Gb.Ni2DBuffer? native, uint width, uint height)
        : base(native)
    {
        Native2DBuffer = native;
        Width = native?.GetWidth() ?? width;
        Height = native?.GetHeight() ?? height;
    }

    public uint Width { get; private set; }
    public uint Height { get; private set; }
    public Gb.Ni2DBuffer? Native2DBuffer { get; protected set; }

    public static Ni2DBuffer Create(uint width, uint height)
    {
        Gb.Ni2DBuffer native = Gb.Ni2DBuffer.Create(width, height);
        return new Ni2DBuffer(native, width, height);
    }

    public void ResetDimensions(uint width, uint height)
    {
        Width = width;
        Height = height;
        Native2DBuffer?.ResetDimensions(width, height);
    }
}

public sealed class NiDepthStencilBuffer : Ni2DBuffer
{
    private NiDepthStencilBuffer(Gb.NiDepthStencilBuffer? native, uint width, uint height)
        : base(native, width, height)
    {
        NativeDepthStencilBuffer = native;
    }

    public Gb.NiDepthStencilBuffer? NativeDepthStencilBuffer { get; }

    public static new NiDepthStencilBuffer Create(uint width, uint height)
    {
        Gb.NiDepthStencilBuffer native = Gb.NiDepthStencilBuffer.Create(width, height, null);
        return new NiDepthStencilBuffer(native, width, height);
    }
}

public sealed class NiRenderTargetGroup : NiObject
{
    private readonly Dictionary<uint, Ni2DBuffer> _buffers = new();

    public IReadOnlyDictionary<uint, Ni2DBuffer> Buffers => _buffers;
    public NiDepthStencilBuffer? DepthStencilBuffer { get; private set; }
    public Gb.NiRenderTargetGroup? NativeRenderTargetGroup { get; private set; }

    public uint GetBufferCount() => (uint)_buffers.Count;

    public bool AttachBuffer(Ni2DBuffer buffer, uint index)
    {
        ArgumentNullException.ThrowIfNull(buffer);
        _buffers[index] = buffer;

        if (NativeRenderTargetGroup is not null && buffer.Native2DBuffer is not null)
        {
            return NativeRenderTargetGroup.AttachBuffer(buffer.Native2DBuffer, index);
        }

        return true;
    }

    public bool AttachDepthStencilBuffer(NiDepthStencilBuffer buffer)
    {
        ArgumentNullException.ThrowIfNull(buffer);
        DepthStencilBuffer = buffer;

        if (NativeRenderTargetGroup is not null && buffer.NativeDepthStencilBuffer is not null)
        {
            return NativeRenderTargetGroup.AttachDepthStencilBuffer(buffer.NativeDepthStencilBuffer);
        }

        return true;
    }

    public Ni2DBuffer? GetBuffer(uint index) =>
        _buffers.TryGetValue(index, out Ni2DBuffer? buffer) ? buffer : null;
}

public sealed class NiVisibleArray
{
    private readonly List<NiAVObject> _visibleObjects = new();

    public NiVisibleArray(uint maxSize = 1024, uint growBy = 1024)
    {
        NativeVisibleArray = new Gb.NiVisibleArray(maxSize, growBy);
    }

    public IReadOnlyList<NiAVObject> VisibleObjects => _visibleObjects;
    public Gb.NiVisibleArray NativeVisibleArray { get; }

    public void Add(NiAVObject visible)
    {
        ArgumentNullException.ThrowIfNull(visible);
        _visibleObjects.Add(visible);

        if (visible.NativeAVObject is Gb.NiGeometry nativeGeometry)
        {
            NativeVisibleArray.Add(nativeGeometry);
        }
    }

    public void RemoveAll()
    {
        _visibleObjects.Clear();
        NativeVisibleArray.RemoveAll();
    }

    public uint GetCount() => (uint)_visibleObjects.Count;

    public NiAVObject GetAt(uint index) => _visibleObjects[checked((int)index)];
}

public sealed class NiCullingProcess : NiRefObject
{
    public NiCullingProcess(NiVisibleArray? visibleSet = null)
        : base(new Gb.NiCullingProcess(visibleSet?.NativeVisibleArray))
    {
        VisibleSet = visibleSet;
        NativeCullingProcess = (Gb.NiCullingProcess)NativeRefObject!;
    }

    public NiVisibleArray? VisibleSet { get; private set; }
    public Gb.NiCullingProcess NativeCullingProcess { get; }

    public void SetVisibleSet(NiVisibleArray? visibleSet)
    {
        VisibleSet = visibleSet;
        NativeCullingProcess.SetVisibleSet(visibleSet?.NativeVisibleArray);
    }

    public void Process(NiCamera? camera, NiAVObject? scene, NiVisibleArray? visibleSet = null)
    {
        if (camera is null || scene is null)
        {
            return;
        }

        NiVisibleArray? targetVisibleSet = visibleSet ?? VisibleSet;
        if (scene.NativeAVObject is not null)
        {
            NativeCullingProcess.Process(camera.NativeCamera, scene.NativeAVObject, targetVisibleSet?.NativeVisibleArray);
        }

        targetVisibleSet?.Add(scene);
    }
}

public abstract class NiRenderView : NiRefObject
{
    protected NiRenderView(Gb.NiRenderView native)
        : base(native)
    {
        NativeRenderView = native;
    }

    public string? Name { get; private set; }
    public Gb.NiRenderView NativeRenderView { get; }

    public void SetName(string? name)
    {
        Name = name;
        NativeRenderView.SetName(name);
    }

    public string? GetName() => NativeRenderView.GetName() ?? Name;

    public abstract NiVisibleArray GetPVGeometry(uint frameId);
}

public sealed class Ni2DRenderView : NiRenderView
{
    public Ni2DRenderView()
        : base(new Gb.Ni2DRenderView())
    {
        Native2DRenderView = (Gb.Ni2DRenderView)NativeRenderView;
        VisibleSet = new NiVisibleArray(16, 16);
    }

    public Gb.Ni2DRenderView Native2DRenderView { get; }
    public NiVisibleArray VisibleSet { get; }

    public void SetCameraData(NiRectF viewport)
    {
        Native2DRenderView.SetCameraData(viewport.ToGamebryoSharp());
    }

    public override NiVisibleArray GetPVGeometry(uint frameId)
    {
        Native2DRenderView.GetPVGeometry(frameId);
        return VisibleSet;
    }
}

public sealed class Ni3DRenderView : NiRenderView
{
    private readonly List<NiAVObject> _scenes = new();
    private readonly NiVisibleArray _visibleSet = new(16, 16);

    public Ni3DRenderView(NiCamera? camera = null, NiCullingProcess? cullingProcess = null, bool alwaysUseCameraViewport = true)
        : base(new Gb.Ni3DRenderView(camera?.NativeCamera, cullingProcess?.NativeCullingProcess, alwaysUseCameraViewport))
    {
        Camera = camera;
        CullingProcess = cullingProcess;
        Native3DRenderView = (Gb.Ni3DRenderView)NativeRenderView;
    }

    public NiCamera? Camera { get; private set; }
    public NiCullingProcess? CullingProcess { get; private set; }
    public Gb.Ni3DRenderView Native3DRenderView { get; }
    public IReadOnlyList<NiAVObject> Scenes => _scenes;

    public void SetCamera(NiCamera? camera)
    {
        Camera = camera;
        Native3DRenderView.SetCamera(camera?.NativeCamera);
    }

    public void SetCullingProcess(NiCullingProcess? cullingProcess)
    {
        CullingProcess = cullingProcess;
        Native3DRenderView.SetCullingProcess(cullingProcess?.NativeCullingProcess);
    }

    public void AppendScene(NiAVObject scene)
    {
        ArgumentNullException.ThrowIfNull(scene);
        _scenes.Add(scene);
        if (scene.NativeAVObject is not null)
        {
            Native3DRenderView.AppendScene(scene.NativeAVObject);
        }
    }

    public void RemoveAllScenes()
    {
        _scenes.Clear();
        Native3DRenderView.RemoveAllScenes();
    }

    public void SetCameraData(NiRectF viewport)
    {
        Native3DRenderView.SetCameraData(viewport.ToGamebryoSharp());
    }

    public override NiVisibleArray GetPVGeometry(uint frameId)
    {
        _visibleSet.RemoveAll();
        foreach (NiAVObject scene in _scenes)
        {
            CullingProcess?.Process(Camera, scene, _visibleSet);
            if (CullingProcess is null)
            {
                _visibleSet.Add(scene);
            }
        }

        Native3DRenderView.GetPVGeometry(frameId);
        return _visibleSet;
    }
}

public sealed class NiDefaultClickRenderStep : NiRefObject
{
    public NiDefaultClickRenderStep()
        : base(new Gb.NiDefaultClickRenderStep())
    {
        NativeDefaultClickRenderStep = (Gb.NiDefaultClickRenderStep)NativeRefObject!;
    }

    public NiRenderTargetGroup? OutputRenderTargetGroup { get; private set; }
    public Gb.NiDefaultClickRenderStep NativeDefaultClickRenderStep { get; }

    public void SetName(string? name) => NativeDefaultClickRenderStep.SetName(name);
    public string? GetName() => NativeDefaultClickRenderStep.GetName();
    public void SetActive(bool active) => NativeDefaultClickRenderStep.SetActive(active);
    public bool GetActive() => NativeDefaultClickRenderStep.GetActive();

    public bool SetOutputRenderTargetGroup(NiRenderTargetGroup? outputRenderTargetGroup)
    {
        OutputRenderTargetGroup = outputRenderTargetGroup;
        return true;
    }

    public void Render() => NativeDefaultClickRenderStep.Render();
}

public sealed class NiFile : IDisposable
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
        NativeFile = new Gb.NiFile(path, ToNativeOpenMode(mode), bufferSize == 0 ? 32768 : bufferSize);
    }

    public string Path { get; }
    public OpenMode Mode { get; }
    public uint BufferSize { get; }
    public Gb.NiFile NativeFile { get; }
    public bool IsValid => NativeFile.IsValid();

    public void Dispose()
    {
        NativeFile.Dispose();
    }

    private static Gb.NiFile.OpenMode ToNativeOpenMode(OpenMode mode)
    {
        return mode switch
        {
            OpenMode.READ_ONLY => Gb.NiFile.OpenMode.READ_ONLY,
            OpenMode.WRITE_ONLY => Gb.NiFile.OpenMode.WRITE_ONLY,
            OpenMode.APPEND_ONLY => Gb.NiFile.OpenMode.APPEND_ONLY,
            _ => Gb.NiFile.OpenMode.READ_ONLY
        };
    }
}

public sealed class NiFilename
{
    public NiFilename(string? fullPath)
    {
        NativeFilename = new Gb.NiFilename(fullPath);
    }

    public Gb.NiFilename NativeFilename { get; }

    public string Directory => NativeFilename.GetDir();
    public string Drive => NativeFilename.GetDrive();
    public string Extension => NativeFilename.GetExt();
    public string Filename => NativeFilename.GetFilename();
    public string PlatformSubDirectory => NativeFilename.GetPlatformSubDir();

    public void SetDir(string? directory) => NativeFilename.SetDir(directory);
    public void SetDrive(string? drive) => NativeFilename.SetDrive(drive);
    public void SetExt(string? extension) => NativeFilename.SetExt(extension);
    public void SetFilename(string? filename) => NativeFilename.SetFilename(filename);
    public void SetPlatformSubDir(string? subDirectory) => NativeFilename.SetPlatformSubDir(subDirectory);

    public bool TryGetFullPath(out string fullPath, uint maxLength = 260) =>
        NativeFilename.GetFullPath(out fullPath, maxLength);
}

public sealed class NiSearchPath
{
    public NiSearchPath()
    {
        NativeSearchPath = new Gb.NiSearchPath();
    }

    public Gb.NiSearchPath NativeSearchPath { get; }

    public void SetFilePath(string? filePath) => NativeSearchPath.SetFilePath(filePath);
    public void SetReferencePath(string? referencePath) => NativeSearchPath.SetReferencePath(referencePath);
    public void Reset() => NativeSearchPath.Reset();

    public bool TryGetNextSearchPath(out string path, uint maxLength = 260) =>
        NativeSearchPath.GetNextSearchPath(out path, maxLength);

    public static void SetDefaultPath(string? path) => Gb.NiSearchPath.SetDefaultPath(path);
    public static string GetDefaultPath() => Gb.NiSearchPath.GetDefaultPath();
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

        public Gb.NiTexture.FormatPrefs ToGamebryoSharp()
        {
            return new Gb.NiTexture.FormatPrefs
            {
                m_ePixelLayout = m_ePixelLayout switch
                {
                    PixelLayout.PALETTIZED_8 => Gb.NiTexture.FormatPrefs.PixelLayout.PALETTIZED_8,
                    PixelLayout.HIGH_COLOR_16 => Gb.NiTexture.FormatPrefs.PixelLayout.HIGH_COLOR_16,
                    PixelLayout.TRUE_COLOR_32 => Gb.NiTexture.FormatPrefs.PixelLayout.TRUE_COLOR_32,
                    PixelLayout.COMPRESSED => Gb.NiTexture.FormatPrefs.PixelLayout.COMPRESSED,
                    PixelLayout.BUMPMAP => Gb.NiTexture.FormatPrefs.PixelLayout.BUMPMAP,
                    _ => Gb.NiTexture.FormatPrefs.PixelLayout.PIX_DEFAULT
                },
                m_eMipMapped = m_eMipMapped switch
                {
                    MipMapFlag.NO => Gb.NiTexture.FormatPrefs.MipFlag.NO,
                    MipMapFlag.YES => Gb.NiTexture.FormatPrefs.MipFlag.YES,
                    _ => Gb.NiTexture.FormatPrefs.MipFlag.MIP_DEFAULT
                }
            };
        }
    }

    public NiTexture()
    {
    }

    protected NiTexture(Gb.NiTexture? native)
        : base(native)
    {
        NativeTexture = native;
    }

    public static bool MipmapByDefault => Gb.NiTexture.GetMipmapByDefault();
    public string Path { get; protected set; } = string.Empty;
    public FormatPrefs Preferences { get; protected set; } = new();
    public Gb.NiTexture? NativeTexture { get; protected set; }

    public static void SetMipmapByDefault(bool value)
    {
        Gb.NiTexture.SetMipmapByDefault(value);
    }
}

public class NiSourceTexture : NiTexture
{
    private static bool _useMipmapping;
    private static bool _usePreloading = true;
    private static bool _destroyAppDataFlag;

    private NiSourceTexture()
    {
    }

    public static bool UseMipmapping => _useMipmapping;
    public static bool UsePreloading => _usePreloading;
    public static bool DestroyAppDataFlag => _destroyAppDataFlag;

    public static void SetUseMipmapping(bool value) => _useMipmapping = value;
    public static void SetUsePreloading(bool value) => _usePreloading = value;
    public static void SetDestroyAppDataFlag(bool value) => _destroyAppDataFlag = value;

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

public abstract class NiProperty : NiObject
{
    protected NiProperty(Gb.NiProperty native)
        : base(native)
    {
        NativeProperty = native;
    }

    public Gb.NiProperty NativeProperty { get; }

    public int Type() => NativeProperty.Type();
}

public sealed class NiPropertyState : NiRefObject
{
    private readonly Dictionary<int, NiProperty> _properties = new();

    public NiPropertyState()
        : base(new Gb.NiPropertyState())
    {
        NativePropertyState = (Gb.NiPropertyState)NativeRefObject!;
    }

    public IReadOnlyDictionary<int, NiProperty> Properties => _properties;
    public Gb.NiPropertyState NativePropertyState { get; }

    public void SetProperty(NiProperty property)
    {
        ArgumentNullException.ThrowIfNull(property);
        _properties[property.Type()] = property;
        NativePropertyState.SetProperty(property.NativeProperty);
    }

    public NiProperty? GetProperty(int type) =>
        _properties.TryGetValue(type, out NiProperty? property) ? property : null;
}

public sealed class NiTexturingProperty : NiProperty
{
    private readonly Dictionary<int, NiTexture> _textures = new();

    public NiTexturingProperty()
        : this(new Gb.NiTexturingProperty())
    {
    }

    private NiTexturingProperty(Gb.NiTexturingProperty native)
        : base(native)
    {
        NativeTexturingProperty = native;
    }

    public IReadOnlyDictionary<int, NiTexture> Textures => _textures;
    public Gb.NiTexturingProperty NativeTexturingProperty { get; }

    public void SetBaseTexture(NiTexture texture)
    {
        _textures[0] = texture;
        NativeTexturingProperty.SetBaseTexture(texture.NativeTexture);
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

public sealed class NiMaterialProperty : NiProperty
{
    public NiMaterialProperty()
        : this(new Gb.NiMaterialProperty())
    {
    }

    private NiMaterialProperty(Gb.NiMaterialProperty native)
        : base(native)
    {
        NativeMaterialProperty = native;
    }

    public NiColorA Diffuse { get; private set; } = NiColorA.White;
    public NiColorA Ambient { get; private set; } = NiColorA.White;
    public NiShader? Shader { get; private set; }
    public Gb.NiMaterialProperty NativeMaterialProperty { get; }

    public void SetDiffuseColor(NiColorA color)
    {
        Diffuse = color;
        NativeMaterialProperty.SetDiffuseColor(color.ToGamebryoSharpColor());
        NativeMaterialProperty.SetAlpha(color.A);
    }

    public void SetAmbientColor(NiColorA color)
    {
        Ambient = color;
        NativeMaterialProperty.SetAmbientColor(color.ToGamebryoSharpColor());
    }

    public void SetShader(NiShader shader)
    {
        Shader = shader;
    }
}

public sealed class NiAlphaProperty : NiProperty
{
    public NiAlphaProperty()
        : this(new Gb.NiAlphaProperty())
    {
    }

    private NiAlphaProperty(Gb.NiAlphaProperty native)
        : base(native)
    {
        NativeAlphaProperty = native;
    }

    public Gb.NiAlphaProperty NativeAlphaProperty { get; }

    public void SetAlphaBlending(bool value) => NativeAlphaProperty.SetAlphaBlending(value);
    public void SetAlphaTesting(bool value) => NativeAlphaProperty.SetAlphaTesting(value);
}

public sealed class NiZBufferProperty : NiProperty
{
    public NiZBufferProperty()
        : this(new Gb.NiZBufferProperty())
    {
    }

    private NiZBufferProperty(Gb.NiZBufferProperty native)
        : base(native)
    {
        NativeZBufferProperty = native;
    }

    public Gb.NiZBufferProperty NativeZBufferProperty { get; }

    public void SetZBufferTest(bool value) => NativeZBufferProperty.SetZBufferTest(value);
    public void SetZBufferWrite(bool value) => NativeZBufferProperty.SetZBufferWrite(value);
}

public sealed class NiStencilProperty : NiProperty
{
    public NiStencilProperty()
        : this(new Gb.NiStencilProperty())
    {
    }

    private NiStencilProperty(Gb.NiStencilProperty native)
        : base(native)
    {
        NativeStencilProperty = native;
    }

    public Gb.NiStencilProperty NativeStencilProperty { get; }

    public void SetStencilOn(bool value) => NativeStencilProperty.SetStencilOn(value);
}

public sealed class NiVertexColorProperty : NiProperty
{
    public NiVertexColorProperty()
        : this(new Gb.NiVertexColorProperty())
    {
    }

    private NiVertexColorProperty(Gb.NiVertexColorProperty native)
        : base(native)
    {
        NativeVertexColorProperty = native;
    }

    public Gb.NiVertexColorProperty NativeVertexColorProperty { get; }
}

public sealed class NiFogProperty : NiProperty
{
    public NiFogProperty()
        : this(new Gb.NiFogProperty())
    {
    }

    private NiFogProperty(Gb.NiFogProperty native)
        : base(native)
    {
        NativeFogProperty = native;
    }

    public Gb.NiFogProperty NativeFogProperty { get; }

    public void SetFog(bool value) => NativeFogProperty.SetFog(value);
    public void SetFogColor(NiColor color) => NativeFogProperty.SetFogColor(color.ToGamebryoSharp());
    public void SetDepth(float value) => NativeFogProperty.SetDepth(value);
}

public sealed class NiRendererSpecificProperty : NiProperty
{
    public NiRendererSpecificProperty()
        : this(new Gb.NiRendererSpecificProperty())
    {
    }

    private NiRendererSpecificProperty(Gb.NiRendererSpecificProperty native)
        : base(native)
    {
        NativeRendererSpecificProperty = native;
    }

    public Gb.NiRendererSpecificProperty NativeRendererSpecificProperty { get; }
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
