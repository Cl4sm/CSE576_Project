static SCM m_iqq(form, depth, env, ctxt)
     SCM form, env, ctxt;
     int depth;
{
  SCM tmp;
  int edepth = depth;
  if (IMP(form)) return form;
  if (VECTORP(form)) {
    long i = LENGTH(form);
    SCM *data = VELTS(form);
    tmp = EOL;
    for (;--i >= 0;) tmp = cons(data[i], tmp);
    tmp = m_iqq(tmp, depth, env, ctxt);
    for (i = 0; i < LENGTH(form); i++) {
      data[i] = CAR(tmp);
      tmp = CDR(tmp);
    }
    return form;
  }
  if (NCONSP(form)) {
#ifdef MACRO
    while M_IDENTP(form) form = IDENT_PARENT(form);
#endif
    return form;
  }
 form = scm_check_linum(form, 0L); /* needed? */
  tmp = scm_check_linum(CAR(form), 0L);
  if (NIMP(tmp)) {
    if (IDENTP(tmp)) {
#ifdef MACRO
      while M_IDENTP(tmp) tmp = IDENT_PARENT(tmp);
#endif
      if (i_quasiquote==tmp && TOPLEVELP(CAR(form), env)) {
	depth++;
	if (0==depth) tmp = IM_QUASIQUOTE;
	goto label;
      }
      else if (i_unquote==tmp && TOPLEVELP(CAR(form), env)) {
	--depth;
	if (0==depth) tmp = IM_UNQUOTE;
      label:
	form = CDR(form);
	ASRTER(NIMP(form) && ECONSP(form) && NULLP(CDR(form)),
	       form, ARG1, s_quasiquote);
	if (0!=depth)
	  form = cons(m_iqq(CAR(form), depth, env, ctxt), EOL);
	return cons(tmp, form);
      }
    }
    else {
      if (TOPDENOTE_EQ(i_uq_splicing, CAR(tmp), env)) {
	if (0==--edepth)
	  return cons(cons(IM_UQ_SPLICING, CDR(tmp)),
		      m_iqq(CDR(form), depth, env, ctxt));
      }
      tmp = m_iqq(tmp, edepth, env, ctxt);
    }
  }
  return cons(tmp, m_iqq(CDR(form), depth, env, ctxt));
}
