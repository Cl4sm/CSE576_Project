regex_match_(long *regex, long position, int *parstack, int *parsp)
{
  long *pp = regex;
  long p = position;

  enum regex_e opcode;
  long min, max;
  long operand;
  long operand2;
  char *bp1;
  long base1;
  int i;

  int local_parstack[REGEX_MAX_PARS];
  int local_parsp = *parsp;
    /* Stack of unmatched parentheses while running the regex-code.
     */

  for (i = local_parsp - 1; i >= 0; --i) local_parstack[i] = parstack[i];
  if (rx_error) return 0;
  regex_set_position(p);
  for (; !rx_error;) {
    if (*rx_interrupt) {
      rx_error = (int)E_interrupt;
      regex_clear();
      *rx_interrupt = 0;
      return 0;
    }
    if ((opcode = (enum regex_e)*pp++) == EOX) goto match;
    if (rx_size) {
      if (opcode == EOF && p >= rx_size) goto match;
      if (p > rx_size) goto fail;
      if (p == rx_size && opcode != END_WORD && opcode != END_LINE) goto fail;
    }
    assert(local_parsp >= 0);
    switch (opcode) {
      case ANY_CHAR:
        if (rx_special_nl && *bp == '\n') goto fail;
	++p, ++bp;
	break;
      case CHAR:
	if (*bp++ != (char)*pp++) goto fail;
	++p;
	break;
      case STRING:
        operand = *pp++; /* length of the string. */
	if (rx_size && p + operand > rx_size) goto fail;
	/* if (memcmp(bp, pp, operand)) goto fail; */
        for (; operand; --operand) if ((char)*pp++ != *bp++) goto fail;
        /*
	pp += operand;
	bp += operand;
        */
	p += operand;
	break;
      case ANY_OF:
        operand = *pp++; /* number of characters */
	for (i = 0; i < operand && *bp > (char)*pp; ++pp, ++i);
	if (*bp != (char)*pp) goto fail;
	pp += operand - i;
	++p, ++bp;
	break;
      case ANY_BUT:
        operand = *pp++; /* number of characters */
        for (i = 0; i < operand && *bp > (char)*pp; ++pp, ++i);
	if (*bp == (char)*pp) goto fail;
        if (rx_special_nl && *bp == '\n') goto fail;
	pp += operand - i;
	++p, ++bp;
	break;
      case PAR_OPEN:
        local_parstack[local_parsp++] = p;
	break;
      case PAR_CLOSE:
        operand = *pp++; /* number of the slot. */
	regex_store(operand, local_parstack[--local_parsp], p);
	break;
      case BACKREF:
        operand = *pp++; /* number of the slot. */
	if (memcmp(bp, regex_ref(operand), i = regex_ref_len(operand)))
          goto fail;
	bp += i;
	p += i;
	if (i >= REGEX_BLOCKSIZE) regex_set_position(p);
	break;
      case BRANCH:
        operand = *pp++;
        operand2 = *pp++;
	bp1 = bp;
	base1 = buffer_base;
	if (regex_match_(pp, p, local_parstack, &local_parsp)) {
	  /* see if the first branch does the job... */
	  if (regex_match_(pp + operand, rx_match_skip,
                           local_parstack, &local_parsp)) {
	    p = rx_match_skip;
	    goto match;
	      /* NOTE:  this is not really correct, since we might have
	       *   found a longer match if we had taken the other branch,
	       *   but... `vi' does the same. */
	  }
        }
	/* the first branch didn't work, so the second branch has to. */
	pp += operand2;
	if (base1 == buffer_base) bp = bp1; else regex_set_position(p);
	break;
      case FIXREPEAT:
	min = *pp++;
	max = *pp++;
	operand = *pp++;
	operand2 = *pp++;
	for (i = 0; i < min; ++i) {
	  if (!regex_match_(pp, p, local_parstack, &local_parsp)) goto fail;
	  assert(rx_match_skip - p == operand);
	  p = rx_match_skip;
	}
	if (max < 0) max = (long)((u_long)~0 >> 1);
	if (max > min) {
	  long count;
	  long p_rec;
	  for (count = 0, p_rec = p; i < max; ++i) {
	    if (!regex_match_(pp, p, local_parstack, &local_parsp)) break;
	    ++count;
	    assert(rx_match_skip - p == operand);
	    p = rx_match_skip;
	  }
	  pp += operand2;
	  while (count && !rx_error) {
	    if (regex_match_(pp, p, local_parstack, &local_parsp)) {
	      p = rx_match_skip;
	      goto match;
	    }
	    p = p_rec + --count * operand;
	  }
	  regex_set_position(p);
	} else
	  pp += operand2;
	break;
      case REPEAT:
	min = pp[0];
	max = pp[1];
	operand = pp[2];
        pp += 3;
	for (i = 0; i < min; ++i) {
	  if (!regex_match_(pp, p, local_parstack, &local_parsp)) goto fail;
	  p = rx_match_skip;
	  regex_set_position(p);
	}
	if (max < 0) max = rx_maxmatch;
	if (max > min) {
	  long *stack, sp;
	  stack = (long *)malloc((max - min + 1) * sizeof(long));
	  for (sp = 0; max < 0 ? 1 : i < max; ++i) {
	    if (!regex_match_(pp, p, local_parstack, &local_parsp)) break;
	    stack[sp++] = p;
	    p = rx_match_skip;
	  }
	  pp += operand;
	  while (sp && !rx_error) {
	    if (regex_match_(pp, p, local_parstack, &local_parsp)) {
	      p = rx_match_skip;
	      free((char *)stack);
	      goto match;
	    }
	    p = stack[--sp];
	  }
	  regex_set_position(p);
	  free((char *)stack);
	} else
	  pp += operand;
	break;
      case BEGIN_WORD:
        if (!p) break;
        regex_set_position(--p);
	++p, ++bp;
	if (isalnum(bp[-1]) || bp[-1] == '_') goto fail;
        break;
      case END_WORD:
        if (rx_size && p == rx_size) break;
	if (isalnum(bp[0]) || bp[0] == '_') goto fail;
        if (!isalnum(bp[-1]) && bp[-1] != '_') goto fail;
	break;
      case BEGIN_LINE:
        if (!p) break;
	regex_set_position(--p);
	++p, ++bp;
	if (bp[-1] != '\n') goto fail;
	break;
      case END_LINE:
        if (rx_size && p == rx_size) break;
	if (bp[0] != '\n' && bp[0] != '\r') goto fail;
	break;
      case EOF:
        goto fail;
      default:
        abort();
    }
  } /* for */

  goto fail;

match:
  rx_match_skip = p;
  if (parstack) {
    for (i = local_parsp - 1; i >= 0; --i) parstack[i] = local_parstack[i];
    *parsp = local_parsp;
  }
  return 1;

fail:
  return 0;
}
