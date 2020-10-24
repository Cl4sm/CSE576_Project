SCM repl()
{
  SCM x;
  SCM env = EOL; /* scm_env_addprop(SCM_ENV_FILENAME, i_repl, EOL); */
  int c;
  if (OPINPORTP(cur_inp) && OPOUTPORTP(cur_outp)) {
    repl_report();
    while(1) {
      if (OPOUTPORTP(cur_inp)) { /* This case for curses window */
	lfflush(cur_outp);
	if (verbose) lputs(PROMPT, cur_inp);
	lfflush(cur_inp);
      }
      else {
	if (verbose) lputs(PROMPT, cur_outp);
	lfflush(cur_outp);
      }
      lcells_allocated = cells_allocated;
      scm_env_work = scm_ecache_index - scm_ecache_len;
      scm_egcs = scm_clo_moved = scm_stk_moved = 0;
      lmallocated = mallocated;
      x = scm_read_for_load(cur_inp);
      rt = INUM(my_time());
      scm_gcs = 0;
      gc_time_taken = 0;
      if (EOF_VAL==x) return MAKINUM(EXIT_SUCCESS);
      if (!CRDYP(cur_inp)) {	/* assure newline read (and transcripted) */
	if (EOF==(c = lgetc(cur_inp))) break;
	lungetc(c, cur_inp);
      }
#ifdef __HIGHC__
# define __MSDOS__
#endif
#ifdef __MSDOS__
      if ('\n' != CGETUN(cur_inp))
	if (OPOUTPORTP(cur_inp)) /* This case for curses window */
	  {lfflush(cur_outp); scm_newline(cur_inp);}
	else scm_newline(cur_outp);
#endif
      if (NIMP(x)) {
	x = CONSP(x) ?
	  scm_eval_values(x, env, (SCM)EOL) :
	  cons(EVAL(x, env, (SCM)EOL), EOL);
      }
      else
	x = cons(x, EOL);
      repl_report();
      if (IMP(x))
	{if (verbose > 2) lputs(";;no values\n", cur_outp);}
      else if (IMP(CDR(x))) {
	scm_iprin1(CAR(x), cur_outp, 1);
	lputc('\n', cur_outp);
      }
      else
	while (NIMP(x)) {
	  lputc(' ', cur_outp);
	  scm_iprin1(CAR(x), cur_outp, 1);
	  lputc('\n', cur_outp);
	  x = CDR(x);
	}
    }
  }
  return UNSPECIFIED;
}
