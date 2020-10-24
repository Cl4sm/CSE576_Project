regex_match(regex, position, replace_str, replace_len, match_len)
  long *regex;        /* pointer to the regex-code to be processed. */
  long position;      /* position in the input stream. */
  char **replace_str;
  long *replace_len;  /* if a match is found at `position',
                       * the replace-string for the match is written to
		       * `*replace_str'/`replace_len'.  the memory for
		       * `*replace_str' is allocated by `regex_match()'
		       * via `malloc()'. */
  long *match_len;    /* if a match is found at `position',
                       * the length of the match is written to
                       * `*match_len'. */
{
  long *pp = regex + *regex;
  int i;
  int zero = 0;

  if (regex_match_(regex + 1, position, 0, &zero)) {
    *match_len = rx_match_skip - position;
    if (!*pp) {
      *replace_str = (char *)malloc(1);
      **replace_str = 0;
      *replace_len = 0;
    } else {
      char *cp;
      long *pp_rec = pp;
      /* store the match to slot 0. */
      regex_store(0, position, rx_match_skip);
      /* check out how long the replace-string will be. */
      for (*replace_len = 0;;) {
	if (*pp == (long)EOX) {
	  ++pp;
	  break;
	}
	switch ((enum regex_e)*pp++) {
	  case STRING:
	    *replace_len += *pp;
	    pp += *pp + 1;
	    break;
	  case CHAR:
	    ++*replace_len;
	    ++pp;
	    break;
	  case BACKREF:
	    *replace_len += regex_ref_len(*pp++);
	    break;
	  default:
	    abort();
	}
      }
      assert(!*pp);
      cp = *replace_str =
        (char *)malloc(*replace_len + !*replace_len);
      for (pp = pp_rec;;) {
	long reflen;
	if (*pp == (long)EOX) break;
	switch ((enum regex_e)*pp++) {
	  case STRING:
            for (i = 0; i < *pp; ++i) cp[i] = pp[i + 1];
	    cp += *pp;
	    pp += *pp + 1;
	    break;
	  case CHAR:
	    *cp++ = *pp++;
	    break;
	  case BACKREF:
	    memcpy(cp, regex_ref(*pp), reflen = regex_ref_len(*pp));
	    cp += reflen;
	    ++pp;
	    break;
	  default:
	    abort();
	}
      }
    }
    return 1;
  }
  return 0;
}
