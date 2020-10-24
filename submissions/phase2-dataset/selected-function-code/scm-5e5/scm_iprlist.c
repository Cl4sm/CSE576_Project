     char *hdr, tlr;
     SCM exp;
     SCM port;
     int writing;
{
  lputs(hdr, port);
  /* CHECK_INTS; */
  scm_iprin1(CAR(exp), port, writing);
  exp = GCCDR(exp); /* CDR(exp); */
  for (;NIMP(exp);exp = GCCDR(exp) /* CDR(exp)*/) {
    if (!scm_cell_p(~1L & exp)) break;
    if (NECONSP(exp)) break;
    lputc(' ', port);
    /* CHECK_INTS; */
    scm_iprin1(CAR(exp), port, writing);
  }
  if (NNULLP(exp)) {
    lputs(" . ", port);
    scm_iprin1(exp, port, writing);
  }
  lputc(tlr, port);
}
