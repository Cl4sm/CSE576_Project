void scm_dynthrow(tocont, arg1, arg2, rest)
     SCM tocont;
     SCM arg1, arg2, rest;
{
  CONTINUATION *cont = CONT(tocont);
  if (cont->stkbse != CONT(rootcont)->stkbse)
    wta(tocont, &s_sstale[10], s_cont);
  dowinds(cont->other.dynenv);
  {
    DEFER_INTS;
#ifdef CHEAP_CONTINUATIONS
    scm_estk = cont->other.estk;
    scm_estk_ptr = cont->other.estk_ptr;
#else
    {
      SCM *to, *from =  VELTS(cont->other.estk);
      sizet n = LENGTH(cont->other.estk);
      if (LENGTH(scm_estk) < n) scm_estk_reset(n);
      to = VELTS(scm_estk);
      scm_estk_ptr = &(to[n - SCM_ESTK_FRLEN]);
      while(n--) to[n] = from[n];
    }
#endif
    scm_env = cont->other.stkframe[0];
    scm_env_tmp = cont->other.stkframe[1];
#ifndef RECKLESS
    scm_trace_env = cont->other.stkframe[2];
    scm_trace = cont->other.stkframe[3];
#endif
    if (!UNBNDP(arg2) && IM_VALUES_TOKEN == scm_env_tmp) {
      scm_env_cons(arg2, rest);
      arg2 = UNDEFINED;
    }
    ALLOW_INTS;
  }
  if (!UNBNDP(arg2)) return;	/* eval will signal wrong number of args */
  throw_to_continuation(cont, arg1, CONT(rootcont));
  wta(tocont, s_sstale, s_cont);
}
