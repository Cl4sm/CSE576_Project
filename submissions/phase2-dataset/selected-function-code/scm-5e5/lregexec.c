SCM lregexec(prog, str)
     SCM prog, str;
{
  ALLOCA_PROTECT;

  FIXUP_REGEXP(prog);
  ASRTER(NIMP(prog) && tc16_rgx==CAR(prog), prog, ARG1, s_regexec);
  ASRTER(NIMP(str) && STRINGP(str), str, ARG2, s_regexec);

#ifdef _GNU_SOURCE
  return lregsearchv(prog, str, EOL);
#else  /* not _GNU_SOURCE */
  {
    size_t nsub;
    SCM ans;
    regmatch_t *pm;
    int flags = 0;		/* XXX - optional arg? */

    nsub = RGX(prog)->re_nsub + 1; /* XXX - is this posix? */
    pm = ALLOCA(nsub * sizeof(regmatch_t));
    if (regexec(RGX(prog), CHARS(str), nsub, pm, flags) != 0)
      ans = BOOL_F;
    else {
      ans = make_vector(MAKINUM(2L * nsub), MAKINUM(-1L));
      while (nsub--) {
	VELTS(ans)[2*nsub+0] = MAKINUM(pm[nsub].rm_so);
	VELTS(ans)[2*nsub+1] = MAKINUM(pm[nsub].rm_eo);
      }
    }
    return ans;
  }
#endif /* _GNU_SOURCE */
}
