     SCM port, pos;
{
  ASRTER(NIMP(port) && OPFPORTP(port), port, ARG1, s_file_position);
  if (UNBNDP(pos) || FALSEP(pos)) {
    long ans;
    SYSCALL(ans = ftell(STREAM(port)););
    if (ans < 0) return BOOL_F;
    if (CRDYP(port)) ans--;
    return MAKINUM(ans);
  }
  ASRTER((INUMP(pos) && (INUM(pos) >= 0))
	 || (NIMP(pos) && (TYP16(pos)==tc16_bigpos)),
	 port, ARG2, s_file_position);
#ifndef RECKLESS
  if (TRACKED & SCM_PORTFLAGS(port)) {
    if (INUM0==pos) {
      int i = SCM_PORTNUM(port);
      scm_port_table[i].line = 1L;
      scm_port_table[i].col = 1;
    }
    else {
      if (2 <= verbose)
	scm_warn("Setting file position for tracked port: ", "", port);
      SCM_PORTFLAGS(port) &= (~TRACKED);
    }
  }
#endif
  {
    int ans;
    CLRDY(port);		/* Clear ungetted char */
    SYSCALL(ans = fseek(STREAM(port), INUM(pos), 0););
#ifdef HAVE_PIPE
# ifdef ESPIPE
    if (!OPIOPORTP(port))
      ASRTER(ESPIPE != errno, port, ARG1, s_file_position);
# endif
#endif
    return ans ? BOOL_F : BOOL_T;
  }
}
