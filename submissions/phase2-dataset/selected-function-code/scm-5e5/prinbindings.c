static void prinbindings(names, inits, init_env,
			 steps, step_env, port, writing)
     SCM names, inits, init_env, steps, step_env, port;
     int writing;
{
  lputc('(', port);
  while (NIMP(names) && NIMP(inits)) {
    lputc('(', port);
    scm_iprin1(CAR(names), port, writing);
    lputc(' ', port);
    scm_princode(CAR(inits), init_env, port, writing);
    if (NIMP(steps)) {
      lputc(' ', port);
      scm_princode(CAR(steps), step_env, port, writing);
      steps = CDR(steps);
    }
    lputc(')', port);
    names = CDR(names);
    inits = CDR(inits);
    if (NIMP(names)) lputc(' ', port);
  }
  lputs(") ", port);
}
