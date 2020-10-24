void init_features()
{
  loc_features = &CDR(sysintern("slib:features", EOL));
  init_iprocs(subr0s, tc7_subr_0);
  init_iprocs(subr1s, tc7_subr_1);
  make_subr(s_execpath, tc7_subr_1o, scm_execpath);
#ifdef SIGALRM
# ifdef SIGPROF
  make_subr(s_setitimer, tc7_subr_3, scm_setitimer);
#  ifdef ITIMER_REAL
  setitimer_tab[0].sym = CAR(sysintern("real", UNDEFINED));
  setitimer_tab[0].which = ITIMER_REAL;
#  endif
#  ifdef ITIMER_VIRTUAL
  setitimer_tab[1].sym = CAR(sysintern("virtual", UNDEFINED));
  setitimer_tab[1].which = ITIMER_VIRTUAL;
#  endif
#  ifdef ITIMER_PROF
  setitimer_tab[2].sym = CAR(sysintern("profile", UNDEFINED));
  setitimer_tab[2].which = ITIMER_PROF;
#  endif
# endif
#endif
#ifdef TICKS
  loc_tick_signal = &CDR(sysintern("ticks-interrupt", UNDEFINED));
  make_subr(s_ticks, tc7_subr_1o, lticks);
#endif
#ifdef RECKLESS
  add_feature("reckless");
#endif
#ifndef _Windows
  add_feature(s_system);
#endif
#ifdef vms
  add_feature(s_ed);
#endif
  sysintern("*scm-version*", CAR(sysintern(SCMVERSION, UNDEFINED)));
}
