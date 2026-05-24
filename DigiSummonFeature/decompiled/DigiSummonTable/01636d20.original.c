
/* ============================================================ */
/* Function: FUN_01636d20 */
/* Entry: 01636d20 */
/* Signature: undefined FUN_01636d20() */
/* ============================================================ */


ulonglong __thiscall FUN_01636d20(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  uint in_EAX;
  int iVar3;
  int *piVar4;
  int local_8;
  
  if (param_2 == 0) {
    return (ulonglong)in_EAX & 0xffffffffffffff00;
  }
  cVar1 = *(char *)(**(int **)(param_1 + 0x14) + 0xd);
  iVar2 = **(int **)(param_1 + 0x14);
  while (cVar1 == '\0') {
    local_8 = iVar2;
    iVar3 = FUN_01677590(iVar2 + 0x10);
    if (iVar3 != 0) {
      if (*(int *)(iVar3 + 0x14) != 0) {
        piVar4 = (int *)(iVar3 + 4);
        if ((int *)(iVar2 + 0x44) != piVar4) {
          if (7 < *(uint *)(iVar3 + 0x18)) {
            piVar4 = (int *)*piVar4;
          }
          FUN_00f1a040(piVar4,*(undefined4 *)(iVar3 + 0x14));
        }
        FUN_015f09e0();
      }
      if (*(int *)(iVar3 + 0x2c) != 0) {
        piVar4 = (int *)(iVar3 + 0x1c);
        if ((int *)(iVar2 + 0x5c) != piVar4) {
          if (7 < *(uint *)(iVar3 + 0x30)) {
            piVar4 = (int *)*piVar4;
          }
          FUN_00f1a040(piVar4,*(undefined4 *)(iVar3 + 0x2c));
        }
        FUN_015f09e0();
      }
    }
    std::
    _Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
    ::operator++((_Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
                  *)&local_8);
    iVar2 = local_8;
    cVar1 = *(char *)(local_8 + 0xd);
  }
  return CONCAT44(param_2,CONCAT31((int3)((uint)iVar2 >> 8),1));
}


