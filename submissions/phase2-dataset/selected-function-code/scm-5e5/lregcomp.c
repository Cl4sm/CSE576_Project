     SCM pattern, flags;
{
  SCM z;
  int i, options;
  regex_t *prog;
  regex_info *info;
  char *flagchars;
#ifdef _GNU_SOURCE
  int fastmap = 0;
  int ignore_case = 0;
  char *err_msg;
#endif

  ASRTER(NIMP(pattern) && STRINGP(pattern), pattern, ARG1, s_regcomp);
  ASRTER(UNBNDP(flags) || (NIMP(flags) && STRINGP(flags)),
	 flags, ARG2, s_regcomp);
  DEFER_INTS;
  z = must_malloc_cell((long)sizeof(regex_info), (SCM)tc16_rgx, s_regex);
  scm_protect_temp(&z);
  info=(regex_info*)CHARS(z);
  prog = &(info->rgx);
#ifdef __REGEXP_LIBRARY_H__
  for (i=sizeof(regex_t);i--;((char *)prog)[i] = 0);
# ifndef _GNU_SOURCE
  {
    regex_t *prog2;
    prog2 = &(info->rgx_anchored);
    for (i=sizeof(regex_t);i--;((char *)prog2)[i] = 0);
  }
# endif
#endif

  ALLOW_INTS;
  info->pattern = pattern;

#ifdef _GNU_SOURCE
  options = RE_SYNTAX_POSIX_EXTENDED;
#else
  options = REG_EXTENDED;
#endif

  if (!UNBNDP(flags)) {
    flagchars = CHARS(flags);
    for (i=0; i<LENGTH(flags); i++)
      switch (flagchars[i]) {
#ifdef _GNU_SOURCE
      case 'n':
	options |= RE_HAT_LISTS_NOT_NEWLINE;
	options &= (~RE_DOT_NEWLINE);
	break;
      case 'i':
	ignore_case = 1;
	break;
      case '0':
	options &= (~RE_DOT_NOT_NULL);
	break;
      case 'f':
	fastmap = 1;
	break;
#else
      case 'n':
	options |= REG_NEWLINE;
	break;
      case 'i':
	options |= REG_ICASE;
	break;
#endif
      }
  }

#ifdef _GNU_SOURCE
  DEFER_INTS;
  if (fastmap)
    prog->fastmap = must_malloc(CHAR_SET_SIZE, s_regex);
  else
    prog->fastmap = 0;

  if (ignore_case) {
    prog->translate = must_malloc(CHAR_SET_SIZE, s_regex);
    for (i = 0; i < CHAR_SET_SIZE; i++)
      prog->translate[i] = ISUPPER (i) ? tolower (i) : i;
  }
  else
    prog->translate = 0;

  prog->buffer = 0;
  prog->allocated = 0;

  re_set_syntax(options);
  err_msg = (char *)re_compile_pattern(CHARS(pattern), LENGTH(pattern), prog);
  ALLOW_INTS;
  prog->regs_allocated = REGS_FIXED;

  /* if error, compile using regcomp to get the error number */
  if (err_msg) {
    int i;
    char *tmppat;
    SCM protect;

    /* Fixup in case pattern has null characters */
    tmppat = CHARS(protect=makstr(LENGTH(pattern)));
    bcopy(CHARS(pattern), tmppat, LENGTH(pattern));
    for (i=0; i<LENGTH(pattern); i++)
      if (tmppat[i] == 0)
	tmppat[i] = ' ';

    i = regcomp(prog, tmppat, options);
    z = MAKINUM(i);
  }
#else
  info->options = options;
  i = regcomp(prog, CHARS(pattern), options);
  if (i) z = MAKINUM(i);
#endif
  return z;
}
