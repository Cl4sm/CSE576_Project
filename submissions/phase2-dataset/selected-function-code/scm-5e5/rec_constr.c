SCM rec_constr(rtd, flds)
     SCM rtd, flds;
{
  SCM flst, fld;
  SCM cclo = makcclo(f_rec_constr1, 4L);
  SCM indices;
  sizet i, j;
  ASRTER(NIMP(rtd) && RTDP(rtd), rtd, ARG1, s_rec_constr);
  RCLO_RTD(cclo) = rtd;
  i = ilength(RTD_FIELDS(rtd));
  RCONSTR_SIZE(cclo) = MAKINUM(i);
  if (UNBNDP(flds)) {
    indices = MAKE_REC_INDS(i);
    while (i--) REC_IND_SET(indices, i, i+1);
  }
  else {
    i = ilength(flds);
    ASRTER(i>=0, flds, ARG2, s_rec_constr);
    indices = MAKE_REC_INDS(i);
    for (i = 0; NIMP(flds); i++, flds = CDR(flds)) {
      fld = CAR(flds);
      ASRTER(NIMP(fld) && SYMBOLP(fld), fld, ARG2, s_rec_constr);
      flst = RTD_FIELDS(rtd);
      for (j = 0; ; j++, flst = CDR(flst)) {
	ASRTER(NNULLP(flst), fld, ARG2, s_rec_constr);
	if (fld==CAR(flst)) {
	  REC_IND_SET(indices, i, j+1);
	  break;
	}
      }
    }
  }
  RCONSTR_INDICES(cclo) = indices;
  return cclo;
}
