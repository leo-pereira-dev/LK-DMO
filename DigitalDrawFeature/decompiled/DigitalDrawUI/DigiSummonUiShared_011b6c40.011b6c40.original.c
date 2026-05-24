
/* ============================================================ */
/* Function: FUN_011b6c40 */
/* Entry: 011b6c40 */
/* Signature: undefined FUN_011b6c40() */
/* ============================================================ */


void FUN_011b6c40(undefined4 param_1,int *param_2)

{
  char cVar1;
  int *piVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  cVar1 = *(char *)((int)param_2 + 0xd);
  while (cVar1 == '\0') {
    FUN_011b6c40(param_1,param_2[2]);
    piVar2 = (int *)*param_2;
    puVar3 = (undefined4 *)param_2[0xc];
    *(undefined4 *)puVar3[1] = 0;
    puVar3 = (undefined4 *)*puVar3;
    while (puVar3 != (undefined4 *)0x0) {
      puVar4 = (undefined4 *)*puVar3;
      FUN_017986fc(puVar3,0xc);
      puVar3 = puVar4;
    }
    FUN_017986fc(param_2[0xc],0xc);
    FUN_017986fc(param_2,0x3c);
    param_2 = piVar2;
    cVar1 = *(char *)((int)piVar2 + 0xd);
  }
  return;
}


