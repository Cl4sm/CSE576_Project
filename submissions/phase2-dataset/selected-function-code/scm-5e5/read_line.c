     SCM port;
{
  register int c;
  register int j = 0;
  sizet len = 30;
  SCM tok_buf = makstr((long) len);
  register char *p = CHARS(tok_buf);
  if (UNBNDP(port)) port = cur_inp;
  else ASRTER(NIMP(port) && OPINPORTP(port), port, ARG1, s_read_line);
  if (EOF==(c = lgetc(port))) return EOF_VAL;
  while(1) {
    switch (c) {
    case LINE_INCREMENTORS:
    case EOF:
      if (j>0 && '\r'==p[j-1]) j--;
      if (len==j) return tok_buf;
      return resizuve(tok_buf, (SCM)MAKINUM(j));
    default:
      if (j >= len) {
	p = grow_tok_buf(tok_buf);
	len = LENGTH(tok_buf);
      }
      p[j++] = c;
      c = lgetc(port);
    }
  }
}
