     const char *s;
     SCM port;
{
  sizet i = PTOBNUM(port);
  ASRTER(s, INUM0, ARG1, "lputs");
  while (EOF==(ptobs[i].fputs)(s, STREAM(port)) &&
	 scm_io_error(port, "fputs"))
    ;
  if (CRDY & CAR(port)) {
    sizet j;
    i = SCM_PORTNUM(port);
    for (j = 0; s[j]; j++) {
      switch (s[j]) {
      case LINE_INCREMENTORS:
	scm_port_table[i].line++;
	scm_port_table[i].col = 1;
	break;
      default:
	scm_port_table[i].col++;
      }
    }
  }
}
