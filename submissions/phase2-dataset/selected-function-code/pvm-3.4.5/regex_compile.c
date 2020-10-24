regex_compile (pattern, size, syntax, bufp)
     const char *pattern;
     int size;
     reg_syntax_t syntax;
     struct re_pattern_buffer *bufp;
{
  /* We fetch characters from PATTERN here.  Even though PATTERN is
     `char *' (i.e., signed), we declare these variables as unsigned, so
     they can be reliably used as array indices.  */
  register unsigned char c, c1;
  
  /* A random tempory spot in PATTERN.  */
  const char *p1;

  /* Points to the end of the buffer, where we should append.  */
  register unsigned char *b;
  
  /* Keeps track of unclosed groups.  */
  compile_stack_type compile_stack;

  /* Points to the current (ending) position in the pattern.  */
  const char *p = pattern;
  const char *pend = pattern + size;
  
  /* How to translate the characters in the pattern.  */
  char *translate = bufp->translate;

  /* Address of the count-byte of the most recently inserted `exactn'
     command.  This makes it possible to tell if a new exact-match
     character can be added to that command or if the character requires
     a new `exactn' command.  */
  unsigned char *pending_exact = 0;

  /* Address of start of the most recently finished expression.
     This tells, e.g., postfix * where to find the start of its
     operand.  Reset at the beginning of groups and alternatives.  */
  unsigned char *laststart = 0;

  /* Address of beginning of regexp, or inside of last group.  */
  unsigned char *begalt;

  /* Place in the uncompiled pattern (i.e., the {) to
     which to go back if the interval is invalid.  */
  const char *beg_interval;
                
  /* Address of the place where a forward jump should go to the end of
     the containing expression.  Each alternative of an `or' -- except the
     last -- ends with a forward jump of this sort.  */
  unsigned char *fixup_alt_jump = 0;

  /* Counts open-groups as they are encountered.  Remembered for the
     matching close-group on the compile stack, so the same register
     number is put in the stop_memory as the start_memory.  */
  regnum_t regnum = 0;

#ifdef DEBUG
  DEBUG_PRINT1 ("\nCompiling pattern: ");
  if (debug)
    {
      unsigned debug_count;
      
      for (debug_count = 0; debug_count < size; debug_count++)
        printchar (pattern[debug_count]);
      putchar ('\n');
    }
#endif /* DEBUG */

  /* Initialize the compile stack.  */
  compile_stack.stack = TALLOC (INIT_COMPILE_STACK_SIZE, compile_stack_elt_t);
  if (compile_stack.stack == NULL)
    return REG_ESPACE;

  compile_stack.size = INIT_COMPILE_STACK_SIZE;
  compile_stack.avail = 0;

  /* Initialize the pattern buffer.  */
  bufp->syntax = syntax;
  bufp->fastmap_accurate = 0;
  bufp->not_bol = bufp->not_eol = 0;

  /* Set `used' to zero, so that if we return an error, the pattern
     printer (for debugging) will think there's no pattern.  We reset it
     at the end.  */
  bufp->used = 0;
  
  /* Always count groups, whether or not bufp->no_sub is set.  */
  bufp->re_nsub = 0;				

#if !defined (emacs) && !defined (SYNTAX_TABLE)
  /* Initialize the syntax table.  */
   init_syntax_once ();
#endif

  if (bufp->allocated == 0)
    {
      if (bufp->buffer)
	{ /* If zero allocated, but buffer is non-null, try to realloc
             enough space.  This loses if buffer's address is bogus, but
             that is the user's responsibility.  */
          RETALLOC (bufp->buffer, INIT_BUF_SIZE, unsigned char);
        }
      else
        { /* Caller did not allocate a buffer.  Do it for them.  */
          bufp->buffer = TALLOC (INIT_BUF_SIZE, unsigned char);
        }
      if (!bufp->buffer) return REG_ESPACE;

      bufp->allocated = INIT_BUF_SIZE;
    }

  begalt = b = bufp->buffer;

  /* Loop through the uncompiled pattern until we're at the end.  */
  while (p != pend)
    {
      PATFETCH (c);

      switch (c)
        {
        case '^':
          {
            if (   /* If at start of pattern, it's an operator.  */
                   p == pattern + 1
                   /* If context independent, it's an operator.  */
                || syntax & RE_CONTEXT_INDEP_ANCHORS
                   /* Otherwise, depends on what's come before.  */
                || at_begline_loc_p (pattern, p, syntax))
              BUF_PUSH (begline);
            else
              goto normal_char;
          }
          break;


        case '$':
          {
            if (   /* If at end of pattern, it's an operator.  */
                   p == pend 
                   /* If context independent, it's an operator.  */
                || syntax & RE_CONTEXT_INDEP_ANCHORS
                   /* Otherwise, depends on what's next.  */
                || at_endline_loc_p (p, pend, syntax))
               BUF_PUSH (endline);
             else
               goto normal_char;
           }
           break;


	case '+':
        case '?':
          if ((syntax & RE_BK_PLUS_QM)
              || (syntax & RE_LIMITED_OPS))
            goto normal_char;
        handle_plus:
        case '*':
          /* If there is no previous pattern... */
          if (!laststart)
            {
              if (syntax & RE_CONTEXT_INVALID_OPS)
                return REG_BADRPT;
              else if (!(syntax & RE_CONTEXT_INDEP_OPS))
                goto normal_char;
            }

          {
            /* Are we optimizing this jump?  */
            boolean keep_string_p = false;
            
            /* 1 means zero (many) matches is allowed.  */
            char zero_times_ok = 0, many_times_ok = 0;

            /* If there is a sequence of repetition chars, collapse it
               down to just one (the right one).  We can't combine
               interval operators with these because of, e.g., `a{2}*',
               which should only match an even number of `a's.  */

            for (;;)
              {
                zero_times_ok |= c != '+';
                many_times_ok |= c != '?';

                if (p == pend)
                  break;

                PATFETCH (c);

                if (c == '*'
                    || (!(syntax & RE_BK_PLUS_QM) && (c == '+' || c == '?')))
                  ;

                else if (syntax & RE_BK_PLUS_QM  &&  c == '\\')
                  {
                    if (p == pend) return REG_EESCAPE;

                    PATFETCH (c1);
                    if (!(c1 == '+' || c1 == '?'))
                      {
                        PATUNFETCH;
                        PATUNFETCH;
                        break;
                      }

                    c = c1;
                  }
                else
                  {
                    PATUNFETCH;
                    break;
                  }

                /* If we get here, we found another repeat character.  */
               }

            /* Star, etc. applied to an empty pattern is equivalent
               to an empty pattern.  */
            if (!laststart)  
              break;

            /* Now we know whether or not zero matches is allowed
               and also whether or not two or more matches is allowed.  */
            if (many_times_ok)
              { /* More than one repetition is allowed, so put in at the
                   end a backward relative jump from `b' to before the next
                   jump we're going to put in below (which jumps from
                   laststart to after this jump).  

                   But if we are at the `*' in the exact sequence `.*\n',
                   insert an unconditional jump backwards to the .,
                   instead of the beginning of the loop.  This way we only
                   push a failure point once, instead of every time
                   through the loop.  */
                assert (p - 1 > pattern);

                /* Allocate the space for the jump.  */
                GET_BUFFER_SPACE (3);

                /* We know we are not at the first character of the pattern,
                   because laststart was nonzero.  And we've already
                   incremented `p', by the way, to be the character after
                   the `*'.  Do we have to do something analogous here
                   for null bytes, because of RE_DOT_NOT_NULL?  */
                if (TRANSLATE (*(p - 2)) == TRANSLATE ('.')
		    && zero_times_ok
                    && p < pend && TRANSLATE (*p) == TRANSLATE ('\n')
                    && !(syntax & RE_DOT_NEWLINE))
                  { /* We have .*\n.  */
                    STORE_JUMP (jump, b, laststart);
                    keep_string_p = true;
                  }
                else
                  /* Anything else.  */
                  STORE_JUMP (maybe_pop_jump, b, laststart - 3);

                /* We've added more stuff to the buffer.  */
                b += 3;
              }

            /* On failure, jump from laststart to b + 3, which will be the
               end of the buffer after this jump is inserted.  */
            GET_BUFFER_SPACE (3);
            INSERT_JUMP (keep_string_p ? on_failure_keep_string_jump
                                       : on_failure_jump,
                         laststart, b + 3);
            pending_exact = 0;
            b += 3;

            if (!zero_times_ok)
              {
                /* At least one repetition is required, so insert a
                   `dummy_failure_jump' before the initial
                   `on_failure_jump' instruction of the loop. This
                   effects a skip over that instruction the first time
                   we hit that loop.  */
                GET_BUFFER_SPACE (3);
                INSERT_JUMP (dummy_failure_jump, laststart, laststart + 6);
                b += 3;
              }
            }
	  break;


	case '.':
          laststart = b;
          BUF_PUSH (anychar);
          break;


        case '[':
          {
            boolean had_char_class = false;

            if (p == pend) return REG_EBRACK;

            /* Ensure that we have enough space to push a charset: the
               opcode, the length count, and the bitset; 34 bytes in all.  */
	    GET_BUFFER_SPACE (34);

            laststart = b;

            /* We test `*p == '^' twice, instead of using an if
               statement, so we only need one BUF_PUSH.  */
            BUF_PUSH (*p == '^' ? charset_not : charset); 
            if (*p == '^')
              p++;

            /* Remember the first position in the bracket expression.  */
            p1 = p;

            /* Push the number of bytes in the bitmap.  */
            BUF_PUSH ((1 << BYTEWIDTH) / BYTEWIDTH);

            /* Clear the whole map.  */
            bzero (b, (1 << BYTEWIDTH) / BYTEWIDTH);

            /* charset_not matches newline according to a syntax bit.  */
            if ((re_opcode_t) b[-2] == charset_not
                && (syntax & RE_HAT_LISTS_NOT_NEWLINE))
              SET_LIST_BIT ('\n');

            /* Read in characters and ranges, setting map bits.  */
            for (;;)
              {
                if (p == pend) return REG_EBRACK;

                PATFETCH (c);

                /* \ might escape characters inside [...] and [^...].  */
                if ((syntax & RE_BACKSLASH_ESCAPE_IN_LISTS) && c == '\\')
                  {
                    if (p == pend) return REG_EESCAPE;

                    PATFETCH (c1);
                    SET_LIST_BIT (c1);
                    continue;
                  }

                /* Could be the end of the bracket expression.  If it's
                   not (i.e., when the bracket expression is `[]' so
                   far), the ']' character bit gets set way below.  */
                if (c == ']' && p != p1 + 1)
                  break;

                /* Look ahead to see if it's a range when the last thing
                   was a character class.  */
                if (had_char_class && c == '-' && *p != ']')
                  return REG_ERANGE;

                /* Look ahead to see if it's a range when the last thing
                   was a character: if this is a hyphen not at the
                   beginning or the end of a list, then it's the range
                   operator.  */
                if (c == '-' 
                    && !(p - 2 >= pattern && p[-2] == '[') 
                    && !(p - 3 >= pattern && p[-3] == '[' && p[-2] == '^')
                    && *p != ']')
                  {
                    reg_errcode_t ret
                      = compile_range (&p, pend, translate, syntax, b);
                    if (ret != REG_NOERROR) return ret;
                  }

                else if (p[0] == '-' && p[1] != ']')
                  { /* This handles ranges made up of characters only.  */
                    reg_errcode_t ret;

		    /* Move past the `-'.  */
                    PATFETCH (c1);
                    
                    ret = compile_range (&p, pend, translate, syntax, b);
                    if (ret != REG_NOERROR) return ret;
                  }

                /* See if we're at the beginning of a possible character
                   class.  */

                else if (syntax & RE_CHAR_CLASSES && c == '[' && *p == ':')
                  { /* Leave room for the null.  */
                    char str[CHAR_CLASS_MAX_LENGTH + 1];

                    PATFETCH (c);
                    c1 = 0;

                    /* If pattern is `[[:'.  */
                    if (p == pend) return REG_EBRACK;

                    for (;;)
                      {
                        PATFETCH (c);
                        if (c == ':' || c == ']' || p == pend
                            || c1 == CHAR_CLASS_MAX_LENGTH)
                          break;
                        str[c1++] = c;
                      }
                    str[c1] = '\0';

                    /* If isn't a word bracketed by `[:' and:`]':
                       undo the ending character, the letters, and leave 
                       the leading `:' and `[' (but set bits for them).  */
                    if (c == ':' && *p == ']')
                      {
                        int ch;
                        boolean is_alnum = STREQ (str, "alnum");
                        boolean is_alpha = STREQ (str, "alpha");
                        boolean is_blank = STREQ (str, "blank");
                        boolean is_cntrl = STREQ (str, "cntrl");
                        boolean is_digit = STREQ (str, "digit");
                        boolean is_graph = STREQ (str, "graph");
                        boolean is_lower = STREQ (str, "lower");
                        boolean is_print = STREQ (str, "print");
                        boolean is_punct = STREQ (str, "punct");
                        boolean is_space = STREQ (str, "space");
                        boolean is_upper = STREQ (str, "upper");
                        boolean is_xdigit = STREQ (str, "xdigit");
                        
                        if (!IS_CHAR_CLASS (str)) return REG_ECTYPE;

                        /* Throw away the ] at the end of the character
                           class.  */
                        PATFETCH (c);					

                        if (p == pend) return REG_EBRACK;

                        for (ch = 0; ch < 1 << BYTEWIDTH; ch++)
                          {
                            if (   (is_alnum  && ISALNUM (ch))
                                || (is_alpha  && ISALPHA (ch))
                                || (is_blank  && ISBLANK (ch))
                                || (is_cntrl  && ISCNTRL (ch))
                                || (is_digit  && ISDIGIT (ch))
                                || (is_graph  && ISGRAPH (ch))
                                || (is_lower  && ISLOWER (ch))
                                || (is_print  && ISPRINT (ch))
                                || (is_punct  && ISPUNCT (ch))
                                || (is_space  && ISSPACE (ch))
                                || (is_upper  && ISUPPER (ch))
                                || (is_xdigit && ISXDIGIT (ch)))
                            SET_LIST_BIT (ch);
                          }
                        had_char_class = true;
                      }
                    else
                      {
                        c1++;
                        while (c1--)    
                          PATUNFETCH;
                        SET_LIST_BIT ('[');
                        SET_LIST_BIT (':');
                        had_char_class = false;
                      }
                  }
                else
                  {
                    had_char_class = false;
                    SET_LIST_BIT (c);
                  }
              }

            /* Discard any (non)matching list bytes that are all 0 at the
               end of the map.  Decrease the map-length byte too.  */
            while ((int) b[-1] > 0 && b[b[-1] - 1] == 0) 
              b[-1]--; 
            b += b[-1];
          }
          break;


	case '(':
          if (syntax & RE_NO_BK_PARENS)
            goto handle_open;
          else
            goto normal_char;


        case ')':
          if (syntax & RE_NO_BK_PARENS)
            goto handle_close;
          else
            goto normal_char;


        case '\n':
          if (syntax & RE_NEWLINE_ALT)
            goto handle_alt;
          else
            goto normal_char;


	case '|':
          if (syntax & RE_NO_BK_VBAR)
            goto handle_alt;
          else
            goto normal_char;


        case '{':
           if (syntax & RE_INTERVALS && syntax & RE_NO_BK_BRACES)
             goto handle_interval;
           else
             goto normal_char;


        case '\\':
          if (p == pend) return REG_EESCAPE;

          /* Do not translate the character after the \, so that we can
             distinguish, e.g., \B from \b, even if we normally would
             translate, e.g., B to b.  */
          PATFETCH_RAW (c);

          switch (c)
            {
            case '(':
              if (syntax & RE_NO_BK_PARENS)
                goto normal_backslash;

            handle_open:
              bufp->re_nsub++;
              regnum++;

              if (COMPILE_STACK_FULL)
                { 
                  RETALLOC (compile_stack.stack, compile_stack.size << 1,
                            compile_stack_elt_t);
                  if (compile_stack.stack == NULL) return REG_ESPACE;

                  compile_stack.size <<= 1;
                }

              /* These are the values to restore when we hit end of this
                 group.  They are all relative offsets, so that if the
                 whole pattern moves because of realloc, they will still
                 be valid.  */
              COMPILE_STACK_TOP.begalt_offset = begalt - bufp->buffer;
              COMPILE_STACK_TOP.fixup_alt_jump 
                = fixup_alt_jump ? fixup_alt_jump - bufp->buffer + 1 : 0;
              COMPILE_STACK_TOP.laststart_offset = b - bufp->buffer;
              COMPILE_STACK_TOP.regnum = regnum;

              /* We will eventually replace the 0 with the number of
                 groups inner to this one.  But do not push a
                 start_memory for groups beyond the last one we can
                 represent in the compiled pattern.  */
              if (regnum <= MAX_REGNUM)
                {
                  COMPILE_STACK_TOP.inner_group_offset = b - bufp->buffer + 2;
                  BUF_PUSH_3 (start_memory, regnum, 0);
                }
                
              compile_stack.avail++;

              fixup_alt_jump = 0;
              laststart = 0;
              begalt = b;
	      /* If we've reached MAX_REGNUM groups, then this open
		 won't actually generate any code, so we'll have to
		 clear pending_exact explicitly.  */
	      pending_exact = 0;
              break;


            case ')':
              if (syntax & RE_NO_BK_PARENS) goto normal_backslash;

              if (COMPILE_STACK_EMPTY)
                if (syntax & RE_UNMATCHED_RIGHT_PAREN_ORD)
                  goto normal_backslash;
                else
                  return REG_ERPAREN;

            handle_close:
              if (fixup_alt_jump)
                { /* Push a dummy failure point at the end of the
                     alternative for a possible future
                     `pop_failure_jump' to pop.  See comments at
                     `push_dummy_failure' in `re_match_2'.  */
                  BUF_PUSH (push_dummy_failure);
                  
                  /* We allocated space for this jump when we assigned
                     to `fixup_alt_jump', in the `handle_alt' case below.  */
                  STORE_JUMP (jump_past_alt, fixup_alt_jump, b - 1);
                }

              /* See similar code for backslashed left paren above.  */
              if (COMPILE_STACK_EMPTY)
                if (syntax & RE_UNMATCHED_RIGHT_PAREN_ORD)
                  goto normal_char;
                else
                  return REG_ERPAREN;

              /* Since we just checked for an empty stack above, this
                 ``can't happen''.  */
              assert (compile_stack.avail != 0);
              {
                /* We don't just want to restore into `regnum', because
                   later groups should continue to be numbered higher,
                   as in `(ab)c(de)' -- the second group is #2.  */
                regnum_t this_group_regnum;

                compile_stack.avail--;		
                begalt = bufp->buffer + COMPILE_STACK_TOP.begalt_offset;
                fixup_alt_jump
                  = COMPILE_STACK_TOP.fixup_alt_jump
                    ? bufp->buffer + COMPILE_STACK_TOP.fixup_alt_jump - 1 
                    : 0;
                laststart = bufp->buffer + COMPILE_STACK_TOP.laststart_offset;
                this_group_regnum = COMPILE_STACK_TOP.regnum;
		/* If we've reached MAX_REGNUM groups, then this open
		   won't actually generate any code, so we'll have to
		   clear pending_exact explicitly.  */
		pending_exact = 0;

                /* We're at the end of the group, so now we know how many
                   groups were inside this one.  */
                if (this_group_regnum <= MAX_REGNUM)
                  {
                    unsigned char *inner_group_loc
                      = bufp->buffer + COMPILE_STACK_TOP.inner_group_offset;
                    
                    *inner_group_loc = regnum - this_group_regnum;
                    BUF_PUSH_3 (stop_memory, this_group_regnum,
                                regnum - this_group_regnum);
                  }
              }
              break;


            case '|':					/* `\|'.  */
              if (syntax & RE_LIMITED_OPS || syntax & RE_NO_BK_VBAR)
                goto normal_backslash;
            handle_alt:
              if (syntax & RE_LIMITED_OPS)
                goto normal_char;

              /* Insert before the previous alternative a jump which
                 jumps to this alternative if the former fails.  */
              GET_BUFFER_SPACE (3);
              INSERT_JUMP (on_failure_jump, begalt, b + 6);
              pending_exact = 0;
              b += 3;

              /* The alternative before this one has a jump after it
                 which gets executed if it gets matched.  Adjust that
                 jump so it will jump to this alternative's analogous
                 jump (put in below, which in turn will jump to the next
                 (if any) alternative's such jump, etc.).  The last such
                 jump jumps to the correct final destination.  A picture:
                          _____ _____ 
                          |   | |   |   
                          |   v |   v 
                         a | b   | c   

                 If we are at `b', then fixup_alt_jump right now points to a
                 three-byte space after `a'.  We'll put in the jump, set
                 fixup_alt_jump to right after `b', and leave behind three
                 bytes which we'll fill in when we get to after `c'.  */

              if (fixup_alt_jump)
                STORE_JUMP (jump_past_alt, fixup_alt_jump, b);

              /* Mark and leave space for a jump after this alternative,
                 to be filled in later either by next alternative or
                 when know we're at the end of a series of alternatives.  */
              fixup_alt_jump = b;
              GET_BUFFER_SPACE (3);
              b += 3;

              laststart = 0;
              begalt = b;
              break;


            case '{': 
              /* If \{ is a literal.  */
              if (!(syntax & RE_INTERVALS)
                     /* If we're at `\{' and it's not the open-interval 
                        operator.  */
                  || ((syntax & RE_INTERVALS) && (syntax & RE_NO_BK_BRACES))
                  || (p - 2 == pattern  &&  p == pend))
                goto normal_backslash;

            handle_interval:
              {
                /* If got here, then the syntax allows intervals.  */

                /* At least (most) this many matches must be made.  */
                int lower_bound = -1, upper_bound = -1;

                beg_interval = p - 1;

                if (p == pend)
                  {
                    if (syntax & RE_NO_BK_BRACES)
                      goto unfetch_interval;
                    else
                      return REG_EBRACE;
                  }

                GET_UNSIGNED_NUMBER (lower_bound);

                if (c == ',')
                  {
                    GET_UNSIGNED_NUMBER (upper_bound);
                    if (upper_bound < 0) upper_bound = RE_DUP_MAX;
                  }
                else
                  /* Interval such as `{1}' => match exactly once. */
                  upper_bound = lower_bound;

                if (lower_bound < 0 || upper_bound > RE_DUP_MAX
                    || lower_bound > upper_bound)
                  {
                    if (syntax & RE_NO_BK_BRACES)
                      goto unfetch_interval;
                    else 
                      return REG_BADBR;
                  }

                if (!(syntax & RE_NO_BK_BRACES)) 
                  {
                    if (c != '\\') return REG_EBRACE;

                    PATFETCH (c);
                  }

                if (c != '}')
                  {
                    if (syntax & RE_NO_BK_BRACES)
                      goto unfetch_interval;
                    else 
                      return REG_BADBR;
                  }

                /* We just parsed a valid interval.  */

                /* If it's invalid to have no preceding re.  */
                if (!laststart)
                  {
                    if (syntax & RE_CONTEXT_INVALID_OPS)
                      return REG_BADRPT;
                    else if (syntax & RE_CONTEXT_INDEP_OPS)
                      laststart = b;
                    else
                      goto unfetch_interval;
                  }

                /* If the upper bound is zero, don't want to succeed at
                   all; jump from `laststart' to `b + 3', which will be
                   the end of the buffer after we insert the jump.  */
                 if (upper_bound == 0)
                   {
                     GET_BUFFER_SPACE (3);
                     INSERT_JUMP (jump, laststart, b + 3);
                     b += 3;
                   }

                 /* Otherwise, we have a nontrivial interval.  When
                    we're all done, the pattern will look like:
                      set_number_at <jump count> <upper bound>
                      set_number_at <succeed_n count> <lower bound>
                      succeed_n <after jump addr> <succed_n count>
                      <body of loop>
                      jump_n <succeed_n addr> <jump count>
                    (The upper bound and `jump_n' are omitted if
                    `upper_bound' is 1, though.)  */
                 else 
                   { /* If the upper bound is > 1, we need to insert
                        more at the end of the loop.  */
                     unsigned nbytes = 10 + (upper_bound > 1) * 10;

                     GET_BUFFER_SPACE (nbytes);

                     /* Initialize lower bound of the `succeed_n', even
                        though it will be set during matching by its
                        attendant `set_number_at' (inserted next),
                        because `re_compile_fastmap' needs to know.
                        Jump to the `jump_n' we might insert below.  */
                     INSERT_JUMP2 (succeed_n, laststart,
                                   b + 5 + (upper_bound > 1) * 5,
                                   lower_bound);
                     b += 5;

                     /* Code to initialize the lower bound.  Insert 
                        before the `succeed_n'.  The `5' is the last two
                        bytes of this `set_number_at', plus 3 bytes of
                        the following `succeed_n'.  */
                     insert_op2 (set_number_at, laststart, 5, lower_bound, b);
                     b += 5;

                     if (upper_bound > 1)
                       { /* More than one repetition is allowed, so
                            append a backward jump to the `succeed_n'
                            that starts this interval.
                            
                            When we've reached this during matching,
                            we'll have matched the interval once, so
                            jump back only `upper_bound - 1' times.  */
                         STORE_JUMP2 (jump_n, b, laststart + 5,
                                      upper_bound - 1);
                         b += 5;

                         /* The location we want to set is the second
                            parameter of the `jump_n'; that is `b-2' as
                            an absolute address.  `laststart' will be
                            the `set_number_at' we're about to insert;
                            `laststart+3' the number to set, the source
                            for the relative address.  But we are
                            inserting into the middle of the pattern --
                            so everything is getting moved up by 5.
                            Conclusion: (b - 2) - (laststart + 3) + 5,
                            i.e., b - laststart.
                            
                            We insert this at the beginning of the loop
                            so that if we fail during matching, we'll
                            reinitialize the bounds.  */
                         insert_op2 (set_number_at, laststart, b - laststart,
                                     upper_bound - 1, b);
                         b += 5;
                       }
                   }
                pending_exact = 0;
                beg_interval = NULL;
              }
              break;

            unfetch_interval:
              /* If an invalid interval, match the characters as literals.  */
               assert (beg_interval);
               p = beg_interval;
               beg_interval = NULL;

               /* normal_char and normal_backslash need `c'.  */
               PATFETCH (c);	

               if (!(syntax & RE_NO_BK_BRACES))
                 {
                   if (p > pattern  &&  p[-1] == '\\')
                     goto normal_backslash;
                 }
               goto normal_char;

#ifdef emacs
            /* There is no way to specify the before_dot and after_dot
               operators.  rms says this is ok.  --karl  */
            case '=':
              BUF_PUSH (at_dot);
              break;

            case 's':	
              laststart = b;
              PATFETCH (c);
              BUF_PUSH_2 (syntaxspec, syntax_spec_code[c]);
              break;

            case 'S':
              laststart = b;
              PATFETCH (c);
              BUF_PUSH_2 (notsyntaxspec, syntax_spec_code[c]);
              break;
#endif /* emacs */


            case 'w':
              laststart = b;
              BUF_PUSH (wordchar);
              break;


            case 'W':
              laststart = b;
              BUF_PUSH (notwordchar);
              break;


            case '<':
              BUF_PUSH (wordbeg);
              break;

            case '>':
              BUF_PUSH (wordend);
              break;

            case 'b':
              BUF_PUSH (wordbound);
              break;

            case 'B':
              BUF_PUSH (notwordbound);
              break;

            case '`':
              BUF_PUSH (begbuf);
              break;

            case '\'':
              BUF_PUSH (endbuf);
              break;

            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
              if (syntax & RE_NO_BK_REFS)
                goto normal_char;

              c1 = c - '0';

              if (c1 > regnum)
                return REG_ESUBREG;

              /* Can't back reference to a subexpression if inside of it.  */
              if (group_in_compile_stack (compile_stack, c1))
                goto normal_char;

              laststart = b;
              BUF_PUSH_2 (duplicate, c1);
              break;


            case '+':
            case '?':
              if (syntax & RE_BK_PLUS_QM)
                goto handle_plus;
              else
                goto normal_backslash;

            default:
            normal_backslash:
              /* You might think it would be useful for \ to mean
                 not to translate; but if we don't translate it
                 it will never match anything.  */
              c = TRANSLATE (c);
              goto normal_char;
            }
          break;


	default:
        /* Expects the character in `c'.  */
	normal_char:
	      /* If no exactn currently being built.  */
          if (!pending_exact 

              /* If last exactn not at current position.  */
              || pending_exact + *pending_exact + 1 != b
              
              /* We have only one byte following the exactn for the count.  */
	      || *pending_exact == (1 << BYTEWIDTH) - 1

              /* If followed by a repetition operator.  */
              || *p == '*' || *p == '^'
	      || ((syntax & RE_BK_PLUS_QM)
		  ? *p == '\\' && (p[1] == '+' || p[1] == '?')
		  : (*p == '+' || *p == '?'))
	      || ((syntax & RE_INTERVALS)
                  && ((syntax & RE_NO_BK_BRACES)
		      ? *p == '{'
                      : (p[0] == '\\' && p[1] == '{'))))
	    {
	      /* Start building a new exactn.  */
              
              laststart = b;

	      BUF_PUSH_2 (exactn, 0);
	      pending_exact = b - 1;
            }
            
	  BUF_PUSH (c);
          (*pending_exact)++;
	  break;
        } /* switch (c) */
    } /* while p != pend */

  
  /* Through the pattern now.  */
  
  if (fixup_alt_jump)
    STORE_JUMP (jump_past_alt, fixup_alt_jump, b);

  if (!COMPILE_STACK_EMPTY) 
    return REG_EPAREN;

  free (compile_stack.stack);

  /* We have succeeded; set the length of the buffer.  */
  bufp->used = b - bufp->buffer;

#ifdef DEBUG
  if (debug)
    {
      DEBUG_PRINT1 ("\nCompiled pattern: ");
      print_compiled_pattern (bufp);
    }
#endif /* DEBUG */

  return REG_NOERROR;
} /* regex_compile */
