char **script_process_argv(argc, argv)
     int argc;
     const char **argv;
{
  int nargc = argc, argi = 1, nargi = 1;
  char *narg, **nargv;
  if (!(argc > 2 && script_meta_arg_P(argv[1]))) return 0L;
  if (!(nargv = (char **)malloc((1 + nargc) * sizeof(char *)))) return 0L;
  nargv[0] = argv[0];
  while (((argi+1) < argc) && (script_meta_arg_P(argv[argi]))) {
    FILE *f = fopen(argv[++argi], "r");
    if (f) {
      nargc--;		/* to compensate for replacement of '\\' */
      while (1) switch (getc(f)) {
      case EOF: return 0L;
      default: continue;
      case '\n': goto found_args;
      }
    found_args: while ((narg = script_read_arg(f)))
      if (!(nargv = (char **)realloc(nargv, (1 + ++nargc) * sizeof(char *))))
	return 0L;
      else nargv[nargi++] = narg;
      fclose(f);
      nargv[nargi++] = argv[argi++];
    }
  }
  while (argi <= argc) nargv[nargi++] = argv[argi++];
  return nargv;
}
