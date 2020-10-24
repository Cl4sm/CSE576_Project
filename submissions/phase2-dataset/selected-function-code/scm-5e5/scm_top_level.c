SCM scm_top_level(initpath, toplvl_fun)
     char *initpath;
     SCM (*toplvl_fun)();
{
  SCM ret;
#ifdef _UNICOS
  int i;
#else
  long i;
#endif
  if (!toplvl_fun) toplvl_fun = repl;
  CONT(rootcont)->stkbse = (STACKITEM *)&i;
  i = setjump(CONT(rootcont)->jmpbuf);
#ifndef SHORT_INT
  if (i) i = UNCOOK(i);
#endif
 drloop:
  switch (PTR2INT(i)) {
  default:
    {
      char *name = errmsgs[i-WNA].s_response;
      if (name) {
	SCM proc = CDR(intern(name, (sizet)strlen(name)));
	if (NIMP(proc)) apply(proc, EOL, EOL);
      }}
    i = errmsgs[i-WNA].parent_err;
    if (i) goto drloop;
  case 1:			/* from everr() */
    def_err_response();
    goto reset_toplvl;
  case 0:
    exitval = MAKINUM(EXIT_SUCCESS);
    errjmp_bad = (char *)0;
    errjmp_recursive = 0;
    if (NIMP(sys_errp) && OPOUTPORTP(sys_errp)) lfflush(sys_errp);
    errno = 0;
    SIG_deferred = 0;
    deferred_proc = 0;
    ints_disabled = 0;
    scm_init_INITS();
    if (dumped) {
      lcells_allocated = cells_allocated;
      lmallocated = mallocated;
      rt = INUM(my_time());
      gc_time_taken = 0;
    }
    else if (initpath &&
	     (isspace(initpath[0]) || ';'==initpath[0] || '('==initpath[0]))
      scm_ldstr(initpath);
    else if (scm_ldfile(initpath ? initpath : "")) /* load Scheme init files */
      wta(*loc_errobj, "Could not open file", s_load);
    {
      SCM boot_tail = scm_evstr("boot-tail");
      /* initialization tail-call */
      if (NIMP(boot_tail))
        apply(boot_tail, (dumped ? makfrom0str(initpath) : BOOL_F), listofnull);
    }
  case -2:			/* abrt */
  reset_toplvl:
    ints_disabled = 1;
    errjmp_bad = (char *)0;
    errjmp_recursive = 0;
    if (NIMP(sys_errp) && OPOUTPORTP(sys_errp)) lfflush(sys_errp);
    SIG_deferred = 0;
    deferred_proc = 0;
    gc_hook_active = 0;
    scm_estk_reset(0);

    /* Closing the loading file turned out to be a bad idea. */
    /* But I will leave the code here in case someone wants it. */
#ifdef CLOSE_LOADING_PORTS_ON_ABORT
    if (NIMP(loadports) && OPINPORTP(CAR(loadports))) {
      if (scm_verbose > 1) {
	lputs("; Aborting load (closing): ", cur_errp);
	scm_display(*loc_loadpath, cur_errp);
	scm_newline(cur_errp);
      }
      close_port(CAR(loadports)); /* close loading file. */
    }
#endif

    *loc_loadpath = BOOL_F;
    loadports = EOL;
    ints_disabled = 0;
    dowinds(EOL);
    ret = toplvl_fun();		/* typically repl() */
    if (INUMP(ret)) exitval = ret;
    err_pos = (char *)EXIT;
    i = EXIT;
    goto drloop;		/* encountered EOF on stdin */
  case -1:			/* quit */
    dowinds(EOL);
    if (MAKINUM(EXIT_SUCCESS) != exitval) {
      lputs("; program args: ", cur_errp);
      scm_write(progargs, cur_errp);
      scm_newline(cur_errp);
    }
    return exitval;
  case -3:			/* restart. */
    dowinds(EOL);
    return 0;
#ifdef CAN_DUMP
  case -4:			/* dump */
    DEFER_INTS;
    scm_estk_reset(0);
    scm_egc();
    igc(s_unexec, BOOL_F);
    ALLOW_INTS;
    dumped = 1;
# ifdef linux
    sbrk(getpagesize());	/* The last few words of the .data segment
				   were not being mapped in for dumped
				   executables. */
# endif
    unexec(CHARS(*loc_errobj), execpath, 0, 0, 0);
    goto reset_toplvl;
#endif
  }
}
