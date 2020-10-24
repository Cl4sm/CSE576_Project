     SCM x, y;
{
  register long yy, z;
#ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_modulo);
    if (NINUMP(y)) {
      ASRTGO(NIMP(y) && BIGP(y), bady);
      return divbigbig(BDIGITS(x), NUMDIGS(x), BDIGITS(y), NUMDIGS(y),
		       BIGSIGN(y), (BIGSIGN(x) ^ BIGSIGN(y)) ? 1 : 0);
    }
    if (!(z = INUM(y))) goto ov;
    return divbigint(x, z, z < 0, (BIGSIGN(x) ? (z > 0) : (z < 0)) ? 1 : 0);
  }
  if (NINUMP(y)) {
# ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
    bady: wta(y, (char *)ARG2, s_modulo);
# endif
    return (BIGSIGN(y) ? (INUM(x)>0) : (INUM(x)<0)) ? sum(x, y) : x;
  }
#else
  ASRTER(INUMP(x), x, ARG1, s_modulo);
  ASRTER(INUMP(y), y, ARG2, s_modulo);
#endif
  if (!(yy = INUM(y)))
  ov: wta(y, (char *)OVFLOW, s_modulo);
#if (__TURBOC__==1)
  z = INUM(x);
  z = ((yy<0) ? -z : z)%yy;
#else
  z = INUM(x)%yy;
#endif
  return MAKINUM(((yy<0) ? (z>0) : (z<0)) ? z+yy : z);
}
