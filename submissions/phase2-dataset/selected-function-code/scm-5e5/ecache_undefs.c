     SCM x;
{
  static SCM argv[10] = {UNDEFINED, UNDEFINED, UNDEFINED,
                         UNDEFINED, UNDEFINED, UNDEFINED,
                         UNDEFINED, UNDEFINED, UNDEFINED,
                         UNDEFINED};

  int imax = sizeof(argv)/sizeof(SCM);
  int i = 0;

  scm_env_tmp = EOL;
  while NIMP(x) {
    if (imax==i) {
      ecache_undefs(x);
      break;
    }
    i++;
    x = CDR(x);
  }
  ENV_V2LST((long)i, argv);
}
