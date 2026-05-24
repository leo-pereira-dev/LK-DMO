#include "../include/DigiCombineTypes.h"

namespace DigiCombine
{
	// Reverse map from unpacked_exe_all/functions/01630020_FUN_01630020.c.
	// Original role: registers the four DM_DigiCombine bins and delegates each
	// payload to its table-specific parser callback.
	void LoadDigiCombineTablesFromClientBinPack()
	{
		// DM_DigiCombine-ItemList.bin  -> ParseDigiCombineItemList
		// DM_DigiCombine-RankList.bin  -> ParseDigiCombineRankList
		// DM_DigiCombine-ItemGroup.bin -> ParseDigiCombineRewardGroups(normal)
		// DM_DigiCombine-CeilGroup.bin -> ParseDigiCombineRewardGroups(ceiling)
	}

	// Reverse map from unpacked_exe_all/functions/0164d930_FUN_0164d930.c.
	// Same flow as DigiCombine, but using the UnionCombine table namespace.
	void LoadUnionCombineTablesFromClientBinPack()
	{
		// DM_UnionCombine-ItemList.bin  -> ParseUnionCombineItemList
		// DM_UnionCombine-RankList.bin  -> ParseUnionCombineRankList
		// DM_UnionCombine-ItemGroup.bin -> ParseUnionCombineRewardGroups(normal)
		// DM_UnionCombine-CeilGroup.bin -> ParseUnionCombineRewardGroups(ceiling)
	}
}
