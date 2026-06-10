using DProject.Resources;
using GUI;

namespace DProject.Interface.Base;

public readonly record struct CsPoint(int X, int Y)
{
    public static readonly CsPoint ZERO = new(0, 0);
}

public readonly record struct CsRect(int Left, int Top, int Right, int Bottom)
{
    public bool PtInRect(CsPoint point)
    {
        return point.X >= Left && point.X < Right && point.Y >= Top && point.Y < Bottom;
    }

    public CsPoint GetPos() => new(Left, Top);
    public CsPoint GetSize() => new(Right - Left, Bottom - Top);
}

public static class ICONITEM
{
    public enum eTYPE
    {
        ARMOR_1 = 1,
        ARMOR_2 = 2,
        ELEMENT_NO_CHECK = 3,
        ELEMENT_CHECK = 4,
        CHIPSET = 5,
        CARD = 6,
        ITEM_CARD = 7,
        QUEST_ITEM = 8,
        DT_MEMORY = 9,
        ITEM_ICON = 10,
        SKILL1 = 11,
        SKILL2 = 12,
        SKILL3 = 13,
        SKILL_MASK = 14,
        SKILL4 = 15,
        SKILL5 = 16,
        SKILL6 = 17,
        DEvol = 21,
        DAttribute = 22,
        DGrowType = 23,
        MiniMap = 31,
        MonsterAI = 32,
        Stat = 33,
        Community = 34,
        CHAT_LV = 35,
        Enchant_Lv = 36,
        New_Taget_DAttribute = 37,
        ITEM_ICON47 = 47,
        ITEM_ICON48 = 48,
        ITEM_ICON49 = 49,
        Talk = 101,
        NUMBER = 400,
        ITEM_MASK = 500
    }

    public static eTYPE GetSkillIconType(int nIcon)
    {
        if (nIcon >= 6000)
        {
            return eTYPE.SKILL6;
        }

        if (nIcon >= 5000)
        {
            return eTYPE.SKILL5;
        }

        if (nIcon >= 4000)
        {
            return eTYPE.SKILL4;
        }

        if (nIcon >= 3000)
        {
            return eTYPE.SKILL3;
        }

        if (nIcon >= 2000)
        {
            return eTYPE.SKILL2;
        }

        return eTYPE.SKILL1;
    }
}

public static class ICON_Dynamic
{
    public enum eTYPE
    {
        Protect = 200,
        Achieve = 201,
        Achieve2 = 202,
        Achieve3 = 204,
        CashShop_Mask = 1000
    }
}

public static class ICONMASK
{
    public enum eMASK_TYPE
    {
        IF_ICON_MOUSE_DRAG,
        IF_ICON_MOUSE_ON,
        IF_ICON_MOUSE_CLICK,
        IF_ICON_SELECT,
        IF_ICON_RED,
        IF_CURSOR_ICON,
        IF_SOCKET,
        IF_ICON_REAL_RED,
        IF_ICON_NORMAL_RED,
        MAX_COUNT
    }
}

public static class NUMBERSPRITE
{
    public enum eCOLOR
    {
        RED,
        ORANGE,
        BLUE,
        GREEN,
        WHITE,
        YELLOW,
        MAX_COLOR
    }
}

public sealed class cSprite
{
    public Texture? Texture { get; private set; }
    public CsPoint Position { get; private set; }
    public CsPoint Size { get; private set; }
    public string FilePath { get; private set; } = string.Empty;
    public float Alpha { get; private set; } = 1.0f;

    public void Init(ResourceMng resourceMng, CsPoint pos, CsPoint size, string imagePath)
    {
        Position = pos;
        Size = size;
        FilePath = imagePath;
        Texture = resourceMng.GetTexture(imagePath);
    }

    public void SetAlpha(float alpha) => Alpha = Math.Clamp(alpha, 0.0f, 1.0f);
    public void Render(CsPoint pos) => Position = pos;
    public void Render(CsPoint pos, CsPoint size)
    {
        Position = pos;
        Size = size;
    }
}

public sealed class cIcon
{
    private readonly ResourceMng _resourceMng;

    public cIcon(ResourceMng resourceMng)
    {
        _resourceMng = resourceMng;
    }

    public cSprite? m_pSpr { get; private set; }
    public CsPoint m_ptPatchSize { get; private set; }
    public CsPoint m_ptIndexCount { get; private set; }
    public int LastRenderedIconIndex { get; private set; }

    public void Init(CsPoint patch, CsPoint size, string cPath)
    {
        m_ptPatchSize = patch;
        m_ptIndexCount = new CsPoint(
            patch.X == 0 ? 0 : size.X / patch.X,
            patch.Y == 0 ? 0 : size.Y / patch.Y);
        m_pSpr = new cSprite();
        m_pSpr.Init(_resourceMng, CsPoint.ZERO, size, cPath);
    }

    public void Delete()
    {
        if (m_pSpr?.Texture is not null)
        {
            _resourceMng.RemoveTexture(m_pSpr.Texture);
        }

        m_pSpr = null;
    }

    public void Render(int nIconIdx, CsPoint pos)
    {
        LastRenderedIconIndex = nIconIdx;
        m_pSpr?.Render(pos);
    }

    public void Render(int nIconIdx, CsPoint pos, CsPoint size)
    {
        LastRenderedIconIndex = nIconIdx;
        m_pSpr?.Render(pos, size);
    }

    public void Render(int nIconIdx, CsPoint pos, CsPoint size, float fAlpha)
    {
        m_pSpr?.SetAlpha(fAlpha);
        Render(nIconIdx, pos, size);
    }

    public cSprite? GetSprite() => m_pSpr;
    public CsPoint GetIndexCount() => m_ptIndexCount;
    public CsPoint GetPatchSize() => m_ptPatchSize;
}

public struct sIFIcon
{
    public enum eSTATE
    {
        NONE,
        MOUSE_DRAG,
        MOUSE_ON,
        MOUSE_LCLICK,
        MOUSE_RCLICK
    }

    public CsRect s_Rect;
    public eSTATE s_eState;

    public void Init(CsRect rc)
    {
        s_Rect = rc;
        s_eState = eSTATE.NONE;
    }

    public bool MouseOn(CsPoint ptLocal)
    {
        if (s_eState == eSTATE.MOUSE_DRAG)
        {
            return false;
        }

        s_eState = s_Rect.PtInRect(ptLocal) ? eSTATE.MOUSE_ON : eSTATE.NONE;
        return s_eState != eSTATE.NONE;
    }

    public bool MouseLBtnClick(CsPoint ptLocal)
    {
        if (s_eState == eSTATE.MOUSE_DRAG)
        {
            return true;
        }

        s_eState = s_Rect.PtInRect(ptLocal) ? eSTATE.MOUSE_LCLICK : eSTATE.NONE;
        return s_eState != eSTATE.NONE;
    }

    public bool MouseRBtnClick(CsPoint ptLocal)
    {
        if (s_eState == eSTATE.MOUSE_DRAG)
        {
            return true;
        }

        s_eState = s_Rect.PtInRect(ptLocal) ? eSTATE.MOUSE_RCLICK : eSTATE.NONE;
        return s_eState != eSTATE.NONE;
    }

    public CsPoint GetPos() => s_Rect.GetPos();
    public CsPoint GetSize() => s_Rect.GetSize();

    public void ReleaseState(bool bIncluseDrag = false)
    {
        if (s_eState == eSTATE.MOUSE_DRAG && !bIncluseDrag)
        {
            return;
        }

        s_eState = eSTATE.NONE;
    }
}

public sealed class cIconMng
{
    private readonly ResourceMng _resourceMng;
    private readonly Dictionary<int, cIcon> _mapIconItem = new();
    private readonly Dictionary<ICON_Dynamic.eTYPE, cIcon> _mapDynamicIcon = new();

    public cIconMng(ResourceMng resourceMng)
    {
        _resourceMng = resourceMng;
    }

    public IReadOnlyDictionary<int, cIcon> IconItems => _mapIconItem;
    public IReadOnlyDictionary<ICON_Dynamic.eTYPE, cIcon> DynamicIcons => _mapDynamicIcon;

    public void Init()
    {
        RegisterIcon(ICONITEM.eTYPE.ARMOR_1, "Icon\\Icon01.dds");
        RegisterIcon(ICONITEM.eTYPE.ARMOR_2, "Icon\\Icon02.dds");
        RegisterIcon(ICONITEM.eTYPE.ITEM_ICON, "Icon\\Icon10.dds", new CsPoint(32, 32), new CsPoint(1024, 1024));
        RegisterIcon(ICONITEM.eTYPE.SKILL1, "Icon\\sIcon01.dds");
        RegisterIcon(ICONITEM.eTYPE.SKILL2, "Icon\\sIcon02.dds");
        RegisterIcon(ICONITEM.eTYPE.SKILL3, "Icon\\sIcon03.dds");
        RegisterIcon(ICONITEM.eTYPE.SKILL4, "Icon\\sIcon04.dds");
        RegisterIcon(ICONITEM.eTYPE.SKILL5, "Icon\\sIcon05.dds");
        RegisterIcon(ICONITEM.eTYPE.SKILL6, "Icon\\sIcon06.dds");
    }

    public void Delete()
    {
        foreach (cIcon icon in _mapIconItem.Values)
        {
            icon.Delete();
        }

        foreach (cIcon icon in _mapDynamicIcon.Values)
        {
            icon.Delete();
        }

        _mapIconItem.Clear();
        _mapDynamicIcon.Clear();
    }

    public void ResetDevice()
    {
    }

    public void InitDynamicIcon(ICON_Dynamic.eTYPE eType, CsPoint patch, CsPoint size, string cPath)
    {
        DeleteDynamicIcon(eType);
        var icon = new cIcon(_resourceMng);
        icon.Init(patch, size, cPath);
        _mapDynamicIcon[eType] = icon;
    }

    public void DeleteDynamicIcon(ICON_Dynamic.eTYPE eType)
    {
        if (_mapDynamicIcon.Remove(eType, out cIcon? icon))
        {
            icon.Delete();
        }
    }

    public cSprite? GetSprite(ICON_Dynamic.eTYPE eType) => GetIcon(eType)?.GetSprite();
    public cIcon? GetIcon(ICON_Dynamic.eTYPE eType) =>
        _mapDynamicIcon.TryGetValue(eType, out cIcon? icon) ? icon : null;

    public cIcon? GetIcon(int nItemID)
    {
        int type = nItemID >= 1000 ? (int)ICONITEM.GetSkillIconType(nItemID) : (int)ICONITEM.eTYPE.ITEM_ICON;
        return _mapIconItem.TryGetValue(type, out cIcon? icon) ? icon : null;
    }

    public void SetItemAlpha(int nItemID, float fAlpha)
    {
        GetIcon(nItemID)?.GetSprite()?.SetAlpha(fAlpha);
    }

    public void RenderItem(int nItemID, CsPoint pos)
    {
        GetIcon(nItemID)?.Render(nItemID, pos);
    }

    public void RenderItem(int nItemID, CsPoint pos, CsPoint size)
    {
        GetIcon(nItemID)?.Render(nItemID, pos, size);
    }

    public void RenderItem(int nItemID, CsPoint pos, CsPoint size, float fAlpha)
    {
        GetIcon(nItemID)?.Render(nItemID, pos, size, fAlpha);
    }

    public void RenderIFIconMask(sIFIcon.eSTATE state, CsPoint pos, CsPoint size)
    {
    }

    private void RegisterIcon(ICONITEM.eTYPE type, string path)
    {
        RegisterIcon(type, path, new CsPoint(32, 32), new CsPoint(512, 512));
    }

    private void RegisterIcon(ICONITEM.eTYPE type, string path, CsPoint patch, CsPoint size)
    {
        var icon = new cIcon(_resourceMng);
        icon.Init(patch, size, path);
        _mapIconItem[(int)type] = icon;
    }
}
