     char *str;
     long len;
     long radix;
{
  int i = 0;
  char ex = 0;
  char ex_p = 0, rx_p = 0;	/* Only allow 1 exactness and 1 radix prefix */
  SCM res;
  if (len==1)
    if (*str=='+' || *str=='-') /* Catches lone `+' and `-' for speed */
      return BOOL_F;

  while ((len-i) >= 2  &&  str[i]=='#' && ++i)
    switch (str[i++]) {
    case 'b': case 'B':  if (rx_p++) return BOOL_F; radix = 2;  break;
    case 'o': case 'O':  if (rx_p++) return BOOL_F; radix = 8;  break;
    case 'd': case 'D':  if (rx_p++) return BOOL_F; radix = 10; break;
    case 'x': case 'X':  if (rx_p++) return BOOL_F; radix = 16; break;
    case 'i': case 'I':  if (ex_p++) return BOOL_F; ex = 2;     break;
    case 'e': case 'E':  if (ex_p++) return BOOL_F; ex = 1;     break;
    default:  return BOOL_F;
    }

  switch (ex) {
  case 1:
    return istr2int(&str[i], len-i, radix);
  case 0:
    res = istr2int(&str[i], len-i, radix);
    if (NFALSEP(res)) return res;
#ifdef FLOATS
  case 2: return istr2flo(&str[i], len-i, radix);
#endif
  }
  return BOOL_F;
}
