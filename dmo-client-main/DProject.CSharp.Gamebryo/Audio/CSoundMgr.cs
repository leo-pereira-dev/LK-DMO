using DProject.Config;
using DProject.Gamebryo;

namespace DProject.Audio;

public sealed class CSound
{
    public CSound(string path, bool is3D)
    {
        Path = path;
        Is3D = is3D;
    }

    public string Path { get; }
    public bool Is3D { get; }
    public bool IsPlaying { get; private set; }
    public float Volume { get; private set; } = 1.0f;

    public void Play() => IsPlaying = true;
    public void Stop() => IsPlaying = false;
    public void SetVolume(float volume) => Volume = Math.Clamp(volume, 0.0f, 1.0f);
}

public sealed class CSoundMgr
{
    private readonly PathOptions _paths;
    private readonly NiMilesAudioSDM _milesAudio = new();
    private readonly Dictionary<string, CSound> _sounds = new(StringComparer.OrdinalIgnoreCase);
    private readonly List<CSound> _cloneSounds = [];
    private readonly List<CSound> _systemSounds = [];

    public CSoundMgr(PathOptions paths)
    {
        _paths = paths;
    }

    public bool m_bEnableSound { get; private set; }
    public bool m_bActiveFocus { get; private set; } = true;
    public bool m_bDeleteMusic { get; private set; }
    public CSound? m_pMusicAmbient { get; private set; }
    public CSound? m_pMusicOldAmbient { get; private set; }
    public float m_fMusicVolume { get; private set; } = 1.0f;
    public float m_fSoundVolume { get; private set; } = 1.0f;
    public float m_fSystemVolume { get; private set; } = 1.0f;

    public void Init()
    {
        m_bEnableSound = _InitMiles();
        m_bDeleteMusic = false;
    }

    public void Destroy()
    {
        foreach (CSound sound in _sounds.Values)
        {
            sound.Stop();
        }

        _sounds.Clear();
        _cloneSounds.Clear();
        _systemSounds.Clear();
        m_pMusicAmbient = null;
        m_pMusicOldAmbient = null;
        _milesAudio.Shutdown();
        m_bEnableSound = false;
    }

    public void Update(float fTime)
    {
        if (!m_bEnableSound)
        {
            return;
        }

        if (m_bDeleteMusic && m_pMusicAmbient is { Volume: <= 0.0f })
        {
            m_pMusicAmbient.Stop();
            m_pMusicAmbient = null;
            m_bDeleteMusic = false;
        }
    }

    public void OnActive() => m_bActiveFocus = true;
    public void OnInactive() => m_bActiveFocus = false;

    public void PlayMusic(string cMusic, float fFadeTime = 1.5f)
    {
        m_pMusicOldAmbient = m_pMusicAmbient;
        m_pMusicAmbient = GetSound(cMusic, false);
        m_pMusicAmbient.SetVolume(m_fMusicVolume);
        m_pMusicAmbient.Play();
        m_bDeleteMusic = false;
    }

    public void DeleteMusic(bool bJustDelete)
    {
        if (bJustDelete)
        {
            m_pMusicAmbient?.Stop();
            m_pMusicAmbient = null;
            m_bDeleteMusic = false;
            return;
        }

        m_bDeleteMusic = true;
        m_pMusicAmbient?.SetVolume(0.0f);
    }

    public void Set_BGM_Volume(float nVol)
    {
        m_fMusicVolume = Math.Clamp(nVol, 0.0f, 1.0f);
        m_pMusicAmbient?.SetVolume(m_fMusicVolume);
    }

    public void Set_BGM_FadeVolume(float nVol) => Set_BGM_Volume(nVol);
    public float GetMusicVolume() => m_fMusicVolume;
    public float GetSoundVolume() => m_fSoundVolume;
    public float GetSystemVolume() => m_fSystemVolume;

    public CSound PlaySound(string cSound)
    {
        CSound sound = GetSound(cSound, false);
        sound.SetVolume(m_fSoundVolume);
        sound.Play();
        _cloneSounds.Add(sound);
        return sound;
    }

    public CSound PlaySystemSound(string cSound)
    {
        CSound sound = GetSound(cSound, false);
        sound.SetVolume(m_fSystemVolume);
        sound.Play();
        _systemSounds.Add(sound);
        return sound;
    }

    public void StopSound(CSound? pSound)
    {
        pSound?.Stop();
    }

    public bool IsSound(CSound? pSound)
    {
        return pSound is not null && _sounds.ContainsKey(pSound.Path);
    }

    public void Set_EFM_Volume(float nVol) => m_fSoundVolume = Math.Clamp(nVol, 0.0f, 1.0f);
    public void Set_System_Volume(float nVol) => m_fSystemVolume = Math.Clamp(nVol, 0.0f, 1.0f);
    public void Play_Stop_Sound() => StopAll(_cloneSounds);
    public void Play_Stop_SystemSound() => StopAll(_systemSounds);

    public CSound GetSound(string cSound, bool b3DSound, float fMinDist = 300.0f, float fMaxDist = 4000.0f)
    {
        string key = $"{b3DSound}:{cSound}";
        if (_sounds.TryGetValue(key, out CSound? sound))
        {
            return sound;
        }

        sound = new CSound(cSound, b3DSound);
        _sounds[key] = sound;
        return sound;
    }

    public CSound GetOtherSound(string cSound, bool b3DSound, float fMinDist = 300.0f, float fMaxDist = 4000.0f) =>
        GetSound(cSound, b3DSound, fMinDist, fMaxDist);

    public CSound GetStaticSound(string cSound, bool b3DSound, float fMinDist = 300.0f, float fMaxDist = 4000.0f) =>
        GetSound(cSound, b3DSound, fMinDist, fMaxDist);

    private bool _InitMiles()
    {
        return _milesAudio.Init(_paths.SoundRoot);
    }

    private static void StopAll(List<CSound> sounds)
    {
        foreach (CSound sound in sounds)
        {
            sound.Stop();
        }

        sounds.Clear();
    }
}
