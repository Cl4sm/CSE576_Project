     SCM exp; SCM port; char *type;
{
  int filn = fileno(STREAM(exp));
  lputs("#<", port);
  if (CLOSEDP(exp)) lputs("closed-", port);
  else {
    if (RDNG & CAR(exp)) lputs("input-", port);
    if (WRTNG & CAR(exp)) lputs("output-", port);
  }
  lputs(type, port);
  lputc(' ', port);
#ifndef MSDOS
# ifndef __EMX__
#  ifndef _DCC
#   ifndef AMIGA
#    ifndef macintosh
#     ifndef PLAN9
  if (OPENP(exp) && tc16_fport==TYP16(exp) && filn >= 0 && isatty(filn)) {
    char *ttyn = ttyname(filn);
    if (ttyn) lputs(ttyn, port);
    else goto punt;
  }
  else
#     endif
#    endif
#   endif
#  endif
# endif
#endif
  punt:
    {
      SCM s = PORTP(exp) ? SCM_PORTDATA(exp) : UNDEFINED;
      if (NIMP(s) && STRINGP(s))
	scm_iprin1(s, port, 1);
      else if (OPFPORTP(exp))
	scm_intprint((long)filn, 10, port);
      else
	scm_intprint(CDR(exp), -16, port);
      if (TRACKED & SCM_PORTFLAGS(exp)) {
	lputs(" L", port);
	scm_intprint(scm_port_table[SCM_PORTNUM(exp)].line, 10, port);
	lputs(" C", port);
	scm_intprint(scm_port_table[SCM_PORTNUM(exp)].col+0L, 10, port);
      }
    }
  lputc('>', port);
}
