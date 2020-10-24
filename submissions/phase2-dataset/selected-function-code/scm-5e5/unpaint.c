     SCM *p;
{
  SCM x;
  while NIMP((x = *p)) {
    if (CONSP(x)) {
      if (NIMP(CAR(x)))
	unpaint(&CAR(x));
      else if (SCM_LINUMP(CAR(x))) {
	*p = CDR(x);
	continue;
      }
      p = &CDR(*p);
    }
    else if (VECTORP(x)) {
      sizet i = LENGTH(x);
      if (0==i) return;
      while (i-- > 1) unpaint(&(VELTS(x)[i]));
      p = VELTS(x);
    }
    else {
#ifdef MACRO
      while M_IDENTP(x) *p = x = IDENT_PARENT(x);
#endif
      return;
    }
  }
}
