     SCM iloc;
{
  register int ir = IFRAME(iloc);
  register SCM er, *eloc;
#ifdef SCM_PROFILE
  ilookup_cases[ir<10 ? ir : 9]
    [IDIST(iloc)<10 ? IDIST(iloc) : 9][ICDRP(iloc)?1:0]++;
#endif
  DEFER_INTS_EGC;
  er = scm_env;
  /* shortcut the two most common cases. */
  if (iloc==MAKILOC(0, 0)) return &CAR(CAR(er));
  if (iloc==MAKILOC(0, 1)) return &CAR(CDR(CAR(er)));
  for (;0 != ir;--ir) er = CDR(er);
  eloc = &CAR(er);
  for (ir = IDIST(iloc); 0 != ir; --ir)
    eloc = &CDR(*eloc);
  if (ICDRP(iloc)) return eloc;
  return &CAR(*eloc);
}
