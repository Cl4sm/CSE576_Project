     SCM tok_buf;
     SCM port;
     int flgs;
{
  int c;
  sizet j;
  SCM p;
  if (2==(3 & flgs)) return scm_lread_rec(tok_buf, port);
 tryagain:
  c = flush_ws(port);
  switch (c) {
  case EOF: return EOF_VAL;
#ifdef BRACKETS_AS_PARENS
  case '[':
#endif
  case '(': return scm_lreadparen(tok_buf, port, flgs, s_list);
#ifdef BRACKETS_AS_PARENS
  case ']':
#endif
  case ')': return UNDEFINED; /* goto tryagain; */
  case '\'': return cons2(i_quote,
			  scm_lreadr(tok_buf, port, flgs), EOL);
  case '`': return cons2(i_quasiquote,
			 scm_lreadr(tok_buf, port, flgs), EOL);
  case ',':
    c = lgetc(port);
    if ('@'==c) p = i_uq_splicing;
    else {
      lungetc(c, port);
      p = i_unquote;
    }
    return cons2(p, scm_lreadr(tok_buf, port, flgs), EOL);
  case '#':
    c = lgetc(port);
    switch (c) {
#ifdef BRACKETS_AS_PARENS
    case '[':
#endif
    case '(':
      p = scm_lreadparen(tok_buf, port, flgs, s_vector);
      return NULLP(p) ? nullvect : vector(p);
    case 't': case 'T': return BOOL_T;
    case 'f': case 'F': return BOOL_F;
    case 'b': case 'B': case 'o': case 'O':
    case 'd': case 'D': case 'x': case 'X':
    case 'i': case 'I': case 'e': case 'E':
      lungetc(c, port);
      c = '#';
      goto num;
    case '*':
      j = scm_read_token(c, tok_buf, port, flgs);
      p = istr2bve(CHARS(tok_buf)+1, (long)(j-1));
      if (NFALSEP(p)) return p;
      else goto unkshrp;
    case '\\':
      c = lgetc(port);
      if ('\\'==c) {
	CHARS(tok_buf)[0] = c;
	j = 1;
      } else j = scm_read_token(c, tok_buf, port, flgs);
      if (j==1) return MAKICHR(c);
      for (c = 0;c<sizeof charnames/sizeof(char *);c++)
	if (charnames[c]
	    && (0==strcasecmp(charnames[c], CHARS(tok_buf))))
	  return MAKICHR(charnums[c]);
      if (loc_charsharp && NIMP(*loc_charsharp)) {
	resizuve(tok_buf, MAKINUM(j));
	p = apply(*loc_charsharp, tok_buf, listofnull);
	if (ICHRP(p)) return p;
      }
      wta(UNDEFINED, "unknown # object: #\\", CHARS(tok_buf));
    case '|':
      j = 1;	/* here j is the comment nesting depth */
    lp: c = lgetc(port);
    lpc:
      switch (c) {
      case EOF: wta(UNDEFINED, s_eofin, "balanced comment");
      case LINE_INCREMENTORS:
      default:
	goto lp;
      case '|':
	if ('#' != (c = lgetc(port))) goto lpc;
	if (--j) goto lp;
	break;
      case '#':
	if ('|' != (c = lgetc(port))) goto lpc;
	++j; goto lp;
      }
      goto tryagain;
    default: callshrp:
      {
	SCM reader =
#ifndef MEMOIZE_LOCALS
	  (3 & flgs) ? p_read_numbered :
#endif
	  ((4 & flgs) ? p_read_for_load : p_read);
	SCM args = cons2(MAKICHR(c), port, cons(reader, EOL));
	if ((4 & flgs) && loc_loadsharp && NIMP(*loc_loadsharp)) {
	  p = apply(*loc_loadsharp, args, EOL);
	  if (UNSPECIFIED==p) goto tryagain;
	  return p;
	} else if (loc_readsharp && NIMP(*loc_readsharp)) {
	  p = apply(*loc_readsharp, args, EOL);
	  if (UNSPECIFIED==p) goto tryagain;
	  return p;
	}
      }
    unkshrp: wta((SCM)MAKICHR(c), s_unknown_sharp, "");
    }
  case '\"':
    j = 0;
    while ('\"' != (c = lgetc(port))) {
      ASRTER(EOF != c, UNDEFINED, s_eofin, s_string);
      if (j+1 >= LENGTH(tok_buf)) grow_tok_buf(tok_buf);
      switch (c) {
      case LINE_INCREMENTORS: break;
      case '\\':
	switch (c = lgetc(port)) {
	case LINE_INCREMENTORS: continue;
	case '0': c = '\0'; break;
	case 'f': c = '\f'; break;
	case 'n': c = '\n'; break;
	case 'r': c = '\r'; break;
	case 't': c = '\t'; break;
	case 'a': c = '\007'; break;
	case 'v': c = '\v'; break;
	}
      }
      CHARS(tok_buf)[j] = c;
      ++j;
    }
    if (j==0) return nullstr;
    CHARS(tok_buf)[j] = 0;
    return makfromstr(CHARS(tok_buf), j);
  case DIGITS:
  case '.': case '-': case '+':
  num:
    j = scm_read_token(c, tok_buf, port, flgs);
    p = istring2number(CHARS(tok_buf), (long)j, 10L);
    if (NFALSEP(p)) return p;
    if (c=='#') {
      if ((j==2) && (lgetc(port)=='(')) {
	lungetc('(', port);
	c = CHARS(tok_buf)[1];
	goto callshrp;
      }
      wta(UNDEFINED, s_unknown_sharp, CHARS(tok_buf));
    }
    goto tok;
  default:
    j = scm_read_token(c, tok_buf, port, flgs);
  tok:
    p = intern(CHARS(tok_buf), j);
    return CAR(p);
  }
}
