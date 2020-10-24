re_match_2 (bufp, string1, size1, string2, size2, pos, regs, stop)
     struct re_pattern_buffer *bufp;
     const char *string1, *string2;
     int size1, size2;
     int pos;
     struct re_registers *regs;
     int stop;
{
  /* General temporaries.  */
  int mcnt;
  unsigned char *p1;

  /* Just past the end of the corresponding string.  */
  const char *end1, *end2;

  /* Pointers into string1 and string2, just past the last characters in
     each to consider matching.  */
  const char *end_match_1, *end_match_2;

  /* Where we are in the data, and the end of the current string.  */
  const char *d, *dend;
  
  /* Where we are in the pattern, and the end of the pattern.  */
  unsigned char *p = bufp->buffer;
  register unsigned char *pend = p + bufp->used;

  /* We use this to map every character in the string.  */
  char *translate = bufp->translate;

  /* Failure point stack.  Each place that can handle a failure further
     down the line pushes a failure point on this stack.  It consists of
     restart, regend, and reg_info for all registers corresponding to
     the subexpressions we're currently inside, plus the number of such
     registers, and, finally, two char *'s.  The first char * is where
     to resume scanning the pattern; the second one is where to resume
     scanning the strings.  If the latter is zero, the failure point is
     a ``dummy''; if a failure happens and the failure point is a dummy,
     it gets discarded and the next next one is tried.  */
  fail_stack_type fail_stack;
#ifdef DEBUG
  static unsigned failure_id = 0;
  unsigned nfailure_points_pushed = 0, nfailure_points_popped = 0;
#endif

  /* We fill all the registers internally, independent of what we
     return, for use in backreferences.  The number here includes
     an element for register zero.  */
  unsigned num_regs = bufp->re_nsub + 1;
  
  /* The currently active registers.  */
  unsigned lowest_active_reg = NO_LOWEST_ACTIVE_REG;
  unsigned highest_active_reg = NO_HIGHEST_ACTIVE_REG;

  /* Information on the contents of registers. These are pointers into
     the input strings; they record just what was matched (on this
     attempt) by a subexpression part of the pattern, that is, the
     regnum-th regstart pointer points to where in the pattern we began
     matching and the regnum-th regend points to right after where we
     stopped matching the regnum-th subexpression.  (The zeroth register
     keeps track of what the whole pattern matches.)  */
  const char **regstart, **regend;

  /* If a group that's operated upon by a repetition operator fails to
     match anything, then the register for its start will need to be
     restored because it will have been set to wherever in the string we
     are when we last see its open-group operator.  Similarly for a
     register's end.  */
  const char **old_regstart, **old_regend;

  /* The is_active field of reg_info helps us keep track of which (possibly
     nested) subexpressions we are currently in. The matched_something
     field of reg_info[reg_num] helps us tell whether or not we have
     matched any of the pattern so far this time through the reg_num-th
     subexpression.  These two fields get reset each time through any
     loop their register is in.  */
  register_info_type *reg_info; 

  /* The following record the register info as found in the above
     variables when we find a match better than any we've seen before. 
     This happens as we backtrack through the failure points, which in
     turn happens only if we have not yet matched the entire string. */
  unsigned best_regs_set = false;
  const char **best_regstart, **best_regend;
  
  /* Logically, this is `best_regend[0]'.  But we don't want to have to
     allocate space for that if we're not allocating space for anything
     else (see below).  Also, we never need info about register 0 for
     any of the other register vectors, and it seems rather a kludge to
     treat `best_regend' differently than the rest.  So we keep track of
     the end of the best match so far in a separate variable.  We
     initialize this to NULL so that when we backtrack the first time
     and need to test it, it's not garbage.  */
  const char *match_end = NULL;

  /* Used when we pop values we don't care about.  */
  const char **reg_dummy;
  register_info_type *reg_info_dummy;

#ifdef DEBUG
  /* Counts the total number of registers pushed.  */
  unsigned num_regs_pushed = 0; 	
#endif

  DEBUG_PRINT1 ("\n\nEntering re_match_2.\n");
  
  INIT_FAIL_STACK ();
  
  /* Do not bother to initialize all the register variables if there are
     no groups in the pattern, as it takes a fair amount of time.  If
     there are groups, we include space for register 0 (the whole
     pattern), even though we never use it, since it simplifies the
     array indexing.  We should fix this.  */
  if (bufp->re_nsub)
    {
      regstart = REGEX_TALLOC (num_regs, const char *);
      regend = REGEX_TALLOC (num_regs, const char *);
      old_regstart = REGEX_TALLOC (num_regs, const char *);
      old_regend = REGEX_TALLOC (num_regs, const char *);
      best_regstart = REGEX_TALLOC (num_regs, const char *);
      best_regend = REGEX_TALLOC (num_regs, const char *);
      reg_info = REGEX_TALLOC (num_regs, register_info_type);
      reg_dummy = REGEX_TALLOC (num_regs, const char *);
      reg_info_dummy = REGEX_TALLOC (num_regs, register_info_type);

      if (!(regstart && regend && old_regstart && old_regend && reg_info 
            && best_regstart && best_regend && reg_dummy && reg_info_dummy)) 
        {
          FREE_VARIABLES ();
          return -2;
        }
    }
#ifdef REGEX_MALLOC
  else
    {
      /* We must initialize all our variables to NULL, so that
         `FREE_VARIABLES' doesn't try to free them.  */
      regstart = regend = old_regstart = old_regend = best_regstart
        = best_regend = reg_dummy = NULL;
      reg_info = reg_info_dummy = (register_info_type *) NULL;
    }
#endif /* REGEX_MALLOC */

  /* The starting position is bogus.  */
  if (pos < 0 || pos > size1 + size2)
    {
      FREE_VARIABLES ();
      return -1;
    }
    
  /* Initialize subexpression text positions to -1 to mark ones that no
     start_memory/stop_memory has been seen for. Also initialize the
     register information struct.  */
  for (mcnt = 1; mcnt < num_regs; mcnt++)
    {
      regstart[mcnt] = regend[mcnt] 
        = old_regstart[mcnt] = old_regend[mcnt] = REG_UNSET_VALUE;
        
      REG_MATCH_NULL_STRING_P (reg_info[mcnt]) = MATCH_NULL_UNSET_VALUE;
      IS_ACTIVE (reg_info[mcnt]) = 0;
      MATCHED_SOMETHING (reg_info[mcnt]) = 0;
      EVER_MATCHED_SOMETHING (reg_info[mcnt]) = 0;
    }
  
  /* We move `string1' into `string2' if the latter's empty -- but not if
     `string1' is null.  */
  if (size2 == 0 && string1 != NULL)
    {
      string2 = string1;
      size2 = size1;
      string1 = 0;
      size1 = 0;
    }
  end1 = string1 + size1;
  end2 = string2 + size2;

  /* Compute where to stop matching, within the two strings.  */
  if (stop <= size1)
    {
      end_match_1 = string1 + stop;
      end_match_2 = string2;
    }
  else
    {
      end_match_1 = end1;
      end_match_2 = string2 + stop - size1;
    }

  /* `p' scans through the pattern as `d' scans through the data. 
     `dend' is the end of the input string that `d' points within.  `d'
     is advanced into the following input string whenever necessary, but
     this happens before fetching; therefore, at the beginning of the
     loop, `d' can be pointing at the end of a string, but it cannot
     equal `string2'.  */
  if (size1 > 0 && pos <= size1)
    {
      d = string1 + pos;
      dend = end_match_1;
    }
  else
    {
      d = string2 + pos - size1;
      dend = end_match_2;
    }

  DEBUG_PRINT1 ("The compiled pattern is: ");
  DEBUG_PRINT_COMPILED_PATTERN (bufp, p, pend);
  DEBUG_PRINT1 ("The string to match is: `");
  DEBUG_PRINT_DOUBLE_STRING (d, string1, size1, string2, size2);
  DEBUG_PRINT1 ("'\n");
  
  /* This loops over pattern commands.  It exits by returning from the
     function if the match is complete, or it drops through if the match
     fails at this starting point in the input data.  */
  for (;;)
    {
      DEBUG_PRINT2 ("\n0x%x: ", p);

      if (p == pend)
	{ /* End of pattern means we might have succeeded.  */
          DEBUG_PRINT1 ("end of pattern ... ");
          
	  /* If we haven't matched the entire string, and we want the
             longest match, try backtracking.  */
          if (d != end_match_2)
	    {
              DEBUG_PRINT1 ("backtracking.\n");
              
              if (!FAIL_STACK_EMPTY ())
                { /* More failure points to try.  */
                  boolean same_str_p = (FIRST_STRING_P (match_end) 
	        	                == MATCHING_IN_FIRST_STRING);

                  /* If exceeds best match so far, save it.  */
                  if (!best_regs_set
                      || (same_str_p && d > match_end)
                      || (!same_str_p && !MATCHING_IN_FIRST_STRING))
                    {
                      best_regs_set = true;
                      match_end = d;
                      
                      DEBUG_PRINT1 ("\nSAVING match as best so far.\n");
                      
                      for (mcnt = 1; mcnt < num_regs; mcnt++)
                        {
                          best_regstart[mcnt] = regstart[mcnt];
                          best_regend[mcnt] = regend[mcnt];
                        }
                    }
                  goto fail;	       
                }

              /* If no failure points, don't restore garbage.  */
              else if (best_regs_set)   
                {
  	        restore_best_regs:
                  /* Restore best match.  It may happen that `dend ==
                     end_match_1' while the restored d is in string2.
                     For example, the pattern `x.*y.*z' against the
                     strings `x-' and `y-z-', if the two strings are
                     not consecutive in memory.  */
                  DEBUG_PRINT1 ("Restoring best registers.\n");
                  
                  d = match_end;
                  dend = ((d >= string1 && d <= end1)
		           ? end_match_1 : end_match_2);

		  for (mcnt = 1; mcnt < num_regs; mcnt++)
		    {
		      regstart[mcnt] = best_regstart[mcnt];
		      regend[mcnt] = best_regend[mcnt];
		    }
                }
            } /* d != end_match_2 */

          DEBUG_PRINT1 ("Accepting match.\n");

          /* If caller wants register contents data back, do it.  */
          if (regs && !bufp->no_sub)
	    {
              /* Have the register data arrays been allocated?  */
              if (bufp->regs_allocated == REGS_UNALLOCATED)
                { /* No.  So allocate them with malloc.  We need one
                     extra element beyond `num_regs' for the `-1' marker
                     GNU code uses.  */
                  regs->num_regs = MAX (RE_NREGS, num_regs + 1);
                  regs->start = TALLOC (regs->num_regs, regoff_t);
                  regs->end = TALLOC (regs->num_regs, regoff_t);
                  if (regs->start == NULL || regs->end == NULL)
                    return -2;
                  bufp->regs_allocated = REGS_REALLOCATE;
                }
              else if (bufp->regs_allocated == REGS_REALLOCATE)
                { /* Yes.  If we need more elements than were already
                     allocated, reallocate them.  If we need fewer, just
                     leave it alone.  */
                  if (regs->num_regs < num_regs + 1)
                    {
                      regs->num_regs = num_regs + 1;
                      RETALLOC (regs->start, regs->num_regs, regoff_t);
                      RETALLOC (regs->end, regs->num_regs, regoff_t);
                      if (regs->start == NULL || regs->end == NULL)
                        return -2;
                    }
                }
              else
                assert (bufp->regs_allocated == REGS_FIXED);

              /* Convert the pointer data in `regstart' and `regend' to
                 indices.  Register zero has to be set differently,
                 since we haven't kept track of any info for it.  */
              if (regs->num_regs > 0)
                {
                  regs->start[0] = pos;
                  regs->end[0] = (int) (MATCHING_IN_FIRST_STRING ?
						(long) d - (long) string1
			          : (long) d - (long) string2 + size1);
                }
              
              /* Go through the first `min (num_regs, regs->num_regs)'
                 registers, since that is all we initialized.  */
	      for (mcnt = 1; mcnt < MIN (num_regs, regs->num_regs); mcnt++)
		{
                  if (REG_UNSET (regstart[mcnt]) || REG_UNSET (regend[mcnt]))
                    regs->start[mcnt] = regs->end[mcnt] = -1;
                  else
                    {
		      regs->start[mcnt] = POINTER_TO_OFFSET (regstart[mcnt]);
                      regs->end[mcnt] = POINTER_TO_OFFSET (regend[mcnt]);
                    }
		}
              
              /* If the regs structure we return has more elements than
                 were in the pattern, set the extra elements to -1.  If
                 we (re)allocated the registers, this is the case,
                 because we always allocate enough to have at least one
                 -1 at the end.  */
              for (mcnt = num_regs; mcnt < regs->num_regs; mcnt++)
                regs->start[mcnt] = regs->end[mcnt] = -1;
	    } /* regs && !bufp->no_sub */

          FREE_VARIABLES ();
          DEBUG_PRINT4 ("%u failure points pushed, %u popped (%u remain).\n",
                        nfailure_points_pushed, nfailure_points_popped,
                        nfailure_points_pushed - nfailure_points_popped);
          DEBUG_PRINT2 ("%u registers pushed.\n", num_regs_pushed);

          mcnt = d - pos - (MATCHING_IN_FIRST_STRING 
			    ? string1 
			    : string2 - size1);

          DEBUG_PRINT2 ("Returning %d from re_match_2.\n", mcnt);

          return mcnt;
        }

      /* Otherwise match next pattern command.  */
#ifdef SWITCH_ENUM_BUG
      switch ((int) ((re_opcode_t) *p++))
#else
      switch ((re_opcode_t) *p++)
#endif
	{
        /* Ignore these.  Used to ignore the n of succeed_n's which
           currently have n == 0.  */
        case no_op:
          DEBUG_PRINT1 ("EXECUTING no_op.\n");
          break;


        /* Match the next n pattern characters exactly.  The following
           byte in the pattern defines n, and the n bytes after that
           are the characters to match.  */
	case exactn:
	  mcnt = *p++;
          DEBUG_PRINT2 ("EXECUTING exactn %d.\n", mcnt);

          /* This is written out as an if-else so we don't waste time
             testing `translate' inside the loop.  */
          if (translate)
	    {
	      do
		{
		  PREFETCH ();
		  if (translate[(unsigned char) *d++] != (char) *p++)
                    goto fail;
		}
	      while (--mcnt);
	    }
	  else
	    {
	      do
		{
		  PREFETCH ();
		  if (*d++ != (char) *p++) goto fail;
		}
	      while (--mcnt);
	    }
	  SET_REGS_MATCHED ();
          break;


        /* Match any character except possibly a newline or a null.  */
	case anychar:
          DEBUG_PRINT1 ("EXECUTING anychar.\n");

          PREFETCH ();

          if ((!(bufp->syntax & RE_DOT_NEWLINE) && TRANSLATE (*d) == '\n')
              || (bufp->syntax & RE_DOT_NOT_NULL && TRANSLATE (*d) == '\000'))
	    goto fail;

          SET_REGS_MATCHED ();
          DEBUG_PRINT2 ("  Matched `%d'.\n", *d);
          d++;
	  break;


	case charset:
	case charset_not:
	  {
	    register unsigned char c;
	    boolean not = (re_opcode_t) *(p - 1) == charset_not;

            DEBUG_PRINT2 ("EXECUTING charset%s.\n", not ? "_not" : "");

	    PREFETCH ();
	    c = TRANSLATE (*d); /* The character to match.  */

            /* Cast to `unsigned' instead of `unsigned char' in case the
               bit list is a full 32 bytes long.  */
	    if (c < (unsigned) (*p * BYTEWIDTH)
		&& p[1 + c / BYTEWIDTH] & (1 << (c % BYTEWIDTH)))
	      not = !not;

	    p += 1 + *p;

	    if (!not) goto fail;
            
	    SET_REGS_MATCHED ();
            d++;
	    break;
	  }


        /* The beginning of a group is represented by start_memory.
           The arguments are the register number in the next byte, and the
           number of groups inner to this one in the next.  The text
           matched within the group is recorded (in the internal
           registers data structure) under the register number.  */
        case start_memory:
	  DEBUG_PRINT3 ("EXECUTING start_memory %d (%d):\n", *p, p[1]);

          /* Find out if this group can match the empty string.  */
	  p1 = p;		/* To send to group_match_null_string_p.  */
          
          if (REG_MATCH_NULL_STRING_P (reg_info[*p]) == MATCH_NULL_UNSET_VALUE)
            REG_MATCH_NULL_STRING_P (reg_info[*p]) 
              = group_match_null_string_p (&p1, pend, reg_info);

          /* Save the position in the string where we were the last time
             we were at this open-group operator in case the group is
             operated upon by a repetition operator, e.g., with `(a*)*b'
             against `ab'; then we want to ignore where we are now in
             the string in case this attempt to match fails.  */
          old_regstart[*p] = REG_MATCH_NULL_STRING_P (reg_info[*p])
                             ? REG_UNSET (regstart[*p]) ? d : regstart[*p]
                             : regstart[*p];
	  DEBUG_PRINT2 ("  old_regstart: %d\n", 
			 POINTER_TO_OFFSET (old_regstart[*p]));

          regstart[*p] = d;
	  DEBUG_PRINT2 ("  regstart: %d\n", POINTER_TO_OFFSET (regstart[*p]));

          IS_ACTIVE (reg_info[*p]) = 1;
          MATCHED_SOMETHING (reg_info[*p]) = 0;
          
          /* This is the new highest active register.  */
          highest_active_reg = *p;
          
          /* If nothing was active before, this is the new lowest active
             register.  */
          if (lowest_active_reg == NO_LOWEST_ACTIVE_REG)
            lowest_active_reg = *p;

          /* Move past the register number and inner group count.  */
          p += 2;
          break;


        /* The stop_memory opcode represents the end of a group.  Its
           arguments are the same as start_memory's: the register
           number, and the number of inner groups.  */
	case stop_memory:
	  DEBUG_PRINT3 ("EXECUTING stop_memory %d (%d):\n", *p, p[1]);
             
          /* We need to save the string position the last time we were at
             this close-group operator in case the group is operated
             upon by a repetition operator, e.g., with `((a*)*(b*)*)*'
             against `aba'; then we want to ignore where we are now in
             the string in case this attempt to match fails.  */
          old_regend[*p] = REG_MATCH_NULL_STRING_P (reg_info[*p])
                           ? REG_UNSET (regend[*p]) ? d : regend[*p]
			   : regend[*p];
	  DEBUG_PRINT2 ("      old_regend: %d\n", 
			 POINTER_TO_OFFSET (old_regend[*p]));

          regend[*p] = d;
	  DEBUG_PRINT2 ("      regend: %d\n", POINTER_TO_OFFSET (regend[*p]));

          /* This register isn't active anymore.  */
          IS_ACTIVE (reg_info[*p]) = 0;
          
          /* If this was the only register active, nothing is active
             anymore.  */
          if (lowest_active_reg == highest_active_reg)
            {
              lowest_active_reg = NO_LOWEST_ACTIVE_REG;
              highest_active_reg = NO_HIGHEST_ACTIVE_REG;
            }
          else
            { /* We must scan for the new highest active register, since
                 it isn't necessarily one less than now: consider
                 (a(b)c(d(e)f)g).  When group 3 ends, after the f), the
                 new highest active register is 1.  */
              unsigned char r = *p - 1;
              while (r > 0 && !IS_ACTIVE (reg_info[r]))
                r--;
              
              /* If we end up at register zero, that means that we saved
                 the registers as the result of an `on_failure_jump', not
                 a `start_memory', and we jumped to past the innermost
                 `stop_memory'.  For example, in ((.)*) we save
                 registers 1 and 2 as a result of the *, but when we pop
                 back to the second ), we are at the stop_memory 1.
                 Thus, nothing is active.  */
	      if (r == 0)
                {
                  lowest_active_reg = NO_LOWEST_ACTIVE_REG;
                  highest_active_reg = NO_HIGHEST_ACTIVE_REG;
                }
              else
                highest_active_reg = r;
            }
          
          /* If just failed to match something this time around with a
             group that's operated on by a repetition operator, try to
             force exit from the ``loop'', and restore the register
             information for this group that we had before trying this
             last match.  */
          if ((!MATCHED_SOMETHING (reg_info[*p])
               || (re_opcode_t) p[-3] == start_memory)
	      && (p + 2) < pend)              
            {
              boolean is_a_jump_n = false;
              
              p1 = p + 2;
              mcnt = 0;
              switch ((re_opcode_t) *p1++)
                {
                  case jump_n:
		    is_a_jump_n = true;
                  case pop_failure_jump:
		  case maybe_pop_jump:
		  case jump:
		  case dummy_failure_jump:
                    EXTRACT_NUMBER_AND_INCR (mcnt, p1);
		    if (is_a_jump_n)
		      p1 += 2;
                    break;
                  
                  default:
                    /* do nothing */ ;
                }
	      p1 += mcnt;
        
              /* If the next operation is a jump backwards in the pattern
	         to an on_failure_jump right before the start_memory
                 corresponding to this stop_memory, exit from the loop
                 by forcing a failure after pushing on the stack the
                 on_failure_jump's jump in the pattern, and d.  */
              if (mcnt < 0 && (re_opcode_t) *p1 == on_failure_jump
                  && (re_opcode_t) p1[3] == start_memory && p1[4] == *p)
		{
                  /* If this group ever matched anything, then restore
                     what its registers were before trying this last
                     failed match, e.g., with `(a*)*b' against `ab' for
                     regstart[1], and, e.g., with `((a*)*(b*)*)*'
                     against `aba' for regend[3].
                     
                     Also restore the registers for inner groups for,
                     e.g., `((a*)(b*))*' against `aba' (register 3 would
                     otherwise get trashed).  */
                     
                  if (EVER_MATCHED_SOMETHING (reg_info[*p]))
		    {
		      unsigned r; 
        
                      EVER_MATCHED_SOMETHING (reg_info[*p]) = 0;
                      
		      /* Restore this and inner groups' (if any) registers.  */
                      for (r = *p; r < *p + *(p + 1); r++)
                        {
                          regstart[r] = old_regstart[r];

                          /* xx why this test?  */
                          if ((long) old_regend[r] >= (long) regstart[r])
                            regend[r] = old_regend[r];
                        }     
                    }
		  p1++;
                  EXTRACT_NUMBER_AND_INCR (mcnt, p1);
                  PUSH_FAILURE_POINT (p1 + mcnt, d, -2);

                  goto fail;
                }
            }
          
          /* Move past the register number and the inner group count.  */
          p += 2;
          break;


	/* \<digit> has been turned into a `duplicate' command which is
           followed by the numeric value of <digit> as the register number.  */
        case duplicate:
	  {
	    register const char *d2, *dend2;
	    int regno = *p++;   /* Get which register to match against.  */
	    DEBUG_PRINT2 ("EXECUTING duplicate %d.\n", regno);

	    /* Can't back reference a group which we've never matched.  */
            if (REG_UNSET (regstart[regno]) || REG_UNSET (regend[regno]))
              goto fail;
              
            /* Where in input to try to start matching.  */
            d2 = regstart[regno];
            
            /* Where to stop matching; if both the place to start and
               the place to stop matching are in the same string, then
               set to the place to stop, otherwise, for now have to use
               the end of the first string.  */

            dend2 = ((FIRST_STRING_P (regstart[regno]) 
		      == FIRST_STRING_P (regend[regno]))
		     ? regend[regno] : end_match_1);
	    for (;;)
	      {
		/* If necessary, advance to next segment in register
                   contents.  */
		while (d2 == dend2)
		  {
		    if (dend2 == end_match_2) break;
		    if (dend2 == regend[regno]) break;

                    /* End of string1 => advance to string2. */
                    d2 = string2;
                    dend2 = regend[regno];
		  }
		/* At end of register contents => success */
		if (d2 == dend2) break;

		/* If necessary, advance to next segment in data.  */
		PREFETCH ();

		/* How many characters left in this segment to match.  */
		mcnt = dend - d;
                
		/* Want how many consecutive characters we can match in
                   one shot, so, if necessary, adjust the count.  */
                if (mcnt > dend2 - d2)
		  mcnt = dend2 - d2;
                  
		/* Compare that many; failure if mismatch, else move
                   past them.  */
		if (translate 
                    ? bcmp_translate (d, d2, mcnt, translate) 
                    : bcmp (d, d2, mcnt))
		  goto fail;
		d += mcnt, d2 += mcnt;
	      }
	  }
	  break;


        /* begline matches the empty string at the beginning of the string
           (unless `not_bol' is set in `bufp'), and, if
           `newline_anchor' is set, after newlines.  */
	case begline:
          DEBUG_PRINT1 ("EXECUTING begline.\n");
          
          if (AT_STRINGS_BEG (d))
            {
              if (!bufp->not_bol) break;
            }
          else if (d[-1] == '\n' && bufp->newline_anchor)
            {
              break;
            }
          /* In all other cases, we fail.  */
          goto fail;


        /* endline is the dual of begline.  */
	case endline:
          DEBUG_PRINT1 ("EXECUTING endline.\n");

          if (AT_STRINGS_END (d))
            {
              if (!bufp->not_eol) break;
            }
          
          /* We have to ``prefetch'' the next character.  */
          else if ((d == end1 ? *string2 : *d) == '\n'
                   && bufp->newline_anchor)
            {
              break;
            }
          goto fail;


	/* Match at the very beginning of the data.  */
        case begbuf:
          DEBUG_PRINT1 ("EXECUTING begbuf.\n");
          if (AT_STRINGS_BEG (d))
            break;
          goto fail;


	/* Match at the very end of the data.  */
        case endbuf:
          DEBUG_PRINT1 ("EXECUTING endbuf.\n");
	  if (AT_STRINGS_END (d))
	    break;
          goto fail;


        /* on_failure_keep_string_jump is used to optimize `.*\n'.  It
           pushes NULL as the value for the string on the stack.  Then
           `pop_failure_point' will keep the current value for the
           string, instead of restoring it.  To see why, consider
           matching `foo\nbar' against `.*\n'.  The .* matches the foo;
           then the . fails against the \n.  But the next thing we want
           to do is match the \n against the \n; if we restored the
           string value, we would be back at the foo.
           
           Because this is used only in specific cases, we don't need to
           check all the things that `on_failure_jump' does, to make
           sure the right things get saved on the stack.  Hence we don't
           share its code.  The only reason to push anything on the
           stack at all is that otherwise we would have to change
           `anychar's code to do something besides goto fail in this
           case; that seems worse than this.  */
        case on_failure_keep_string_jump:
          DEBUG_PRINT1 ("EXECUTING on_failure_keep_string_jump");
          
          EXTRACT_NUMBER_AND_INCR (mcnt, p);
          DEBUG_PRINT3 (" %d (to 0x%x):\n", mcnt, p + mcnt);

          PUSH_FAILURE_POINT (p + mcnt, NULL, -2);
          break;


	/* Uses of on_failure_jump:
        
           Each alternative starts with an on_failure_jump that points
           to the beginning of the next alternative.  Each alternative
           except the last ends with a jump that in effect jumps past
           the rest of the alternatives.  (They really jump to the
           ending jump of the following alternative, because tensioning
           these jumps is a hassle.)

           Repeats start with an on_failure_jump that points past both
           the repetition text and either the following jump or
           pop_failure_jump back to this on_failure_jump.  */
	case on_failure_jump:
        on_failure:
          DEBUG_PRINT1 ("EXECUTING on_failure_jump");

          EXTRACT_NUMBER_AND_INCR (mcnt, p);
          DEBUG_PRINT3 (" %d (to 0x%x)", mcnt, p + mcnt);

          /* If this on_failure_jump comes right before a group (i.e.,
             the original * applied to a group), save the information
             for that group and all inner ones, so that if we fail back
             to this point, the group's information will be correct.
             For example, in \(a*\)*\1, we need the preceding group,
             and in \(\(a*\)b*\)\2, we need the inner group.  */

          /* We can't use `p' to check ahead because we push
             a failure point to `p + mcnt' after we do this.  */
          p1 = p;

          /* We need to skip no_op's before we look for the
             start_memory in case this on_failure_jump is happening as
             the result of a completed succeed_n, as in \(a\)\{1,3\}b\1
             against aba.  */
          while (p1 < pend && (re_opcode_t) *p1 == no_op)
            p1++;

          if (p1 < pend && (re_opcode_t) *p1 == start_memory)
            {
              /* We have a new highest active register now.  This will
                 get reset at the start_memory we are about to get to,
                 but we will have saved all the registers relevant to
                 this repetition op, as described above.  */
              highest_active_reg = *(p1 + 1) + *(p1 + 2);
              if (lowest_active_reg == NO_LOWEST_ACTIVE_REG)
                lowest_active_reg = *(p1 + 1);
            }

          DEBUG_PRINT1 (":\n");
          PUSH_FAILURE_POINT (p + mcnt, d, -2);
          break;


        /* A smart repeat ends with `maybe_pop_jump'.
	   We change it to either `pop_failure_jump' or `jump'.  */
        case maybe_pop_jump:
          EXTRACT_NUMBER_AND_INCR (mcnt, p);
          DEBUG_PRINT2 ("EXECUTING maybe_pop_jump %d.\n", mcnt);
          {
	    register unsigned char *p2 = p;

            /* Compare the beginning of the repeat with what in the
               pattern follows its end. If we can establish that there
               is nothing that they would both match, i.e., that we
               would have to backtrack because of (as in, e.g., `a*a')
               then we can change to pop_failure_jump, because we'll
               never have to backtrack.
               
               This is not true in the case of alternatives: in
               `(a|ab)*' we do need to backtrack to the `ab' alternative
               (e.g., if the string was `ab').  But instead of trying to
               detect that here, the alternative has put on a dummy
               failure point which is what we will end up popping.  */

	    /* Skip over open/close-group commands.  */
	    while (p2 + 2 < pend
		   && ((re_opcode_t) *p2 == stop_memory
		       || (re_opcode_t) *p2 == start_memory))
	      p2 += 3;			/* Skip over args, too.  */

            /* If we're at the end of the pattern, we can change.  */
            if (p2 == pend)
	      {
		/* Consider what happens when matching ":\(.*\)"
		   against ":/".  I don't really understand this code
		   yet.  */
  	        p[-3] = (unsigned char) pop_failure_jump;
                DEBUG_PRINT1
                  ("  End of pattern: change to `pop_failure_jump'.\n");
              }

            else if ((re_opcode_t) *p2 == exactn
		     || (bufp->newline_anchor && (re_opcode_t) *p2 == endline))
	      {
		register unsigned char c
                  = *p2 == (unsigned char) endline ? '\n' : p2[2];
		p1 = p + mcnt;

                /* p1[0] ... p1[2] are the `on_failure_jump' corresponding
                   to the `maybe_finalize_jump' of this case.  Examine what 
                   follows.  */
                if ((re_opcode_t) p1[3] == exactn && p1[5] != c)
                  {
  		    p[-3] = (unsigned char) pop_failure_jump;
                    DEBUG_PRINT3 ("  %c != %c => pop_failure_jump.\n",
                                  c, p1[5]);
                  }
                  
		else if ((re_opcode_t) p1[3] == charset
			 || (re_opcode_t) p1[3] == charset_not)
		  {
		    int not = (re_opcode_t) p1[3] == charset_not;
                    
		    if (c < (unsigned char) (p1[4] * BYTEWIDTH)
			&& p1[5 + c / BYTEWIDTH] & (1 << (c % BYTEWIDTH)))
		      not = !not;

                    /* `not' is equal to 1 if c would match, which means
                        that we can't change to pop_failure_jump.  */
		    if (!not)
                      {
  		        p[-3] = (unsigned char) pop_failure_jump;
                        DEBUG_PRINT1 ("  No match => pop_failure_jump.\n");
                      }
		  }
	      }
	  }
	  p -= 2;		/* Point at relative address again.  */
	  if ((re_opcode_t) p[-1] != pop_failure_jump)
	    {
	      p[-1] = (unsigned char) jump;
              DEBUG_PRINT1 ("  Match => jump.\n");
	      goto unconditional_jump;
	    }
        /* Note fall through.  */


	/* The end of a simple repeat has a pop_failure_jump back to
           its matching on_failure_jump, where the latter will push a
           failure point.  The pop_failure_jump takes off failure
           points put on by this pop_failure_jump's matching
           on_failure_jump; we got through the pattern to here from the
           matching on_failure_jump, so didn't fail.  */
        case pop_failure_jump:
          {
            /* We need to pass separate storage for the lowest and
               highest registers, even though we don't care about the
               actual values.  Otherwise, we will restore only one
               register from the stack, since lowest will == highest in
               `pop_failure_point'.  */
            unsigned dummy_low_reg, dummy_high_reg;
            unsigned char *pdummy;
            const char *sdummy;

            DEBUG_PRINT1 ("EXECUTING pop_failure_jump.\n");
            POP_FAILURE_POINT (sdummy, pdummy,
                               dummy_low_reg, dummy_high_reg,
                               reg_dummy, reg_dummy, reg_info_dummy);
          }
          /* Note fall through.  */

          
        /* Unconditionally jump (without popping any failure points).  */
        case jump:
	unconditional_jump:
	  EXTRACT_NUMBER_AND_INCR (mcnt, p);	/* Get the amount to jump.  */
          DEBUG_PRINT2 ("EXECUTING jump %d ", mcnt);
	  p += mcnt;				/* Do the jump.  */
          DEBUG_PRINT2 ("(to 0x%x).\n", p);
	  break;

	
        /* We need this opcode so we can detect where alternatives end
           in `group_match_null_string_p' et al.  */
        case jump_past_alt:
          DEBUG_PRINT1 ("EXECUTING jump_past_alt.\n");
          goto unconditional_jump;


        /* Normally, the on_failure_jump pushes a failure point, which
           then gets popped at pop_failure_jump.  We will end up at
           pop_failure_jump, also, and with a pattern of, say, `a+', we
           are skipping over the on_failure_jump, so we have to push
           something meaningless for pop_failure_jump to pop.  */
        case dummy_failure_jump:
          DEBUG_PRINT1 ("EXECUTING dummy_failure_jump.\n");
          /* It doesn't matter what we push for the string here.  What
             the code at `fail' tests is the value for the pattern.  */
          PUSH_FAILURE_POINT (0, 0, -2);
          goto unconditional_jump;


        /* At the end of an alternative, we need to push a dummy failure
           point in case we are followed by a `pop_failure_jump', because
           we don't want the failure point for the alternative to be
           popped.  For example, matching `(a|ab)*' against `aab'
           requires that we match the `ab' alternative.  */
        case push_dummy_failure:
          DEBUG_PRINT1 ("EXECUTING push_dummy_failure.\n");
          /* See comments just above at `dummy_failure_jump' about the
             two zeroes.  */
          PUSH_FAILURE_POINT (0, 0, -2);
          break;

        /* Have to succeed matching what follows at least n times.
           After that, handle like `on_failure_jump'.  */
        case succeed_n: 
          EXTRACT_NUMBER (mcnt, p + 2);
          DEBUG_PRINT2 ("EXECUTING succeed_n %d.\n", mcnt);

          assert (mcnt >= 0);
          /* Originally, this is how many times we HAVE to succeed.  */
          if (mcnt > 0)
            {
               mcnt--;
	       p += 2;
               STORE_NUMBER_AND_INCR (p, mcnt);
               DEBUG_PRINT3 ("  Setting 0x%x to %d.\n", p, mcnt);
            }
	  else if (mcnt == 0)
            {
              DEBUG_PRINT2 ("  Setting two bytes from 0x%x to no_op.\n", p+2);
	      p[2] = (unsigned char) no_op;
              p[3] = (unsigned char) no_op;
              goto on_failure;
            }
          break;
        
        case jump_n: 
          EXTRACT_NUMBER (mcnt, p + 2);
          DEBUG_PRINT2 ("EXECUTING jump_n %d.\n", mcnt);

          /* Originally, this is how many times we CAN jump.  */
          if (mcnt)
            {
               mcnt--;
               STORE_NUMBER (p + 2, mcnt);
	       goto unconditional_jump;	     
            }
          /* If don't have to jump any more, skip over the rest of command.  */
	  else      
	    p += 4;		     
          break;
        
	case set_number_at:
	  {
            DEBUG_PRINT1 ("EXECUTING set_number_at.\n");

            EXTRACT_NUMBER_AND_INCR (mcnt, p);
            p1 = p + mcnt;
            EXTRACT_NUMBER_AND_INCR (mcnt, p);
            DEBUG_PRINT3 ("  Setting 0x%x to %d.\n", p1, mcnt);
	    STORE_NUMBER (p1, mcnt);
            break;
          }

        case wordbound:
          DEBUG_PRINT1 ("EXECUTING wordbound.\n");
          if (AT_WORD_BOUNDARY (d))
	    break;
          goto fail;

	case notwordbound:
          DEBUG_PRINT1 ("EXECUTING notwordbound.\n");
	  if (AT_WORD_BOUNDARY (d))
	    goto fail;
          break;

	case wordbeg:
          DEBUG_PRINT1 ("EXECUTING wordbeg.\n");
	  if (WORDCHAR_P (d) && (AT_STRINGS_BEG (d) || !WORDCHAR_P (d - 1)))
	    break;
          goto fail;

	case wordend:
          DEBUG_PRINT1 ("EXECUTING wordend.\n");
	  if (!AT_STRINGS_BEG (d) && WORDCHAR_P (d - 1)
              && (!WORDCHAR_P (d) || AT_STRINGS_END (d)))
	    break;
          goto fail;

#ifdef emacs
#ifdef emacs19
  	case before_dot:
          DEBUG_PRINT1 ("EXECUTING before_dot.\n");
 	  if (PTR_CHAR_POS ((unsigned char *) d) >= point)
  	    goto fail;
  	  break;
  
  	case at_dot:
          DEBUG_PRINT1 ("EXECUTING at_dot.\n");
 	  if (PTR_CHAR_POS ((unsigned char *) d) != point)
  	    goto fail;
  	  break;
  
  	case after_dot:
          DEBUG_PRINT1 ("EXECUTING after_dot.\n");
          if (PTR_CHAR_POS ((unsigned char *) d) <= point)
  	    goto fail;
  	  break;
#else /* not emacs19 */
	case at_dot:
          DEBUG_PRINT1 ("EXECUTING at_dot.\n");
	  if (PTR_CHAR_POS ((unsigned char *) d) + 1 != point)
	    goto fail;
	  break;
#endif /* not emacs19 */

	case syntaxspec:
          DEBUG_PRINT2 ("EXECUTING syntaxspec %d.\n", mcnt);
	  mcnt = *p++;
	  goto matchsyntax;

        case wordchar:
          DEBUG_PRINT1 ("EXECUTING Emacs wordchar.\n");
	  mcnt = (int) Sword;
        matchsyntax:
	  PREFETCH ();
	  if (SYNTAX (*d++) != (enum syntaxcode) mcnt)
            goto fail;
          SET_REGS_MATCHED ();
	  break;

	case notsyntaxspec:
          DEBUG_PRINT2 ("EXECUTING notsyntaxspec %d.\n", mcnt);
	  mcnt = *p++;
	  goto matchnotsyntax;

        case notwordchar:
          DEBUG_PRINT1 ("EXECUTING Emacs notwordchar.\n");
	  mcnt = (int) Sword;
        matchnotsyntax:
	  PREFETCH ();
	  if (SYNTAX (*d++) == (enum syntaxcode) mcnt)
            goto fail;
	  SET_REGS_MATCHED ();
          break;

#else /* not emacs */
	case wordchar:
          DEBUG_PRINT1 ("EXECUTING non-Emacs wordchar.\n");
	  PREFETCH ();
          if (!WORDCHAR_P (d))
            goto fail;
	  SET_REGS_MATCHED ();
          d++;
	  break;
	  
	case notwordchar:
          DEBUG_PRINT1 ("EXECUTING non-Emacs notwordchar.\n");
	  PREFETCH ();
	  if (WORDCHAR_P (d))
            goto fail;
          SET_REGS_MATCHED ();
          d++;
	  break;
#endif /* not emacs */
          
        default:
          abort ();
	}
      continue;  /* Successfully executed one pattern command; keep going.  */


    /* We goto here if a matching operation fails. */
    fail:
      if (!FAIL_STACK_EMPTY ())
	{ /* A restart point is known.  Restore to that state.  */
          DEBUG_PRINT1 ("\nFAIL:\n");
          POP_FAILURE_POINT (d, p,
                             lowest_active_reg, highest_active_reg,
                             regstart, regend, reg_info);

          /* If this failure point is a dummy, try the next one.  */
          if (!p)
	    goto fail;

          /* If we failed to the end of the pattern, don't examine *p.  */
	  assert (p <= pend);
          if (p < pend)
            {
              boolean is_a_jump_n = false;
              
              /* If failed to a backwards jump that's part of a repetition
                 loop, need to pop this failure point and use the next one.  */
              switch ((re_opcode_t) *p)
                {
                case jump_n:
                  is_a_jump_n = true;
                case maybe_pop_jump:
                case pop_failure_jump:
                case jump:
                  p1 = p + 1;
                  EXTRACT_NUMBER_AND_INCR (mcnt, p1);
                  p1 += mcnt;	

                  if ((is_a_jump_n && (re_opcode_t) *p1 == succeed_n)
                      || (!is_a_jump_n
                          && (re_opcode_t) *p1 == on_failure_jump))
                    goto fail;
                  break;
                default:
                  /* do nothing */ ;
                }
            }

          if (d >= string1 && d <= end1)
	    dend = end_match_1;
        }
      else
        break;   /* Matching at this starting point really fails.  */
    } /* for (;;) */

  if (best_regs_set)
    goto restore_best_regs;

  FREE_VARIABLES ();

  return -1;         			/* Failure to match.  */
} /* re_match_2 */
