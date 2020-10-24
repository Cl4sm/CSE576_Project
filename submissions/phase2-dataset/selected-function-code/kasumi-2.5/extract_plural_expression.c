void
internal_function
EXTRACT_PLURAL_EXPRESSION (nullentry, pluralp, npluralsp)
     const char *nullentry;
     struct expression **pluralp;
     unsigned long int *npluralsp;
{
  if (nullentry != NULL)
    {
      const char *plural;
      const char *nplurals;

      plural = strstr (nullentry, "plural=");
      nplurals = strstr (nullentry, "nplurals=");
      if (plural == NULL || nplurals == NULL)
	goto no_plural;
      else
	{
	  char *endp;
	  unsigned long int n;
	  struct parse_args args;

	  /* First get the number.  */
	  nplurals += 9;
	  while (*nplurals != '\0' && isspace ((unsigned char) *nplurals))
	    ++nplurals;
	  if (!(*nplurals >= '0' && *nplurals <= '9'))
	    goto no_plural;
#if defined HAVE_STRTOUL || defined _LIBC
	  n = strtoul (nplurals, &endp, 10);
#else
	  for (endp = nplurals, n = 0; *endp >= '0' && *endp <= '9'; endp++)
	    n = n * 10 + (*endp - '0');
#endif
	  if (nplurals == endp)
	    goto no_plural;
	  *npluralsp = n;

	  /* Due to the restrictions bison imposes onto the interface of the
	     scanner function we have to put the input string and the result
	     passed up from the parser into the same structure which address
	     is passed down to the parser.  */
	  plural += 7;
	  args.cp = plural;
	  if (PLURAL_PARSE (&args) != 0)
	    goto no_plural;
	  *pluralp = args.res;
	}
    }
  else
    {
      /* By default we are using the Germanic form: singular form only
         for `one', the plural form otherwise.  Yes, this is also what
         English is using since English is a Germanic language.  */
    no_plural:
      INIT_GERMANIC_PLURAL ();
      *pluralp = &GERMANIC_PLURAL;
      *npluralsp = 2;
    }
}
