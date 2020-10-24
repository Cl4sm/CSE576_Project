regcomp (preg, pattern, cflags)
    regex_t *preg;
    const char *pattern; 
    int cflags;
{
  reg_errcode_t ret;
  unsigned syntax
    = (cflags & REG_EXTENDED) ?
      RE_SYNTAX_POSIX_EXTENDED : RE_SYNTAX_POSIX_BASIC;

  /* regex_compile will allocate the space for the compiled pattern.  */
  preg->buffer = 0;
  preg->allocated = 0;
  
  /* Don't bother to use a fastmap when searching.  This simplifies the
     REG_NEWLINE case: if we used a fastmap, we'd have to put all the
     characters after newlines into the fastmap.  This way, we just try
     every character.  */
  preg->fastmap = 0;
  
  if (cflags & REG_ICASE)
    {
      unsigned i;
      
      preg->translate = (char *) malloc (CHAR_SET_SIZE);
      if (preg->translate == NULL)
        return (int) REG_ESPACE;

      /* Map uppercase characters to corresponding lowercase ones.  */
      for (i = 0; i < CHAR_SET_SIZE; i++)
        preg->translate[i] = ISUPPER (i) ? tolower (i) : i;
    }
  else
    preg->translate = NULL;

  /* If REG_NEWLINE is set, newlines are treated differently.  */
  if (cflags & REG_NEWLINE)
    { /* REG_NEWLINE implies neither . nor [^...] match newline.  */
      syntax &= ~RE_DOT_NEWLINE;
      syntax |= RE_HAT_LISTS_NOT_NEWLINE;
      /* It also changes the matching behavior.  */
      preg->newline_anchor = 1;
    }
  else
    preg->newline_anchor = 0;

  preg->no_sub = !!(cflags & REG_NOSUB);

  /* POSIX says a null character in the pattern terminates it, so we 
     can use strlen here in compiling the pattern.  */
  ret = regex_compile (pattern, (int) strlen(pattern), syntax, preg);
  
  /* POSIX doesn't distinguish between an unmatched open-group and an
     unmatched close-group: both are REG_EPAREN.  */
  if (ret == REG_ERPAREN) ret = REG_EPAREN;
  
  return (int) ret;
}
