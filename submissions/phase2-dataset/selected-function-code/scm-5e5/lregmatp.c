SCM lregmatp(prog, str)
     SCM prog, str;
{
  FIXUP_REGEXP(prog);
  ASRTER(NIMP(prog) && tc16_rgx==CAR(prog), prog, ARG1, s_regmatp);
  ASRTER(NIMP(str) && STRINGP(str), str, ARG2, s_regmatp);

#ifdef _GNU_SOURCE
  return (lregsearch(prog, str, EOL)==BOOL_F)?BOOL_F:BOOL_T;
#else  /* not _GNU_SOURCE */
  {
    int flags = 0;		/* XXX - optional arg? */

    flags = regexec(RGX(prog), CHARS(str), 0, 0, flags);
    if (!flags) return BOOL_T;
    if (REG_NOMATCH!=flags) wta(MAKINUM(flags), s_error, s_regmatp);
    return BOOL_F;
  }
#endif
}
