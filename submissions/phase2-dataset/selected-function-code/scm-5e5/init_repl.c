void init_repl( iverbose )
     int iverbose;
{
	sysintern(s_ccl, MAKINUM(CHAR_CODE_LIMIT));
	i_repl = CAR(sysintern("repl", UNDEFINED));
	loc_errobj = &CDR(sysintern("errobj", UNDEFINED));
	loc_loadpath = &CDR(sysintern("*load-pathname*", BOOL_F));
	loc_loadsharp = &CDR(sysintern("load:sharp", UNDEFINED));
	loc_readsharp = &CDR(sysintern("read:sharp", UNDEFINED));
	loc_charsharp = &CDR(sysintern("char:sharp", UNDEFINED));
	loc_broken_pipe = &CDR(sysintern("broken-pipe", UNDEFINED));
	scm_verbose = iverbose;
	init_iprocs(subr0s, tc7_subr_0);
	init_iprocs(subr1os, tc7_subr_1o);
	init_iprocs(subr1s, tc7_subr_1);
	init_iprocs(subr2os, tc7_subr_2o);
	add_feature(s_char_readyp);
	make_subr(s_swapcar, tc7_subr_2, swapcar);
	make_subr(s_wfi, tc7_lsubr, wait_for_input);
#ifndef MEMOIZE_LOCALS
	p_read_numbered =
	  make_subr(s_read_numbered, tc7_subr_1, scm_read_numbered);
#endif
	p_read_for_load =
	  make_subr(s_read_for_load, tc7_subr_1, scm_read_for_load);
	p_read =
	  make_subr(s_read, tc7_subr_1o, scm_read);
	i_eval_string = CAR(sysintern(s_eval_string, UNDEFINED));
	i_load_string = CAR(sysintern(s_load_string, UNDEFINED));
	scm_ldstr("\n\
(define file-set-position file-position)\n\
");
#ifdef CAN_DUMP
	add_feature("dump");
	scm_ldstr("\
(define (dump file . thunk)\n\
  (cond ((null? thunk) (set! *interactive* #f) (set! *argv* #f))\n\
	((not (car thunk)) (set! *argv* #f))\n\
	((boolean? (car thunk)))\n\
	(else (set! boot-tail (lambda (t) ((car thunk))))))\n\
  (set! restart exec-self)\n\
  (require #f)\n\
  (unexec file))\n\
");
#endif
#ifdef ARM_ULIB
	set_erase();
#endif
	tc16_arbiter = newsmob(&arbsmob);
}
