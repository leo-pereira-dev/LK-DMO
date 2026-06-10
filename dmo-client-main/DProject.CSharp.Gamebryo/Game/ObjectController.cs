using DProject.Resist;
using DProject.Runtime;

namespace DProject.Game;

public readonly record struct ShadowRenderCommand(uint Uid, uint ClassId, string Name);

public static class cObjectController
{
    public static int RenderShadow(CCharMng charMng, cResist resist, RenderCommandQueue? renderCommands = null)
    {
        int queued = 0;

        if (!resist.m_Global.s_bFigureTamer)
        {
            foreach (CTamer tamer in charMng.Tamers.Values)
            {
                if (RenderShadowObject(tamer, renderCommands))
                {
                    queued++;
                }
            }
        }
        else if (charMng.GetTamerUser() is CTamerUser tamerUser &&
            RenderShadowObject(tamerUser, renderCommands))
        {
            queued++;
        }

        if (!resist.m_Global.s_bFigureDigimon)
        {
            foreach (CDigimon digimon in charMng.Digimons.Values)
            {
                if (RenderShadowObject(digimon, renderCommands))
                {
                    queued++;
                }
            }
        }
        else if (charMng.GetDigimonUser(0) is CDigimonUser digimonUser &&
            RenderShadowObject(digimonUser, renderCommands))
        {
            queued++;
        }

        foreach (CMonster monster in charMng.Monsters.Values)
        {
            if (monster.IsMonsterState(CMonster.eMONSTER_STATE.MONSTER_CREATE))
            {
                continue;
            }

            if (RenderShadowObject(monster, renderCommands))
            {
                queued++;
            }
        }

        foreach (CNpc npc in charMng.Npcs.Values)
        {
            if (npc.IsEnableUse() && RenderShadowObject(npc, renderCommands))
            {
                queued++;
            }
        }

        if (!resist.m_Global.s_bFigureEmployment)
        {
            foreach (CEmployment employment in charMng.Employments.Values)
            {
                if (RenderShadowObject(employment, renderCommands))
                {
                    queued++;
                }
            }
        }

        return queued;
    }

    public static bool RenderShadowObject(CsC_AvObject? obj, RenderCommandQueue? renderCommands = null)
    {
        if (obj is null || !obj.IsPresent)
        {
            return false;
        }

        ShadowRenderCommand command = new(obj.GetUniqID(), obj.GetClass(), obj.GetName());
        renderCommands?.Enqueue(() =>
        {
            _ = command;
            // Placeholder until the Gamebryo shadow shader path is ported.
        });
        return true;
    }
}
