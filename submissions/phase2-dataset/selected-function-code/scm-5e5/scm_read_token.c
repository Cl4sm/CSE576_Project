     int ic;
     SCM tok_buf;
     SCM port;
     int flgs;
{
  register sizet j = 1;
  register int c = ic;
  register char *p = CHARS(tok_buf);
  p[0] = '\\'==c ? lgetc(port) : 8 & flgs ? c : downcase[c];
  while(1) {
    if (j+1 >= LENGTH(tok_buf)) p = grow_tok_buf(tok_buf);
    switch (c = lgetc(port)) {
#ifdef BRACKETS_AS_PARENS
    case '[': case ']':
#endif
    case '(': case ')': case '\"': case ';':
    case ',': case '`':
      /* case '#': */
    case WHITE_SPACES:
    case LINE_INCREMENTORS:
      lungetc(c, port);
    case EOF:
      p[j] = 0;
      return j;
    case '\\':	/* slashified symbol */
      p[j++] = lgetc(port);
      break;
    default:
      p[j++] = 8 & flgs ? c : downcase[c];
    }
  }
}
