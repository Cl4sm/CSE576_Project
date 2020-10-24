     SCM pipestr, modes;
{
	FILE *f;
	register SCM z;
	ASRTER(NIMP(pipestr) && STRINGP(pipestr), pipestr, ARG1, s_op_pipe);
	ASRTER(NIMP(modes) && (STRINGP(modes) || SYMBOLP(modes)), modes, ARG2, s_op_pipe);
	NEWCELL(z);
	/* DEFER_INTS, SYSCALL, and ALLOW_INTS are probably paranoid here*/
	DEFER_INTS;
	ignore_signals();
	SCM_OPENCALL(f = popen(CHARS(pipestr), CHARS(modes)));
	unignore_signals();
	z = f ?
	  scm_port_entry(f, tc16_pipe,
			 OPN | (strchr(CHARS(modes), 'r') ? RDNG : WRTNG)) :
	  BOOL_F;
	ALLOW_INTS;
	return z;
}
