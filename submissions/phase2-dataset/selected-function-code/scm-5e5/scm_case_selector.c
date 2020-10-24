     SCM x;
{
  SCM key, keys, *kv, *av;
  SCM actions, offset;
  long i, n;
  int op = ISYMVAL(CAR(x));
  x = CDR(x);
  key = EVALCAR(x);
  x = CDR(x);
  switch (op) {
  default: wta(MAKINUM(op), "internal error", s_case);
  case 0:			/* linear search */
    keys = CAR(x);
    kv = VELTS(keys);
    av = VELTS(CAR(CDR(x)));
    n = LENGTH(keys);
    for (i = n - 1; i > 0; i--)
      if (key == kv[i]) return av[i];
#ifndef INUMS_ONLY
    /* Bignum and flonum keys are pessimized. */
    if (NIMP(key) && NUMP(key))
      for (i = n - 1; i > 0; i--)
	if (NFALSEP(eqv(kv[i], key))) return av[i];
#endif
    return av[0];
  case 1:			/* integer jump table */
    offset = CAR(x);
    if (INUMP(key))
      i = INUM(key) - INUM(offset) + 1;
    else
      i = 0;
  jump:
    actions = CAR(CDR(x));
    if (i >= 1 && i < LENGTH(actions))
      return VELTS(actions)[i];
    else
      return VELTS(actions)[0];
  case 2:			/* character jump table */
    offset = CAR(x);
    if (ICHRP(key))
      i = ICHR(key) - ICHR(offset) + 1;
    else
      i = 0;
    goto jump;
  }
}
