void init_eval()
{
  scm_env = EOL;
  scm_env_tmp = UNSPECIFIED;
#ifndef RECKLESS
  scm_trace = BOOL_F;
  scm_trace_env = EOL;
#endif
  tc16_promise = newsmob(&promsmob);
  tc16_macro = newsmob(&macrosmob);
  tc16_env = newsmob(&envsmob);
  init_iprocs(subr1s, tc7_subr_1);
  init_iprocs(subr2s, tc7_subr_2);
  init_iprocs(lsubr2s, tc7_lsubr_2);
  init_iprocs(subr3s, tc7_subr_3);
#ifdef SCM_PROFILE
  make_subr("scm:profile", tc7_subr_1o, scm_profile);
#endif
  make_specfun(s_apply, tc16_apply, 0);
  make_specfun(s_call_cc, tc16_call_cc, 0);
  make_specfun(s_eval, tc16_eval, 0);
  make_specfun(s_values, tc16_values, 0);
  make_specfun(s_call_wv, tc16_call_wv, 0);
  add_feature(s_values);

  i_dot = CAR(sysintern(".", UNDEFINED));
  i_arrow = CAR(sysintern("=>", UNDEFINED));
  i_else = CAR(sysintern("else", UNDEFINED));
  i_unquote = CAR(sysintern("unquote", UNDEFINED));
  i_uq_splicing = CAR(sysintern("unquote-splicing", UNDEFINED));
  i_quasiquote = make_synt(s_quasiquote, MAC_MMACRO, m_quasiquote);
  i_define = make_synt(s_define, MAC_MMACRO, m_define);
  make_synt(s_delay, MAC_MMACRO, m_delay);

  i_bind = CAR(sysintern("bind", UNDEFINED));
  i_anon = CAR(sysintern("<anon>", UNDEFINED));
  i_side_effect = CAR(sysintern("side-effect", UNDEFINED));
  i_test = CAR(sysintern("test", UNDEFINED));
  i_procedure = CAR(sysintern("procedure", UNDEFINED));
  i_argument = CAR(sysintern("argument", UNDEFINED));
  i_check_defines = CAR(sysintern("check-defines", UNDEFINED));
  loc_atcase_aux = &CDR(sysintern("@case-aux", UNDEFINED));

  /* acros */
  make_synt(s_definedp, MAC_ACRO, definedp);
  /* end of acros */

  make_synt(s_and, MAC_MMACRO, m_and);
  i_begin = make_synt(s_begin, MAC_MMACRO, m_begin);
  make_synt(s_case, MAC_MMACRO, m_case);
  make_synt(s_cond, MAC_MMACRO, m_cond);
  make_synt(s_do, MAC_MMACRO, m_do);
  make_synt(s_if, MAC_MMACRO, m_if);
  i_lambda = make_synt(s_lambda, MAC_MMACRO, m_lambda);
  i_let = make_synt(s_let, MAC_MMACRO, m_let);
  make_synt(s_letrec, MAC_MMACRO, m_letrec);
  make_synt(s_letstar, MAC_MMACRO, m_letstar);
  make_synt(s_or, MAC_MMACRO, m_or);
  i_quote = make_synt(s_quote, MAC_MMACRO, m_quote);
  make_synt(s_set, MAC_MMACRO, m_set);
  make_synt(s_atapply, MAC_MMACRO, m_apply);
  /*  make_synt(s_atcall_cc, MAC_MMACRO, m_cont); */
#ifdef MAC_INLINE
  make_synt("@inline-lambda", MAC_MMACRO, m_inline_lambda);
#endif
#ifdef MACRO
  tc16_ident = newsmob(&idsmob);
  make_subr(s_renamed_ident, tc7_subr_2, renamed_ident);
  make_synt(s_syn_quote, MAC_MMACRO, m_syn_quote);
  make_synt(s_defsyntax, MAC_MMACRO, m_defsyntax);
  make_synt(s_let_syntax, MAC_MMACRO, m_let_syntax);
  make_synt(s_letrec_syntax, MAC_MMACRO, m_letrec_syntax);

  make_synt(s_the_macro, MAC_ACRO, m_the_macro);
  add_feature("primitive-hygiene");
#endif

  f_begin = CDR(CDR(KEYWORD_MACRO(sym2vcell(i_begin))));
  f_define = CDR(CDR(KEYWORD_MACRO(sym2vcell(i_define))));

  list_unspecified = cons(UNSPECIFIED, EOL);
  f_evapply = cons(IM_EVAL_FOR_APPLY, EOL);
#ifdef SCM_ENV_FILENAME
  eval_env = scm_env_addprop(SCM_ENV_FILENAME,
			     CAR(sysintern("eval", UNDEFINED)),
			     EOL);
#else
  eval_env = EOL;
#endif
  f_apply_closure = scm_evstr("(let ((ap apply)) (lambda (p . a) (ap p a)))");
}
