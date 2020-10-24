     FILE *inport;
{
  if (isatty(fileno(inport))) {
#ifndef NOSETBUF
# ifndef _DCC
#  ifndef ultrix
#   ifndef __WATCOMC__
#    ifndef macintosh
#     if (__TURBOC__ != 1)
#      ifndef _Windows
    setbuf(inport, 0L);		/* Often setbuf isn't actually required */
#      endif
#     endif
#    endif
#   endif
#  endif
# endif
#endif
    return !0;			/* stdin gets marked BUF0 in init_scm() */
  }
  return 0;
}
