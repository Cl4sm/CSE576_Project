SCM lremainder(x, y)
     SCM x, y;
{
  register long z;
#ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_remainder);
    if (NINUMP(y)) {
      ASRTGO(NIMP(y) && BIGP(y), bady);
      return divbigbig(BDIGITS(x), NUMDIGS(x), BDIGITS(y), NUMDIGS(y),
		       BIGSIGN(x), 0);
    }
    if (!(z = INUM(y))) goto ov;
    return divbigint(x, z, BIGSIGN(x), 0);
  }
  if (NINUMP(y)) {
# ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
    bady: wta(y, (char *)ARG2, s_remainder);
# endif
    return x;
  }
#else
  ASRTER(INUMP(x), x, ARG1, s_remainder);
  ASRTER(INUMP(y), y, ARG2, s_remainder);
#endif
  if (!(z = INUM(y)))
  ov: wta(y, (char *)OVFLOW, s_remainder);
#if (__TURBOC__==1)
  if (z < 0) z = -z;
#endif
  z = INUM(x)%z;
#ifdef BADIVSGNS
  if (!z) ;
  else if (z < 0)
	  if (x < 0) ;
	  else z += INUM(y);
  else if (x < 0) z -= INUM(y);
#endif
  return MAKINUM(z);
}
