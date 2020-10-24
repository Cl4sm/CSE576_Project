  long *
regex_compile(str, replace)
  char *str;
  char *replace;
  /* Compile the regular expression `str' and return a pointer to the
   * compiled regex-code.  If `replace' is non-zero, the string `replace'
   * is interpreted as a replace-string, which may contain `\'-escape
   * sequences including references to parenthesized matches (`\0',
   * `\1', ... `\9'), where `\0' stands for the whole match.
   */
{
# define REGEX_EOF(position) (rx_size ? (position) >= rx_size : 0)
# define EAT_WHITESPACE { while (*cp == ' ' || *cp == '\t') ++cp; }
# define GET_OCT(o, n) {                                                      \
  for ((o) = 0, i = 0; i < (n) && isoct(*cp) && ((o) << 1) >= 0; ++i, ++cp)   \
    (o) *= 8, (o) += *cp - '0'; }
# define GET_HEX(h, n) {                                                      \
  for ((h) = 0, i = 0; i < (n) && ishex(*cp) && ((h) << 1) >= 0; ++i, ++cp)   \
    (h) *= 16, (h) += *cp > '9' ? tolower(*cp) - 'a' + 10 : *cp - '0'; }
# define GET_DEC(d, n) {                                                      \
  for ((d) = 0, i = 0; i < (n) && isdigit(*cp) && ((d) << 1) >= 0; ++i, ++cp) \
    (d) *= 10, (d) += *cp - '0'; }
# define GET_NUMBER(n) {                                                      \
  if (*cp == '0')                                                             \
    if (*++cp == 'x') { ++cp; GET_HEX((n), 32); } else { GET_OCT((n), 32); }  \
  else { GET_DEC((n), 32); } }

  char *cp, *s;

  int pc;
    /* Parentheses counter; number of unmatched parentheses.
     */
  long *par[1024];
    /* Stack of positions of opening parentheses.
     * For `pc > 0', `par[pc]' points to the first regex-command after the
     * `PAR_OPEN'-command that opened the current parentheses-level `pc'.
     * `par[0]' is equal to `regex'.
     */
  long *branch[1024];
    /* Stack of positions of branches.
     * The top element points to the first regex-command in the branch
     * currently generated (in the current parentheses-level `pc').
     */

  /* We need to know the positions of *all* successive `BRANCH'-commands
   * for *all* levels of parentheses.  This means we need a stack of
   * stacks.  The first stack pointer is `pc', the parentheses counter.
   */
  long *branch_n[REGEX_MAX_PARS][REGEX_MAX_BRANCH];
    /* On par-level `pc', `branch_n[pc][x]' points to the `x'th `BRANCH'-
     * command on that level.
     */
  int branch_len[REGEX_MAX_PARS][REGEX_MAX_BRANCH];
    /* On par-level `pc', `branch_len[pc][x]' holds the length of the `x'th
     * `BRANCH'-command on that level.  If `branch_len[pc][x] < 0' the]
     * length of that branch is variable.
     */
  int branch_n_c[REGEX_MAX_PARS];
    /* On par-level `pc' there are `branch_n_c[pc]' `BRANCH'-commands.
     */

# define BRANCH_LEN_INC(x) {                                                  \
    if (branch_len[pc][branch_n_c[pc]] >= 0)                                  \
      branch_len[pc][branch_n_c[pc]] += (x);                                  \
    exp_len = (x);                                                            \
  }

  long ref_len[256];
    /* If we want to know the length of all strings (if unique) matching
     * an expression which contains back-references, we need to store the
     * lengths of all references.
     */
  long *expression;
    /* Pointer to the beginning of the current expression.  This pointer
     * is relevant for processing operators (like `\?', `\*', `\+', ...).
     */
  int exp_len;
    /* length of the current expression.
     */
  int escape;
    /* Set to 1, if the last read character was a '\\', else 0.
     */
  int start;
  int slot;
    /* Number of slots used by pairs of parentheses.  The slot 0 is reserved
     * for the whole expression.
     */
  static long *regex = 0;
  int regex_size = 0;
  const int regex_blocksize = 1 << 16;
  long *pp;
  char escape_char[128];

  int i, j;

  escape_char['a'] = '\a'; escape_char['b'] = '\b'; escape_char['f'] = '\f';
  escape_char['n'] = '\n'; escape_char['r'] = '\r'; escape_char['t'] = '\t';
  escape_char['v'] = '\v';

  /* we don't know yet, how much memory will be needed to store the resulting
   * regex-code, so we allocate `regex_blocksize' bytes in the first place.
   * if it turns out to be insufficient, we simply cancel the compilation,
   * reallocate the double amount of memory and restart from the beginning.
   * the following macro will take care of watching the memory-consumption:
   */
# define CHECK_PP {                                                           \
    if (pp - regex > regex_size * regex_blocksize - 512) goto compile;        \
  }

  if (regex) free((char *)regex);

compile:
  if (regex_size) {
    regex_size <<= 1;
    regex =
      (long *)realloc(regex, regex_size * regex_blocksize * sizeof(long));
  } else
    ++regex_size, regex =
      (long *)malloc(regex_blocksize * sizeof(long));

  par[0] = regex + 1;
  branch[0] = regex + 1;
  branch_n_c[0] = 0;
  branch_len[0][0] = 0;
  pp = regex;
  cp = str;
  expression = 0;
  exp_len = 0;
  escape = 0;
  start = 1;
  pc = 0;
  slot = 1;

  rx_start = -1;
  /*
   * Yes, I know this generates warnings from both the compiler and cppcheck.
   * Unfortunately, fixing it would mean basically redoing the regular
   * expression parser; I've been thinking about using the libc one for some
   * time, but not quite right now.
   *   -- Peter Pentchev, 2010/12/29
   */
  branch[-1] = regex;

  ++pp; /* leave space for the replace offset */
  while (*cp) {
    switch (*cp++) {
      /* escape character */
      case '\\':
        if (escape) {
	  expression = pp;
	  *pp++ = CHAR;
	  *pp++ = '\\';
	  if (start) rx_start = '\\';
	  BRANCH_LEN_INC(1);
	} else {
	  ++escape;
	  continue;
	}
	break;

      /* simple escapes */
      case 'a': case 'b': case 'f': case 'n': case 'r': case 't': case 'v':
        if (escape) {
	  expression = pp;
	  *pp++ = CHAR;
	  *pp++ = escape_char[(int)cp[-1]];
	  if (start) rx_start = pp[-1];
	  BRANCH_LEN_INC(1);
	} else
	  goto Default;
	break;
	  
      /* octal, hex, decimal escapes */
      case 'o':
        /* The strings `\0', `\1', ... `\9' are interpreted as back
	 * references, therefore octal escape sequences have to be
	 * prefixed with `\o'.
	 */
        if (escape) {
	  int oct;
	  expression = pp;
	  if (*cp == 'o') { /* octal string */
	    long *length;
	    ++cp;
	    *pp++ = STRING;
	    *(length = pp++) = 1;
	    EAT_WHITESPACE;
	    for (;; ++*length) {
	      GET_OCT(oct, 3);
	      if (oct > 255) { rx_error = (int)E_invalid_character; break; }
	      *pp++ = oct;
	      CHECK_PP;
	      if (start) rx_start = pp[-1], start = 0;
	      EAT_WHITESPACE;
	      if (!isoct(*cp)) break;
	    }
	    BRANCH_LEN_INC(*length);
	  } else { /* octal character */
	    GET_OCT(oct, 3);
	    if (oct > 255) { rx_error = (int)E_invalid_character; break; }
	    *pp++ = CHAR;
	    *pp++ = oct;
	    if (start) rx_start = pp[-1];
	    BRANCH_LEN_INC(1);
	  }
	} else
	  goto Default;
	break;
      case 'x':
        if (escape) {
	  int hex;
	  expression = pp;
	  if (*cp == 'x') { /* hex string */
	    long *length;
	    ++cp;
	    *pp++ = STRING;
	    *(length = pp++) = 1;
	    EAT_WHITESPACE;
	    for (;; ++*length) {
	      GET_HEX(hex, 2);
	      *pp++ = hex;
	      CHECK_PP;
	      if (start) rx_start = pp[-1], start = 0;
	      EAT_WHITESPACE;
	      if (!ishex(*cp)) break;
	    }
	    BRANCH_LEN_INC(*length);
	  } else { /* hex character */
	    GET_HEX(hex, 2);
	    *pp++ = (long)CHAR;
	    *pp++ = hex;
	    if (start) rx_start = pp[-1];
	    BRANCH_LEN_INC(1);
	  }
	} else
	  goto Default;
	break;
      case 'd':  /* special: decimal */
        if (escape) {
	  int dec;
	  expression = pp;
	  if (*cp == 'd') { /* decimal string */
	    long *length;
	    ++cp;
	    *pp++ = STRING;
	    *(length = pp++) = 1;
	    EAT_WHITESPACE;
	    for (;; ++*length) {
	      GET_DEC(dec, 3);
	      if (dec > 255) { rx_error = (int)E_invalid_character; break; }
	      CHECK_PP;
	      *pp++ = dec;
	      if (start) rx_start = pp[-1], start = 0;
	      EAT_WHITESPACE;
	      if (!isdigit(*cp)) break;
	    }
	    BRANCH_LEN_INC(*length);
	  } else { /* decimal character */
	    GET_DEC(dec, 3);
	    if (dec > 255) { rx_error = (int)E_invalid_character; break; }
	    *pp++ = CHAR;
	    *pp++ = dec;
	    if (start) rx_start = pp[-1];
	    BRANCH_LEN_INC(1);
	  }
	} else
	  goto Default;
	break;

      /* ranges */
      case '[':
        if ((escape && rx_nomagic) || (!escape && !rx_nomagic)) {
	  u_char any[1024], table[256];
	  int nescape = 0;
	  i = 0;
	  expression = pp;
	  if (*cp == '^')
	    ++cp, *pp++ = ANY_BUT;
	  else
	    *pp++ = ANY_OF;
	  do {
	    switch (*cp++) {
	      case 0:
	        rx_error = (int)E_unmatched_bracket;
		break;
	      case '\\':
	        if (!nescape) { nescape = 1; continue; } else goto Default1;
		break;
	      case '-':
	        if (*cp == ']' || !i)
		  any[i++] = '-';
		else {
		  if (any[i - 1] > *cp) {
		    rx_error = (int)E_invalid_range;
		    break;
		  }
		  for (j = any[i - 1] + 1; j <= *cp; ++j) any[i++] = j;
		  ++cp;
		}
		break;
	      case 'a': case 'b': case 'f': case 'n': case 'r':
	      case 't': case 'v':
		if (nescape)
		  any[i++] = escape_char[(int)cp[-1]];
		else
		  goto Default1;
		break;
	      case '0': case '1': case '2': case '3': case '4':
	      case '5': case '6': case '7':
	        if (nescape) {
		  /* using back references in a range doesn't make sense, so
		   * we treat them as octal characters. */
		  --cp;
		} else
		  goto Default1;
		/* fall through */
	      case 'o':  /* octal character */
	        if (nescape) {
		  int oct;
		  GET_OCT(oct, 3);
		  if (oct > 255) {
                    rx_error = (int)E_invalid_character;
                    break;
                  }
		  any[i++] = oct;
		} else
		  goto Default1;
		break;
	      case 'd':  /* decimal character */
	        if (nescape) {
		  int dec;
		  GET_DEC(dec, 3);
		  if (dec > 255) {
                    rx_error = (int)E_invalid_character;
                    break;
                  }
		  any[i++] = dec;
		} else
		  goto Default1;
		break;
	      case 'x':  /* hex character */
	        if (nescape) {
		  int hex;
		  GET_HEX(hex, 2);
		  any[i++] = hex;
		} else
		  goto Default1;
		break;
	      default: Default1:
	        any[i++] = cp[-1];
	    }
	    if (rx_error) break;
	    nescape = 0;
	  } while (*cp != ']');
	  if (rx_error) break;
	  ++cp;
	  /* sort the list and remove duplicate entries. */
	  memset(table, 0, 256);
	  for (j = 0; j < i; ++j) table[any[j]] = 1;
	  for (i = 0, j = 0; j < 256; ++j) if (table[j]) any[i++] = j;
	  *pp++ = i;
	  for (j = 0; j < i; ++j) *pp++ = any[j];
	  BRANCH_LEN_INC(1);
	} else
	  goto Default;
	break;

      /* parentheses, branches */
      case '(':
        if ((escape && !rx_allmagic) || (!escape && rx_allmagic)) {
	  expression = 0;
	  *pp++ = PAR_OPEN;
	  ++pc;
	  branch[pc] = par[pc] = pp;
	  branch_len[pc][0] = 0;
	  branch_n_c[pc] = 0;
	} else
	  goto Default;
	break;
      case '|':
        if ((escape && !rx_allmagic) || (!escape && rx_allmagic)) {
	  expression = 0;
	  *pp++ = EOX; /* end of branch */
	  /* insert a `BRANCH'-command at `branch[pc]'. */
	  for (i = -1; pp + i >= branch[pc]; --i)
	    pp[i + SIZE_BRANCH] = pp[i];
	  branch[pc][0] = BRANCH;
	  branch[pc][2] = pp - branch[pc];
	  /* store the position of this `BRANCH' command. */
	  branch_n[pc][branch_n_c[pc]++] = branch[pc];
	  branch_len[pc][branch_n_c[pc]] = 0;
	  pp += SIZE_BRANCH;
	  branch[pc] = pp;
	  if (!pc) rx_start = -1;
	} else
	  goto Default;
	break;
      case ')':
        if ((escape && !rx_allmagic) || (!escape && rx_allmagic)) {
	  /* NOTE: if the parenthesized expression is split up into
	   * multiple branches, the last branch is *not* terminated
	   * by an `EOX'. */
	  *pp++ = PAR_CLOSE;
	  *pp++ = slot++;
	  /* first we'll check if all branches have equal length. */
	  j = branch_len[pc][0];
	  if (branch_n_c[pc]) {
	    for (i = 1; i <= branch_n_c[pc]; ++i)
	      if (j != branch_len[pc][i]) { i = 0; break; }
	  } else
	    i = 1;
	  /* fill in the `n'-field (see description of the `BRANCH'-command)
	   * of all `BRANCH'-commands of the current par-level `pc'. */
	  while (branch_n_c[pc]--) {
	    branch_n[pc][branch_n_c[pc]][1] =
	      pp - (branch_n[pc][branch_n_c[pc]] + SIZE_BRANCH + 2);
	  }
	  expression = par[pc] - 1;
	  if (!pc--) {
	    rx_error = (int)E_unmatched_closing_parenthesis;
	    break;
	  }
	  /* if `i' is set, `j' holds the uniqe length of all branches. */
	  if (i) {
	    BRANCH_LEN_INC(j);
	  } else {
	    branch_len[pc][branch_n_c[pc]] = -1;
	    exp_len = -1;
	  }
	  ref_len[slot - 1] = i ? j : -1;
	} else
	  goto Default;
	break;

      /* back references */
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        if (escape) {
	  expression = pp;
	  *pp++ = BACKREF;
	  *pp++ = cp[-1] - '0';
	  if (ref_len[cp[-1] - '0'] >= 0) {
	    BRANCH_LEN_INC(ref_len[cp[-1] - '0']);
	  } else {
	    branch_len[pc][branch_n_c[pc]] = -1;
	    exp_len = -1;
	  }
	} else
	  goto Default;
        break;
        
      /* repeat operators */
      case '=': /* ... to stay compatible with VIM */
      case '?':
	if ((escape && !rx_allmagic) || (!escape && rx_allmagic)) {
	  if (!expression) {
	    rx_error = (int)E_operator_without_operand;
	    break;
	  }
	  *pp++ = EOX; /* end of expression */
	  /* insert a `REPEAT'-command at `expression'. */
	  for (i = -1; pp + i >= expression; --i)
	    pp[i + SIZE_REPEAT] = pp[i];
	  expression[0] = REPEAT;
	  expression[1] = 0;
	  expression[2] = 1;
	  expression[3] = pp - expression;
	  pp += SIZE_REPEAT;
	  branch_len[pc][branch_n_c[pc]] = -1;
	  expression = 0;
	} else
	  goto Default;
	break;
      case '+':
	if ((escape && !rx_allmagic) || (!escape && rx_allmagic)) {
	  int no_limit = 0;
	  if (!expression) {
	    rx_error = (int)E_operator_without_operand;
	    break;
	  }
	  if (*cp == '+') {
	    if (exp_len < 0) {
	      rx_error = (int)E_operand_with_variable_length;
	      break;
	    }
	    no_limit = 1;
	    ++cp;
	  }
	  *pp++ = EOX; /* end of expression */
	  if (exp_len < 0) {
	    /* insert a `REPEAT'-command at `expression'. */
	    for (i = -1; pp + i >= expression; --i)
	      pp[i + SIZE_REPEAT] = pp[i];
	    expression[0] = REPEAT;
	    expression[1] = 1;
	    expression[2] = -1;
	    expression[3] = pp - expression;
	    pp += SIZE_REPEAT;
	  } else {
	    /* insert a `FIXREPEAT'-command at `expression'. */
	    for (i = -1; pp + i >= expression; --i)
	      pp[i + SIZE_FIXREPEAT] = pp[i];
	    expression[0] = FIXREPEAT;
	    expression[1] = 1;
	    expression[2] = no_limit ? -1 : rx_maxmatch;
	    expression[3] = exp_len;
	    expression[4] = pp - expression;
	    pp += SIZE_FIXREPEAT;
	    exp_len = -1;
	  }
	  branch_len[pc][branch_n_c[pc]] = -1;
	  expression = 0;
	} else
	  goto Default;
	break;
      case '*':
        if ((escape && rx_nomagic) || (!escape && !rx_nomagic)) {
	  int no_limit = 0;
	  if (!expression) {
	    rx_error = (int)E_operator_without_operand;
	    break;
	  }
	  if (*cp == '*') {
	    if (exp_len < 0) {
	      rx_error = (int)E_operand_with_variable_length;
	      break;
	    }
	    no_limit = 1;
	    ++cp;
	  }
	  *pp++ = EOX; /* end of expression */
	  if (exp_len < 0) {
	    /* insert a `REPEAT'-command at `expression'. */
	    for (i = -1; pp + i >= expression; --i)
	      pp[i + SIZE_REPEAT] = pp[i];
	    expression[0] = REPEAT;
	    expression[1] = 0;
	    expression[2] = -1;
	    expression[3] = pp - expression;
	    pp += SIZE_REPEAT;
	  } else {
	    /* insert a `FIXREPEAT'-command at `expression'. */
	    for (i = -1; pp + i >= expression; --i)
	      pp[i + SIZE_FIXREPEAT] = pp[i];
	    expression[0] = FIXREPEAT;
	    expression[1] = 0;
	    expression[2] = no_limit ? -1 : rx_maxmatch;
	    expression[3] = exp_len;
	    expression[4] = pp - expression;
	    pp += SIZE_FIXREPEAT;
	    exp_len = -1;
	  }
	  branch_len[pc][branch_n_c[pc]] = -1;
	  expression = 0;
	} else
	  goto Default;
	break;
      case '{':
	if ((escape && !rx_allmagic) || (!escape && rx_allmagic)) {
	  long min, max;
	  if (!expression) {
	    rx_error = (int)E_operator_without_operand;
	    break;
	  }
	  EAT_WHITESPACE;
	  GET_NUMBER(min);
	  EAT_WHITESPACE;
          if (*cp == '\\') ++cp;
	  if (*cp == '}')
	    max = min;
	  else {
	    if (*cp++ != ',') { rx_error = (int)E_malformed_operator; break; }
	    EAT_WHITESPACE;
	    GET_NUMBER(max);
	    EAT_WHITESPACE;
	  }
          if (*cp == '\\') ++cp;
	  if ((max && max < min) || *cp++ != '}') {
	    rx_error = (int)E_malformed_operator;
	    break;
	  }
	  if (!max) max = exp_len < 0 ? rx_maxmatch : -1;
	  *pp++ = EOX; /* end of expression */
	  if (exp_len < 0) {
	    /* insert a `REPEAT'-command at `expression'. */
	    for (i = -1; pp + i >= expression; --i)
	      pp[i + SIZE_REPEAT] = pp[i];
	    expression[0] = REPEAT;
	    expression[1] = min;
	    expression[2] = max;
	    expression[3] = pp - expression;
	    pp += SIZE_REPEAT;
	  } else {
	    /* insert a `FIXREPEAT'-command at `expression'. */
	    for (i = -1; pp + i >= expression; --i)
	      pp[i + SIZE_FIXREPEAT] = pp[i];
	    expression[0] = FIXREPEAT;
	    expression[1] = min;
	    expression[2] = max;
	    expression[3] = exp_len;
	    expression[4] = pp - expression;
	    pp += SIZE_FIXREPEAT;
	    exp_len = -1;
	  }
	  if (min != max)
	    branch_len[pc][branch_n_c[pc]] = -1;
	  else
	    BRANCH_LEN_INC(min);
	  expression = 0;
	} else
	  goto Default;
	break;

      /* context specifiers */
      case '<':
	if ((escape && !rx_allmagic) || (!escape && rx_allmagic)) {
	  *pp++ = BEGIN_WORD;
	  expression = 0;
	} else
	  goto Default;
	break;
      case '>':
	if ((escape && !rx_allmagic) || (!escape && rx_allmagic)) {
	  *pp++ = END_WORD;
	  expression = 0;
	} else
	  goto Default;
	break;
      case '^':
        if (!escape) {
	  *pp++ = BEGIN_LINE;
	  expression = 0;
	} else
	  goto Default;
	break;
      case '$':
        if (!escape) {
	  *pp++ = END_LINE;
	  expression = 0;
	} else
	  goto Default;
	break;

      /* any character */
      case '.':
        if ((escape && rx_nomagic) || (!escape && !rx_nomagic)) {
	  expression = pp;
	  *pp++ = ANY_CHAR;
	  BRANCH_LEN_INC(1);
	} else
	  goto Default;
	break;
	
      /* single character */
      default: Default:
        expression = pp;
	if (start) rx_start = cp[-1];
        *pp++ = CHAR;
	*pp++ = cp[-1];
	BRANCH_LEN_INC(1);
	break;
    } /* switch */
    start = escape = 0;
    CHECK_PP;
    if (rx_error) break;
  } /* while */

  if (pc) {
    if (!rx_error) rx_error = (int)E_unmatched_opening_parenthesis;
    goto exit_regex_compile;
  }
    
  if (branch_n_c[0]) {
    /* NOTE: if the whole expression is split up into multiple branches, the
     * last branch is *not* terminated by an `EOX'.
     */
    /* fill in the `n'-field (see description of the `BRANCH'-command)
     * of all `BRANCH'-commands. */
    while (branch_n_c[0]--) {
      branch_n[pc][branch_n_c[0]][1] =
	pp - (branch_n[pc][branch_n_c[0]] + SIZE_BRANCH);
    }
  }
  *pp++ = EOX;
  *pp++ = 0;

  *regex = pp - regex; /* set the replace offset */
  if (!replace) {
    *pp++ = 0;
    goto exit_regex_compile;
  }
  /* compile the `replace'-string:  we only need two commands for the
   * replace code:  `STRING' and `BACKREF'.
   */
  cp = replace;
  escape = 0;
  s = (char *)malloc(strlen(replace) + 1);
  j = 0;
  while (*cp) {
    switch (*cp++) {
      /* escape character */
      case '\\':
        if (!escape) {
	  ++escape;
	  continue;
	} else
	  goto Default2;
	break;

      /* simple escapes */
      case 'a': case 'b': case 'f': case 'n': case 'r': case 't': case 'v':
        if (escape) s[j++] = escape_char[(int)cp[-1]]; else goto Default2;
	break;
	  
      /* octal, hex, decimal escapes */
      case 'o':
        /* The strings `\0', `\1', ... `\9' are interpreted as back
	 * references, therefore octal escape sequences have to be
	 * prefixed with `\o'.
	 */
        if (escape) {
	  int oct;
	  if (*cp == 'o') { /* octal string */
	    ++cp;
	    EAT_WHITESPACE;
	    for (;;) {
	      GET_OCT(oct, 3);
	      if (oct > 255) { rx_error = (int)E_invalid_character; break; }
	      s[j++] = (char)oct;
	      EAT_WHITESPACE;
	      if (!isoct(*cp)) { ++cp; break; }
	    }
	  } else { /* octal character */
	    GET_OCT(oct, 3);
	    if (oct > 255) { rx_error = (int)E_invalid_character; break; }
	    s[j++] = (char)oct;
	  }
	} else
	  goto Default2;
	break;
      case 'x':
        if (escape) {
	  int hex;
	  if (*cp == 'x') { /* hex string */
	    ++cp;
	    EAT_WHITESPACE;
	    for (;;) {
	      GET_HEX(hex, 2);
	      s[j++] = (char)hex;
	      EAT_WHITESPACE;
	      if (!ishex(*cp)) { ++cp; break; }
	    }
	  } else { /* hex character */
	    GET_HEX(hex, 2);
	    s[j++] = (char)hex;
	  }
	} else
	  goto Default2;
	break;
      case 'd':
        if (escape) {
	  int dec;
	  if (*cp == 'd') { /* decimal string */
	    ++cp;
	    EAT_WHITESPACE;
	    for (;;) {
	      GET_DEC(dec, 3);
	      if (dec > 255) { rx_error = (int)E_invalid_character; break; }
	      s[j++] = (char)dec;
	      EAT_WHITESPACE;
	      if (!isdigit(*cp)) { ++cp; break; }
	    }
	  } else { /* decimal character */
	    GET_DEC(dec, 3);
	    if (dec > 255) { rx_error = (int)E_invalid_character; break; }
	    s[j++] = (char)dec;
	  }
	} else
	  goto Default2;
	break;

      /* back references */
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        if (escape) {
	  if (j) {
	    if (pp - regex + j > regex_size * regex_blocksize - 64)
	      goto compile;
	      /* it is sufficient lo leave 64 bytes of extra space here,
	       * because there won't be any `ANY_OF' of `ANY_BUT' commands
	       * in the replace-code. */
	    *pp++ = STRING;
	    *pp++ = j;
	    for (i = 0; i < j; ++i) *pp++ = s[i];
	    j = 0;
	  }
	  *pp++ = BACKREF;
	  *pp++ = cp[-1] - '0';
	} else
	  goto Default2;
	break;

      default: Default2:
        s[j++] = cp[-1];
	break;
    } /* switch */
    escape = 0;
    if (rx_error) break;
    CHECK_PP;
  } /* while */
  if (j) {
    if (pp - regex + j > regex_size * regex_blocksize - 64) goto compile;
    *pp++ = STRING;
    *pp++ = j;
    for (i = 0; i < j; ++i) *pp++ = s[i];
  }
  *pp++ = EOX;
  *pp = 0;

exit_regex_compile:
  return rx_error ? 0 : regex;
}
