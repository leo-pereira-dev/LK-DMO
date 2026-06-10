using DProject.Gamebryo;

namespace DProject.Engine;

public sealed class CEngine
{
    private NiRenderer? _renderer;
    private readonly NiShaderMaterialPipeline _shaderMaterialPipeline = new();

    public NiCamera? MainCamera { get; private set; }
    public NiShaderMaterialPipeline ShaderMaterialPipeline => _shaderMaterialPipeline;
    public bool IsDeviceLost => _renderer?.IsDeviceLost ?? false;

    public void GBInit()
    {
        NiRuntime.NiInit();
        NiTexture.SetMipmapByDefault(false);
        NiSourceTexture.SetUseMipmapping(false);
        NiSourceTexture.SetUsePreloading(true);
        NiSourceTexture.SetDestroyAppDataFlag(false);
        NiMaterial.SetDefaultWorkingDirectory("Data\\Shaders\\Generated");
    }

    public void ShutDown()
    {
        NiRuntime.NiShutdown();
    }

    public bool Init()
    {
        _renderer = new NiRenderer();
        return _renderer.Create();
    }

    public bool Create()
    {
        CreateCamera();
        return true;
    }

    public void Delete()
    {
        _shaderMaterialPipeline.Reset();
        MainCamera = null;
        _renderer?.Dispose();
        _renderer = null;
    }

    public void CreateCamera()
    {
        MainCamera = new NiCamera();
        MainCamera.SetName("MainCamera");
        MainCamera.SetViewFrustum(60.0f, 1.0f, 10000.0f);
    }

    public void SetGaussianBlurVal(float radius, float intensity, float threshold)
    {
    }

    public void ResetRendererCamera()
    {
        CreateCamera();
    }

    public void ScreenSpace()
    {
    }

    public void Render()
    {
        _renderer?.Render();
    }

    public bool BeginFrame()
    {
        return _renderer?.BeginFrame() ?? false;
    }

    public void EndFrame()
    {
        _renderer?.EndFrame();
    }

    public void DisplayFrame()
    {
        _renderer?.DisplayFrame();
    }

    public void MarkDeviceLost()
    {
        _renderer?.MarkDeviceLost();
    }

    public bool ResetDevice()
    {
        bool reset = _renderer?.ResetDevice() ?? false;
        if (reset)
        {
            ResetRendererCamera();
        }

        return reset;
    }

    public NiShader GetOrCreateShader(string sourcePath)
    {
        return _shaderMaterialPipeline.GetOrCreateShader(sourcePath);
    }
}
