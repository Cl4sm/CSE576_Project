{
  SCM file, env = err_env, obj = *loc_errobj;
  SCM linum = UNDEFINED;
  int badport = IMP(cur_errp) || !OPOUTPORTP(cur_errp);
  int writing = 2; /* Value of 2 used only for printing error messages */
  int codep = errobj_codep;
  DEFER_INTS;
  if (badport || (errjmp_recursive++)) {
    if (IMP(def_errp) || !OPOUTPORTP(def_errp)) exit(EXIT_FAILURE);
    lputs("RECURSIVE ERROR: ", def_errp);
    if (badport || TYP16(cur_errp)==tc16_sfport) {
      lputs("reverting from ", def_errp);
      scm_iprin1(cur_errp, def_errp, 2);
      lputs("to default error port\n", def_errp);
      cur_errp = def_errp;
      errjmp_recursive = 0;
    }
    else exit(EXIT_FAILURE);
  }
#ifdef SCM_ENV_FILENAME
  file = scm_env_getprop(SCM_ENV_FILENAME, env);
  if (NIMP(file)) file = CAR(file);
  else file = UNDEFINED;
#else
  file = BOOL_F;
#endif
  if (codep) obj = scm_check_linum(obj, &linum);
  err_exp = scm_check_linum(err_exp, UNBNDP(linum) ? &linum : 0L);
  err_head("ERROR");
  scm_err_line("ERROR: ", file, linum, cur_errp);
  if (err_s_subr && *err_s_subr) {
    lputs(err_s_subr, cur_errp);
    lputs(": ", cur_errp);
  }
  if (!err_pos) return;		/* Already been printed */
  if (err_pos==(char *)ARG1 && UNBNDP(obj)) err_pos = (char *)WNA;
#ifdef nosve
  if ((~0x1fL) & (short)err_pos) lputs(err_pos, cur_errp);
  else if (WNA > (short)err_pos) {
    lputs("Wrong type in arg", cur_errp);
    lputc((short)err_pos <= ARGn ? ' ' : '1' + (short)err_pos - ARG1, cur_errp);
  }
#else
  if ((~0x1fL) & (long)err_pos) lputs(err_pos, cur_errp);
  else if (WNA > (long)err_pos) {
    lputs("Wrong type in arg", cur_errp);
    lputc((long)err_pos <= ARGn ? ' ' : '1' + (int)err_pos - ARG1, cur_errp);
  }
#endif
  else lputs(errmsgs[((int)err_pos)-WNA].msg, cur_errp);
  lputs(((long)err_pos==WNA)?" given ":" ", cur_errp);
  err_pos = 0;
  if (!UNBNDP(obj))
    if (reset_safeport(sys_safep, 55, cur_errp))
      if (0==setjmp(SAFEP_JMPBUF(sys_safep))) {
	if (codep) scm_princode(obj, EOL, sys_safep, writing);
	else scm_iprin1(obj, sys_safep, writing);
      }
  if (UNBNDP(err_exp)) goto getout;
  if (NIMP(err_exp)) {
    if (reset_safeport(sys_safep, 55, cur_errp))
      if (0==setjmp(SAFEP_JMPBUF(sys_safep))) {
	lputs("\n; in expression: ", cur_errp);
	if (NCONSP(err_exp)) scm_princode(err_exp, env, sys_safep, writing);
	else if (UNDEFINED==CDR(err_exp))
	  scm_iprin1(CAR(err_exp), sys_safep, writing);
	else {
	  if (UNBNDP(env)) scm_iprlist("(... ", err_exp, ')', sys_safep, writing);
	  else scm_princode(err_exp, env, sys_safep, writing);
	}
      }
  }
  scm_scope_trace(env);
 getout:
#ifdef CAUTIOUS
  scm_stack_trace(UNDEFINED);
#endif
  lputc('\n', cur_errp);
  lfflush(cur_errp);
  err_exp = err_env = UNDEFINED;
  if (errjmp_bad) {
    lputs("\nFATAL ERROR DURING CRITICAL CODE SECTION: ", cur_errp);
    lputs(errjmp_bad, cur_errp);
    lputc('\n', cur_errp);
    lroom(BOOL_T);
#ifdef vms
    exit(EXIT_FAILURE);
#else
    exit(errno? (long)errno : EXIT_FAILURE);
#endif
  }
  errno = 0;
  ALLOW_INTS;
}
