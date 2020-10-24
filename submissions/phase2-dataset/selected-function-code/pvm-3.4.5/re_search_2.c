re_search_2 (bufp, string1, size1, string2, size2, startpos, range, regs, stop)
     struct re_pattern_buffer *bufp;
     const char *string1, *string2;
     int size1, size2;
     int startpos;
     int range;
     struct re_registers *regs;
     int stop;
{
  int val;
  register char *fastmap = bufp->fastmap;
  register char *translate = bufp->translate;
  int total_size = size1 + size2;
  int endpos = startpos + range;

  /* Check for out-of-range STARTPOS.  */
  if (startpos < 0 || startpos > total_size)
    return -1;
    
  /* Fix up RANGE if it might eventually take us outside
     the virtual concatenation of STRING1 and STRING2.  */
  if (endpos < -1)
    range = -1 - startpos;
  else if (endpos > total_size)
    range = total_size - startpos;

  /* If the search isn't to be a backwards one, don't waste time in a
     search for a pattern that must be anchored.  */
  if (bufp->used > 0 && (re_opcode_t) bufp->buffer[0] == begbuf && range > 0)
    {
      if (startpos > 0)
	return -1;
      else
	range = 1;
    }

  /* Update the fastmap now if not correct already.  */
  if (fastmap && !bufp->fastmap_accurate)
    if (re_compile_fastmap (bufp) == -2)
      return -2;
  
  /* Loop through the string, looking for a place to start matching.  */
  for (;;)
    { 
      /* If a fastmap is supplied, skip quickly over characters that
         cannot be the start of a match.  If the pattern can match the
         null string, however, we don't need to skip characters; we want
         the first null string.  */
      if (fastmap && startpos < total_size && !bufp->can_be_null)
	{
	  if (range > 0)	/* Searching forwards.  */
	    {
	      register const char *d;
	      register int lim = 0;
	      int irange = range;

              if (startpos < size1 && startpos + range >= size1)
                lim = range - (size1 - startpos);

	      d = (startpos >= size1 ? string2 - size1 : string1) + startpos;
   
              /* Written out as an if-else to avoid testing `translate'
                 inside the loop.  */
	      if (translate)
                while (range > lim
                       && !fastmap[(unsigned char)
				   translate[(unsigned char) *d++]])
                  range--;
	      else
                while (range > lim && !fastmap[(unsigned char) *d++])
                  range--;

	      startpos += irange - range;
	    }
	  else				/* Searching backwards.  */
	    {
	      register char c = (size1 == 0 || startpos >= size1
                                 ? string2[startpos - size1] 
                                 : string1[startpos]);

	      if (!fastmap[(unsigned char) TRANSLATE (c)])
		goto advance;
	    }
	}

      /* If can't match the null string, and that's all we have left, fail.  */
      if (range >= 0 && startpos == total_size && fastmap
          && !bufp->can_be_null)
	return -1;

      val = re_match_2 (bufp, string1, size1, string2, size2,
	                startpos, regs, stop);
      if (val >= 0)
	return startpos;
        
      if (val == -2)
	return -2;

    advance:
      if (!range) 
        break;
      else if (range > 0) 
        {
          range--; 
          startpos++;
        }
      else
        {
          range++; 
          startpos--;
        }
    }
  return -1;
} /* re_search_2 */
