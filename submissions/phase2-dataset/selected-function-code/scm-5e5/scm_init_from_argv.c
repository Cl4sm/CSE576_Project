void scm_init_from_argv(argc, argv, script_arg, iverbose, buf0stdin)
     int argc;
     const char * const *argv;
     char *script_arg;
     int iverbose;
     int buf0stdin;
{
  long i = 0L;
  int j = 0;
  if ((2 <= argc) && argv[1] && (0==strncmp("-a", argv[1], 2)))
    i = atol((0==argv[1][2] && 3 <= argc && argv[2]) ? argv[2] : &argv[1][2]);
  init_scm(iverbose, buf0stdin, (0 >= i) ? 0L : 1024L * i); /* size in kB */
  for (j = 0; argv[j]; j++) {
    if (0==strcmp(argv[j], "--no-symbol-case-fold")) {
      case_sensitize_symbols = 8;
      break;
    }
  }
  progargs = EOL;
  progargs = makfromstrs(argc, argv);
  sysintern("*script*", script_arg ? makfrom0str(script_arg) : BOOL_F);
}
