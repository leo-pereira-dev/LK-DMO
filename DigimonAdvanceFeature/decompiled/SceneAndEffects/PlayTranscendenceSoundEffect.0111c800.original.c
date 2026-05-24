
/* ============================================================ */
/* Function: FUN_0111c800 */
/* Entry: 0111c800 */
/* Signature: undefined FUN_0111c800() */
/* ============================================================ */


void __thiscall FUN_0111c800(int param_1,undefined4 *param_2)

{
  char cVar1;
  int iVar2;
  
  if (*(int *)(param_1 + 0xb0) != 0) {
    iVar2 = __RTDynamicCast(*(undefined4 *)(*(int *)(param_1 + 0xb0) + 0x34),0,&DAT_01bc4a94,
                            &DAT_01bc50f0,0);
    if (iVar2 != 0) {
      *(undefined4 *)(iVar2 + 4) = *param_2;
      *(undefined4 *)(*(int *)(param_1 + 0xb0) + 0x74) = 1;
      iVar2 = *(int *)(param_1 + 0xb0);
      *(undefined1 *)(iVar2 + 0x78) = 1;
      *(undefined1 *)(iVar2 + 0x2d) = 1;
      *(undefined1 *)(iVar2 + 0x88) = 1;
      *(undefined4 *)(iVar2 + 0x6c) = 0;
      *(undefined4 *)(iVar2 + 0x68) = *(undefined4 *)(iVar2 + 100);
      if (DAT_01bfb32c != (int *)0x0) {
        cVar1 = (**(code **)(*DAT_01bfb32c + 4))(s_System_transcendental_wav_01ad8f48);
        if (cVar1 != '\0') {
          (**(code **)(*DAT_01bfb32c + 8))(s_System_transcendental_wav_01ad8f48);
        }
        (**(code **)*DAT_01bfb32c)(s_System_transcendental_wav_01ad8f48);
      }
      if (*(int **)(param_1 + 0x94) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x94) + 0x18))(0);
      }
      if (*(int **)(param_1 + 0x8c) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x8c) + 0x18))(0);
      }
      if (*(int **)(param_1 + 0x90) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x90) + 0x18))(0);
      }
      if (*(int **)(param_1 + 0x7c) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x7c) + 0x18))(0);
      }
      if (*(int **)(param_1 + 0x80) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x80) + 0x18))(0);
      }
      if (*(int **)(param_1 + 0x84) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x84) + 0x18))(0);
      }
      if (*(int **)(param_1 + 0x88) != (int *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0111c91b. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (**(code **)(**(int **)(param_1 + 0x88) + 0x18))();
        return;
      }
    }
  }
  return;
}


