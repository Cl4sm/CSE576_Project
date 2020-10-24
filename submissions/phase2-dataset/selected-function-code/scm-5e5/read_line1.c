     SCM str, port;
{
  register int c;
  register int j = 0;
  register char *p;
  sizet len;
  ASRTER(NIMP(str) && STRINGP(str), str, ARG1, s_read_line1);
  p = CHARS(str);
  len = LENGTH(str);
  if (UNBNDP(port)) port = cur_inp;
  else ASRTER(NIMP(port) && OPINPORTP(port), port, ARG2, s_read_line1);
  c = lgetc(port);
  if (EOF==c) return EOF_VAL;
  while(1) {
    switch (c) {
    case LINE_INCREMENTORS:
    case EOF:
      return MAKINUM(j);
    default:
      if (j >= len) {
	lungetc(c, port);
	return BOOL_F;
      }
      p[j++] = c;
      c = lgetc(port);
    }
  }
}
