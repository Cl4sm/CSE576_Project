SCM close_port(port)
     SCM port;
{
	sizet i;
        SCM ret = UNSPECIFIED;
	ASRTER(NIMP(port) && PORTP(port), port, ARG1, s_close_port);
	if (CLOSEDP(port)) return UNSPECIFIED;
	i = PTOBNUM(port);
	DEFER_INTS;
	if (ptobs[i].fclose) {
          int r;
	  SYSCALL(r = (ptobs[i].fclose)(STREAM(port)););
          if (EOF == r)
            ret = BOOL_F;
          else
            ret = MAKINUM(r);
	}
	CAR(port) &= ~OPN;
	SCM_PORTFLAGS(port) &= ~OPN;
	/* Bash the old ptobnum with the closed port ptobnum.
	   This allows catching some errors cheaply. */
	SCM_SET_PTOBNUM(port, tc16_clport);
	ALLOW_INTS;
	return ret;
}
