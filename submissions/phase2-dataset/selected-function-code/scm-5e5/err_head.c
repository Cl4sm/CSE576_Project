     char *str;
{
  SCM lps;
  int oerrno = errno;
  exitval = MAKINUM(EXIT_FAILURE);
  if (NIMP(cur_outp) && OPOUTPORTP(cur_outp)) lfflush(cur_outp);
  lps = IMP(loadports) ? loadports : CDR(loadports);
  if (NIMP(lps)) {
    lputs("\n;In file loaded from ", cur_errp);
    for (; NIMP(lps); lps = CDR(lps)) {
      scm_iprin1(scm_port_filename(CAR(lps)), cur_errp, 0);
      lputs(":", cur_errp);
      scm_iprin1(scm_port_line(CAR(lps)), cur_errp, 1);
      lputs(IMP(CDR(lps)) ? ":" : ",\n;       loaded from ", cur_errp);
    }
  }
  lputs("\n;", cur_errp);
  lfflush(cur_errp);
  errno = oerrno;
  /* if (NIMP(cur_errp) && stderr==STREAM(cur_errp)) { ... } */
  if (errno>0) perror(str);
  fflush(stderr);
}
