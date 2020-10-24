SCM equal(x, y)
     SCM x, y;
{
  CHECK_STACK;
 tailrecurse: POLL;
  if (x==y) return BOOL_T;
  if (IMP(x)) return BOOL_F;
  if (IMP(y)) return BOOL_F;
  if (CONSP(x) && CONSP(y)) {
    if (FALSEP(equal(CAR(x), CAR(y)))) return BOOL_F;
    x = CDR(x);
    y = CDR(y);
    goto tailrecurse;
  }
  /* this ensures that types and length are the same. */
  if (CAR(x) != CAR(y)) return BOOL_F;
  switch (TYP7(x)) {
  default: return BOOL_F;
  case tc7_string: return st_equal(x, y);
  case tc7_vector: return vector_equal(x, y);
  case tc7_smob: {
    int i = SMOBNUM(x);
    if (!(i < numsmob)) return BOOL_F;
    if (smobs[i].equalp) return (smobs[i].equalp)(x, y);
    else return BOOL_F;
  }
  case tc7_Vbool: case tc7_VfixN8: case tc7_VfixZ8:
  case tc7_VfixN16: case tc7_VfixZ16: case tc7_VfixN32: case tc7_VfixZ32:
  case tc7_VfloR32: case tc7_VfloC32: case tc7_VfloC64: case tc7_VfloR64: {
    SCM (*pred)() = smobs[0x0ff & (tc16_array>>8)].equalp;
    if (pred) return (*pred)(x, y);
    else return BOOL_F;
  }
  }
}
