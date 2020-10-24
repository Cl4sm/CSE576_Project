char *
ma_strcasestr (phaystack, pneedle)
     const char *phaystack;
     const char *pneedle;
{
  register const unsigned char *haystack, *needle;
  register chartype b, c;

  haystack = (const unsigned char *) phaystack;
  needle = (const unsigned char *) pneedle;

  b = TOLOWER (*needle);
  if (b != '\0')
    {
      haystack--;		/* possible ANSI violation */
      do
	{
	  c = *++haystack;
	  if (c == '\0')
	    goto ret0;
	}
      while (TOLOWER (c) != (int) b);

      c = TOLOWER (*++needle);
      if (c == '\0')
	goto foundneedle;
      ++needle;
      goto jin;

      for (;;)
        {
          register chartype a;
	  register const unsigned char *rhaystack, *rneedle;

	  do
	    {
	      a = *++haystack;
	      if (a == '\0')
		goto ret0;
	      if (TOLOWER (a) == (int) b)
		break;
	      a = *++haystack;
	      if (a == '\0')
		goto ret0;
shloop:
	      ;
	    }
          while (TOLOWER (a) != (int) b);

jin:	  a = *++haystack;
	  if (a == '\0')
	    goto ret0;

	  if (TOLOWER (a) != (int) c)
	    goto shloop;

	  rhaystack = haystack-- + 1;
	  rneedle = needle;
	  a = TOLOWER (*rneedle);

	  if (TOLOWER (*rhaystack) == (int) a)
	    do
	      {
		if (a == '\0')
		  goto foundneedle;
		++rhaystack;
		a = TOLOWER (*++needle);
		if (TOLOWER (*rhaystack) != (int) a)
		  break;
		if (a == '\0')
		  goto foundneedle;
		++rhaystack;
		a = TOLOWER (*++needle);
	      }
	    while (TOLOWER (*rhaystack) == (int) a);

	  needle = rneedle;	/* took the register-poor approach */

	  if (a == '\0')
	    break;
        }
    }
foundneedle:
  return (char*) haystack;
ret0:
  return 0;
}