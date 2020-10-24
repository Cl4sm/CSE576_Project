void lputc(c, port)
     int c;
     SCM port;
{
  sizet i = PTOBNUM(port);
  while (EOF==(ptobs[i].fputc)(c, STREAM(port)) &&
	 scm_io_error(port, "fputc"))
    ;
  if (CRDY & CAR(port)) {
    i = SCM_PORTNUM(port);
    switch (c) {
    case LINE_INCREMENTORS:
      scm_port_table[i].line++;
      scm_port_table[i].col = 1;
      break;
    default:
      scm_port_table[i].col++;
    }
  }
}
