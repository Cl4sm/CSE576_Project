static SCM scm_lread_rec(tok_buf, port)
     SCM tok_buf;
     SCM port;
{
  SCM line, form;
  int c = flush_ws(port);
  switch (c) {
  default:
    lungetc(c, port);
    line = scm_port_line(port);
    form = scm_lreadpr(tok_buf, port, 5);
    if (NFALSEP(line) && NIMP(form) &&
	(CONSP(form) || VECTORP(form))) {
      return cons(SCM_MAKE_LINUM(INUM(line)), form);
    }
    return form;
#ifdef BRACKETS_AS_PARENS
  case ']':
#endif
  case ')': return UNDEFINED;
  case EOF: return EOF_VAL;
  }
}
