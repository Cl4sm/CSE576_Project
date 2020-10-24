static int recprin1(exp, port, writing)
     SCM exp, port;
     int writing;
{
  SCM names, printer = RTD_PRINTER(REC_RTD(exp));
  SCM argv[3];
  sizet i;
  if (NIMP(printer)) {
    argv[0] = exp;
    argv[1] = port;
    argv[2] = writing ? BOOL_T : BOOL_F;
    /* A writing value of 2 means we are printing an error message.
       An error in a record printer at this time will result in a
       fatal recursive error. */
    if (2 != writing) {
      if (NFALSEP(scm_cvapply(printer, 3L, argv)))
	return 1;
    }
    else {
      lputs("\n; Ignoring record-printer: ", cur_errp);
    }
  }
  names = RTD_FIELDS(REC_RTD(exp));
  lputs("#s(", port);
  scm_iprin1(RTD_NAME(REC_RTD(exp)), port, 0);
  if (writing) {
    lputc(':', port);
    scm_intprint(((long)REC_RTD(exp))>>1, 16, port);
  }
  for (i = 1; i < NUMDIGS(exp); i++) {
    lputc(' ', port);
    scm_iprin1(CAR(names), port, 0);
    names = CDR(names);
    lputc(' ', port);
    scm_iprin1(VELTS(exp)[i], port, writing);
  }
  lputc(')', port);
/*
  lputs("#<record <", port);
  scm_iprin1(RTD_NAME(REC_RTD(exp)), port, 0);
  lputc('>', port);
  for (i = 1; i < NUMDIGS(exp); i++) {
      lputc(' ', port);
      scm_iprin1(VELTS(exp)[i], port, writing);
    }
  lputc('>', port);
*/
  return 1;
}
