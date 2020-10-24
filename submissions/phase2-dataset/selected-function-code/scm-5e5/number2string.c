     SCM x, radix;
{
  if (UNBNDP(radix)) radix=MAKINUM(10L);
  else ASRTER(INUMP(radix), radix, ARG2, s_number2string);
#ifdef FLOATS
  if (NINUMP(x)) {
    char num_buf[FLOBUFLEN];
# ifdef BIGDIG
    ASRTGO(NIMP(x), badx);
    if (BIGP(x)) return big2str(x, (unsigned int)INUM(radix));
#  ifndef RECKLESS
    if (!(INEXP(x)))
      badx: wta(x, (char *)ARG1, s_number2string);
#  endif
# else
    ASRTER(NIMP(x) && INEXP(x), x, ARG1, s_number2string);
# endif
    return makfromstr(num_buf, iflo2str(x, num_buf));
  }
#else
# ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_number2string);
    return big2str(x, (unsigned int)INUM(radix));
  }
# else
  ASRTER(INUMP(x), x, ARG1, s_number2string);
# endif
#endif
  {
    char num_buf[INTBUFLEN];
    return makfromstr(num_buf, iint2str(INUM(x), (int)INUM(radix), num_buf));
  }
}
