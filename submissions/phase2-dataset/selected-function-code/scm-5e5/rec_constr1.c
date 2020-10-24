     SCM args;
{
  SCM cclo = CAR(args);
  SCM rec, inds = RCONSTR_INDICES(cclo);
  sizet i = INUM(RCONSTR_SIZE(cclo));
  args = CDR(args);
  DEFER_INTS;
  rec = must_malloc_cell((i+1L)*sizeof(SCM),
			 MAKE_NUMDIGS(i+1L, tc16_record), s_record);
  while (i--)
    VELTS(rec)[i+1] = UNSPECIFIED;
  REC_RTD(rec) = RCLO_RTD(cclo);
  ALLOW_INTS;
  for (i = 0; i < LENGTH(inds); i++, args = CDR(args)) {
#ifndef RECKLESS
    if (NULLP(args))
      wna: rec_error(UNDEFINED, WNA, s_rec_constr1, RCLO_RTD(cclo), -1);
#endif
    VELTS(rec)[ REC_IND_REF(inds, i) ] = CAR(args);
  }
  ASRTGO(NULLP(args), wna);
  return rec;
}
