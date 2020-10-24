     SCM z;
{
#ifdef FLOATS
  if (NINUMP(z)) {
# ifdef BIGDIG
    ASRTGO(NIMP(z), badz);
    if (BIGP(z)) return BOOL_F;
#  ifndef RECKLESS
    if (!(INEXP(z)))
      badz: wta(z, (char *)ARG1, s_zerop);
#  endif
# else
    ASRTER(NIMP(z) && INEXP(z), z, ARG1, s_zerop);
# endif
    return (z==flo0) ? BOOL_T : BOOL_F;
  }
#else
# ifdef BIGDIG
  if (NINUMP(z)) {
    ASRTER(NIMP(z) && BIGP(z), z, ARG1, s_zerop);
    return BOOL_F;
  }
# else
  ASRTER(INUMP(z), z, ARG1, s_zerop);
# endif
#endif
  return (z==INUM0) ? BOOL_T: BOOL_F;
}
