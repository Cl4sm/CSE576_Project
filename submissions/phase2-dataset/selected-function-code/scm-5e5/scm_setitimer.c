SCM scm_setitimer(which, value, interval)
     SCM which, value, interval;
{
  struct itimerval tval, oval;
  int w;
  int i = sizeof(setitimer_tab)/sizeof(setitimer_tab[0]);
  while (i--) {
    if (which==setitimer_tab[i].sym) {
      w = setitimer_tab[i].which;
      if (BOOL_T==value)
	SYSCALL(w = getitimer(w, &oval););
      else {
	if (BOOL_F==value) value = INUM0;
	ASRTER(INUMP(value), value, ARG2, s_setitimer);
	if (BOOL_F==interval) interval = INUM0;
	ASRTER(INUMP(interval), interval, ARG3, s_setitimer);
	tval.it_value.tv_sec = INUM(value) / 1000;
	tval.it_value.tv_usec = (INUM(value) % 1000)*1000;
	tval.it_interval.tv_sec = INUM(interval) / 1000;
	tval.it_interval.tv_usec = (INUM(interval) % 1000)*1000;
	SYSCALL(w = setitimer(w, &tval, &oval););
      }
      if (w) return BOOL_F;
      return cons2(MAKINUM(oval.it_value.tv_usec/1000 +
			   oval.it_value.tv_sec*1000),
		   MAKINUM(oval.it_interval.tv_usec/1000 +
			   oval.it_interval.tv_sec*1000),
		   EOL);
    }
  }
  return BOOL_F;
}
