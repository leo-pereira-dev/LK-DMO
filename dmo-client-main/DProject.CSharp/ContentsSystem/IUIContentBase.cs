namespace DProject.ContentsSystem;

public interface IUIContentBase
{
    eContentsType ContentsType { get; }
    void Initialize();
    void Update(float elapsedTime);
}

