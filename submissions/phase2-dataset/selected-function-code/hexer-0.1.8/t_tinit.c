  static void
t_tinit()
  /* initialize the global variables `t_PC' and `t_speed';
   * called by `tgetent()'.
   */
{
  static char *t_tgetwhatever( /* char * */ );
  char *pc = t_tgetwhatever("pc");
  struct termios ts;
  int cfspeed;

  if (pc) { /* set the padding character */
    if (*pc++ != '=')
      t_PC = 0;
    else {
      if (*pc == '\\') {
        switch (*++pc) {
	  case 'E':
	  case 'e': t_PC = '\e'; break;
	  case 'n': t_PC = '\n'; break;
	  case 'r': t_PC = '\r'; break;
	  case 't': t_PC = '\t'; break;
	  case 'b': t_PC = '\b'; break;
	  case 'f': t_PC = '\f'; break;
	  case '0': case '1': case '2': case '3': case '4':
	  case '5': case '6': case '7':  /* octal escapes */
	    for (t_PC = 0; *pc >= '0' && *pc <= '7'; ++pc)
	      t_PC <<= 3, t_PC = *pc - '0';
	    break;
	  default: t_PC = *pc;
        }
      } else if (*pc == '^')
        t_PC = *pc & 0x1f;
      else
        t_PC = *pc;
    }
  } else /* default padding character '\0' */
    t_PC = 0;
  
  /* get the line speed */
  tcgetattr(0, &ts);
  cfspeed = cfgetospeed(&ts);
  if (cfspeed <= 13)
    t_speed = speeds[cfspeed];
  else { /* these are beyond the POSIX spec. */
    switch (cfspeed) {
#ifdef B19200
      case B19200: t_speed = 19200; break;
#else
#ifdef EXTA
      case EXTA: t_speed = 19200; break;
#endif
#endif
#ifdef B38400 
      case B38400: t_speed = 38400; break;
#else
#ifdef EXTB
      case EXTB: t_speed = 38400; break;
#endif
#endif
#ifdef B57600
      case B57600: t_speed = 57600; break;
#endif
#ifdef B115200
      case B115200: t_speed = 115200; break;
#endif
#ifdef B230400
      case B230400: t_speed = 230400; break;
#endif
      default: t_speed = 0; /* hmm... */
    }
  }
}
