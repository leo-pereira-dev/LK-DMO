
/* ============================================================ */
/* Function: FUN_011ba9b0 */
/* Entry: 011ba9b0 */
/* Signature: undefined FUN_011ba9b0() */
/* ============================================================ */


void __fastcall FUN_011ba9b0(int *param_1)

{
  int iVar1;
  float fVar2;
  float fVar3;
  
  if ((int *)*param_1 != (int *)0x0) {
    (**(code **)(*(int *)*param_1 + 0x58))(s_DigiCombine_DigiCombine_CardFron_01ae106c,1);
  }
  if ((param_1[1] != 0) && (iVar1 = *(int *)(param_1[1] + 0x1c), iVar1 != 0)) {
    *(undefined4 *)(iVar1 + 0x80) = 2;
    *(undefined4 *)(iVar1 + 0x7c) = 0;
    *(undefined4 *)(iVar1 + 0x68) = 0x3c23d70a;
    *(undefined4 *)(iVar1 + 0x84) = 3;
    fVar3 = DAT_01b02568;
    fVar2 = DAT_01b02568;
    iVar1 = FUN_00f505d0();
    if (DAT_01b02504 < fVar2) {
      *(undefined4 *)(iVar1 + 0x70) = 0x3da3d70a;
      *(undefined4 *)(iVar1 + 0x74) = 0;
      *(float *)(iVar1 + 0x78) = (float)*(int *)(iVar1 + 0x20) / fVar3;
    }
    (**(code **)(*(int *)param_1[1] + 0x40))(1);
  }
  return;
}


