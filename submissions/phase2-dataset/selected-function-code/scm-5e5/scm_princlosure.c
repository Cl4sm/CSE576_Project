void scm_princlosure(proc, port, writing)
     SCM proc, port;
     int writing;
{
  SCM env, linum = UNDEFINED;
  proc = CODE(proc);
  lputs("#<CLOSURE ", port);
  env = CAR(proc);
#ifdef CAUTIOUS
  if (NIMP(env=scm_env_getprop(SCM_ENV_PROCNAME, env))) {
    scm_iprin1(CAR(env), port, 1);
    lputc(' ', port);
    env = CDR(env);
    if (NIMP(env) && SCM_LINUMP(CAR(env)))
      linum = CAR(env);
  }
  else
    lputs("<anon> ", port);
  env = CAR(proc);
  if (NIMP(env=scm_env_getprop(SCM_ENV_FILENAME, env)))
    scm_line_msg(CAR(env), linum, port);
#endif
  env = CAR(proc);
  scm_iprin1(SCM_ENV_FORMALS(env), port, writing);
  if (writing) {
    for (proc = CDR(proc); NIMP(proc); proc = CDR(proc)) {
      lputc(' ', port);
      scm_princode(CAR(proc), env, port, writing);
    }
  }
  lputc('>', port);
}
