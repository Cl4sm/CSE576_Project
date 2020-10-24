int lgetc(port)
  SCM port;
{
  FILE *f;
  int c;
  int i, j = -1;
  if (CRDY & CAR(port)) {
    j = SCM_PORTNUM(port);
    c = scm_port_table[j].unread;
    if (c != EOF) {
      scm_port_table[j].unread = EOF;
      CAR(port) &= (scm_port_table[j].flags | (~0xf0000)); /* CLRDY(port) */
      return c;
    }
  }
  f = STREAM(port);
  i = PTOBNUM(port);
#ifdef linux
  c = (ptobs[i].fgetc)(f);
#else
  SYSCALL(c = (ptobs[i].fgetc)(f););
#endif
  if (j > -1)  {
    /* This means that CRDY is set, note that CRDY is overloaded */
    switch (c) {
    case LINE_INCREMENTORS:
      scm_port_table[j].line++;
      scm_port_table[j].colprev = scm_port_table[j].col;
      scm_port_table[j].col = 1;
      break;
    default:
      scm_port_table[j].col++;
    }
  }
  return c;
}
