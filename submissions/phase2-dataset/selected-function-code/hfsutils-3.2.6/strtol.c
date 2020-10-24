{
  long result = 0;
  int negative = 0;

  /* this replacement function optimized for use with hfsutils */

  if (endptr || base)
    abort();

  while (isspace(*nptr))
    ++nptr;

  switch (*nptr)
    {
    case '-':
      negative = 1;

    case '+':
      ++nptr;
    }

  while (1)
    {
      unsigned digit;

      digit = *nptr++ - '0';
      if (digit > 9)
	break;

      result = 10 * result - digit;
    }

  return negative ? result : -result;
}
