
/* ============================================================ */
/* Function: FUN_011b9f70 */
/* Entry: 011b9f70 */
/* Signature: undefined FUN_011b9f70() */
/* ============================================================ */


/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void __fastcall FUN_011b9f70(int param_1)

{
  int iVar1;
  bool bVar2;
  char cVar3;
  int aiStack_c [2];
  uint local_4;
  
  local_4 = DAT_01bbb1c0 ^ (uint)aiStack_c;
  (**(code **)(**(int **)(param_1 + 0x94) + 0x1c))(0);
  if ((DAT_01bfb32c != (int *)0x0) &&
     (cVar3 = (**(code **)(*DAT_01bfb32c + 4))(s_System_DigiCombine_Gacha_Progres_01ae1b00),
     cVar3 != '\0')) {
    (**(code **)(*DAT_01bfb32c + 8))(s_System_DigiCombine_Gacha_Progres_01ae1b00);
  }
  aiStack_c[0] = **(int **)(param_1 + 0x7c);
  cVar3 = *(char *)(aiStack_c[0] + 0xd);
  while (cVar3 == '\0') {
    if (((*(int *)(aiStack_c[0] + 0x2c) != 0) &&
        (*(char *)(*(int *)(aiStack_c[0] + 0x2c) + 0x1c) != '\0')) &&
       (iVar1 = *(int *)(*(int *)(aiStack_c[0] + 0x18) + 0x1c), iVar1 != 0)) {
      if (*(int *)(iVar1 + 0x80) == 2) {
        bVar2 = 0 < *(int *)(iVar1 + 0x84);
      }
      else {
        bVar2 = false;
      }
      if (bVar2) {
        FUN_011ba870();
        iVar1 = *(int *)(param_1 + 0x78);
        *(int *)(iVar1 + 0x68) = *(int *)(iVar1 + 0x68) + 1;
        if (*(int *)(iVar1 + 0x74) <= *(int *)(iVar1 + 0x68)) {
          FUN_00ffcb30(&stack0xfffffff0,&DAT_01bfe74c);
        }
      }
    }
    std::
    _Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
    ::operator++((_Tree_unchecked_const_iterator<class_std::_Tree_val<struct_std::_Tree_simple_types<unsigned_int>_>,struct_std::_Iterator_base0>
                  *)aiStack_c);
    cVar3 = *(char *)(aiStack_c[0] + 0xd);
  }
  return;
}


