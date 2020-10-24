     SCM x;
{
  SCM ret = x;
  SCM *px = &ret;
 tail:
  x = *px;
  if (IMP(x)) return ret;
  if (CONSP(x)) {
    if (SCM_LINUMP(CAR(x))) {
      *px = CDR(x);
      px = &CDR(x);
      goto tail;
    }
    if (NIMP(CAR(x)))
      CAR(x) = scm_remove_linums(CAR(x));
    px = &CDR(x);
    goto tail;
  }
  else if (VECTORP(x)) {
    SCM *ve = VELTS(x);
    sizet i = LENGTH(x);
    while (i--) {
      if (NIMP(ve[i]))
	ve[i] = scm_remove_linums(ve[i]);
    }
    return ret;
  }
  else
    return ret;
}
