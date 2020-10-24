SCM lregsearchmatch(prog, str, args, search, vector)
     SCM prog, str, args;
     int vector, search;
{
  int len = ilength(args);
  int start, size, nsub;
  SCM matches;
  ALLOCA_PROTECT;

  FIXUP_REGEXP(prog);
  ASRTER(NIMP(prog) && tc16_rgx==CAR(prog), prog, ARG1, s_regsearch);
  ASRTER(NIMP(str) && STRINGP(str), str, ARG2, s_regsearch);
  ASRTER(len<=2, args, WNA, s_regsearch);
  ASRTER((len<1)||(INUMP(CAR(args))), CAR(args), ARG3, s_regsearch);
  ASRTER((len<2)||(INUMP(CAR(CDR(args)))), CAR(CDR(args)), ARG4, s_regsearch);

  start = (len>=1)?(INUM(CAR(args))):0;
  size  = (len>=2)?(INUM(CAR(CDR(args)))):LENGTH(str);

#ifdef _GNU_SOURCE
  {
    int ret, dir=1;
    struct re_registers regs, *pregs=0;

    if (search && start<0)
      start *= -1, dir = -1;

    if (vector) {
      pregs = &regs;
      nsub = RGX(prog)->re_nsub + 1;
      regs.num_regs = nsub;

      regs.start	  = ALLOCA(nsub * sizeof(regoff_t));
      regs.end	  = ALLOCA(nsub * sizeof(regoff_t));
    }

    if (search)
      ret = re_search(RGX(prog), CHARS(str), size, start, dir*size, pregs);
    else
      ret = re_match(RGX(prog), CHARS(str), size, start, pregs);

    if (ret < 0)
      return BOOL_F;

    if (!vector)
      return MAKINUM(ret);

    matches = make_vector(MAKINUM(2L * nsub), MAKINUM(-1L));
    while (nsub--) {
      VELTS(matches)[2*nsub+0] = MAKINUM(regs.start[nsub]);
      VELTS(matches)[2*nsub+1] = MAKINUM(regs.end[nsub]);
    }
    return matches;
  }
#else  /* not _GNU_SOURCE */
  {
    regex_t *regexp;
    regmatch_t *pm;
    char *search_string;
    if (size > LENGTH(str))
      size = LENGTH(str);

    if (start<0 || start >= size)
      return BOOL_F;

    if (size < LENGTH(str)) {
      search_string = ALLOCA(size-start+1);
      bcopy(CHARS(str)+start, search_string, size-start);
      search_string[size-start] = 0;
    } else
      search_string = CHARS(str)+start;

    nsub = RGX(prog)->re_nsub + 1;
    pm = ALLOCA(nsub * sizeof(regmatch_t));
    if (search)
      regexp = RGX(prog);
    else {
      /* doing a match */
      if (RGX_INFO(prog)->options) {
	/* strlen & strcpy OK, posix patterns are null terminated */
	char *pattern;

	pattern = ALLOCA(strlen(CHARS(RGX_PATTERN(prog)))+2);
	pattern[0] = '^';
	strcpy(pattern+1, CHARS(RGX_PATTERN(prog)));
	regcomp(RGX2(prog), pattern, RGX_INFO(prog)->options);
	RGX_INFO(prog)->options = 0;
      }
      regexp = RGX2(prog);
    }

    if (regexec(regexp, search_string, nsub, pm, 0) != 0)
      return BOOL_F;

    if (vector) {
      matches = make_vector(MAKINUM(2L * nsub), MAKINUM(-1L));
      while (nsub--) {
	VELTS(matches)[2*nsub+0] = MAKINUM(pm[nsub].rm_so + start);
	VELTS(matches)[2*nsub+1] = MAKINUM(pm[nsub].rm_eo + start);
      }
      return matches;
    }

    if (search)
      return MAKINUM(pm[0].rm_so + start);
    else
      return MAKINUM(pm[0].rm_eo - pm[0].rm_so);
  }

#endif /* _GNU_SOURCE */
}
