
/* ============================================================ */
/* Function: FUN_0161e740 */
/* Entry: 0161e740 */
/* Signature: undefined FUN_0161e740() */
/* ============================================================ */


int * __thiscall FUN_0161e740(int param_1,byte *param_2,int *param_3)

{
  int *piVar1;
  int *piVar2;
  undefined1 local_10 [8];
  int local_8;
  
  FUN_015d4b80(local_10,param_2);
  if (((*(char *)(local_8 + 0xd) == '\0') && (*(byte *)(local_8 + 0x10) <= *param_2)) &&
     (local_8 != *(int *)(param_1 + 0x2c))) {
    piVar1 = *(int **)(local_8 + 0x14);
    piVar2 = (int *)*piVar1;
    if (piVar2 != piVar1) {
      do {
        if ((piVar2[3] <= *param_3) && (*param_3 <= piVar2[4])) {
          return piVar2 + 2;
        }
        piVar2 = (int *)*piVar2;
      } while (piVar2 != piVar1);
    }
  }
  return (int *)0x0;
}


