SCM istr2int(str, len, radix)
     register char *str;
     long len;
     register long radix;
{
  register long n = 0, ln;
  register int c;
  register int i = 0;
  int lead_neg = 0;
  if (0 >= len) return BOOL_F;	/* zero length */
  switch (*str) {		/* leading sign */
  case '-': lead_neg = 1;
  case '+': if (++i==len) return BOOL_F; /* bad if lone `+' or `-' */
  }

  do {
    switch (c = str[i++]) {
    case DIGITS:
      c = c - '0';
      goto accumulate;
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
      c = c-'A'+10;
      goto accumulate;
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
      c = c-'a'+10;
    accumulate:
      if (c >= radix) return BOOL_F; /* bad digit for radix */
      ln = n;
      n = n * radix - c;
      if (n > ln
# ifdef hpux
	  || (-n > -MOST_NEGATIVE_FIXNUM) /* workaround for HP700 cc bug */
# endif
	  ) goto ovfl;
      break;
    default:
      return BOOL_F;		/* not a digit */
    }
  } while (i < len);
  if (lead_neg) {
    if (n < MOST_NEGATIVE_FIXNUM) goto ovfl;
  }
  else {
    if (n < -MOST_POSITIVE_FIXNUM) goto ovfl;
    n = -n;
  }
  return MAKINUM(n);
 ovfl:				/* overflow scheme integer */
  return BOOL_F;
}
