SCM scm_copybitfield(to, from, rest)
     SCM to, from, rest;
{
  long len;
  SCM start, end;
#ifndef RECKLESS
  if (!(NIMP(rest) && CONSP(rest)))
    wna: wta(UNDEFINED, (char *)WNA, s_copybitfield);
#endif
  start = CAR(rest);
  rest = CDR(rest);
  ASRTGO(NIMP(rest) && CONSP(rest), wna);
  end = CAR(rest);
  ASRTGO(NULLP(CDR(rest)), wna);
  ASRTER(INUMP(start) && INUM(start)>=0, start, ARG2, s_copybitfield);
  len = INUM(end) - INUM(start);
  ASRTER(INUMP(end), end, ARG3, s_copybitfield);
  ASRTER(len >= 0, MAKINUM(len), OUTOFRANGE, s_copybitfield);
#ifdef BIGDIG
  if (NINUMP(from) || NINUMP(to) || (INUM(end) >= LONG_BIT - 2)) {
    SCM mask = difference(scm_ash(MAKINUM(1L), MAKINUM(len)), MAKINUM(1L));
    mask = scm_ash(mask, start);
    return scm_logior(scm_logand(mask, scm_ash(from, start)),
		      scm_logand(scm_lognot(mask), to));
  }
#else
  ASRTER(INUMP(to), to, ARG1, s_copybitfield);
  ASRTER(INUMP(from), from, ARG4, s_copybitfield);
  ASRTER(INUM(end) < LONG_BIT - 2, end, OUTOFRANGE, s_copybitfield);
#endif
  {
    long mask = ((1L<<len) - 1)<<INUM(start);
    return MAKINUM((mask & (INUM(from)<<INUM(start))) | ((~mask) & INUM(to)));
  }
}
