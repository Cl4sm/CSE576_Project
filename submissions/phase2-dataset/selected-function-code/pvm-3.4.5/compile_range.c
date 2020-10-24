static reg_errcode_t
compile_range (p_ptr, pend, translate, syntax, b)
    const char **p_ptr, *pend;
    char *translate;
    reg_syntax_t syntax;
    unsigned char *b;
{
  unsigned this_char;

  const char *p = *p_ptr;
  int range_start, range_end;
  
  if (p == pend)
    return REG_ERANGE;

  /* Even though the pattern is a signed `char *', we need to fetch
     with unsigned char *'s; if the high bit of the pattern character
     is set, the range endpoints will be negative if we fetch using a
     signed char *.

     We also want to fetch the endpoints without translating them; the 
     appropriate translation is done in the bit-setting loop below.  */
  range_start = ((unsigned char *) p)[-2];
  range_end   = ((unsigned char *) p)[0];

  /* Have to increment the pointer into the pattern string, so the
     caller isn't still at the ending character.  */
  (*p_ptr)++;

  /* If the start is after the end, the range is empty.  */
  if (range_start > range_end)
    return syntax & RE_NO_EMPTY_RANGES ? REG_ERANGE : REG_NOERROR;

  /* Here we see why `this_char' has to be larger than an `unsigned
     char' -- the range is inclusive, so if `range_end' == 0xff
     (assuming 8-bit characters), we would otherwise go into an infinite
     loop, since all characters <= 0xff.  */
  for (this_char = range_start; this_char <= range_end; this_char++)
    {
      SET_LIST_BIT (TRANSLATE (this_char));
    }
  
  return REG_NOERROR;
}
