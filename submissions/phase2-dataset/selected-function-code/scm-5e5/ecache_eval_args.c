     SCM proc, arg1, arg2, arg3, x;
{
  SCM argv[3];
  argv[0] = arg1;
  argv[1] = arg2;
  argv[2] = arg3;
  if (NIMP(x))
    ecache_evalx(x);
  else
    scm_env_tmp = EOL;
  ENV_V2LST(3L, argv);
#ifndef RECKLESS
  proc = SCM_ENV_FORMALS(CAR(CODE(proc)));
  proc = CDR(proc);
  proc = CDR(proc);
  proc = CDR(proc);
  for (; NIMP(proc); proc=CDR(proc)) {
    if (IMP(x)) return 0;
    x = CDR(x);
  }
  if (NIMP(x)) return 0;
#endif
  return 1;
}
