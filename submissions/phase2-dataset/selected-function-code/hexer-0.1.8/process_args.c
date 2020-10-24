process_args(int argc, char **argv)
{
  int c, i;
  int exit_f = 0;
  char *first_buffer = 0;
  int open_f = 0;
#if HEXER_LONG_OPTIONS
  int longopt_idx;
#endif

  /* set default options */
  hexer_readonly = 0;

  for (; !exit_f;) {
    if (optind >= argc) break;
    if (*argv[optind] == '+') {
      if (!argv[optind][1])
        startup_commands[startup_commands_n++] = argv[++optind];
      else
        startup_commands[startup_commands_n++] = argv[optind] + 1;
      ++optind;
      continue;
    }
#if HEXER_LONG_OPTIONS
    c = getopt_long(argc, argv, shortopts, longopts, &longopt_idx);
#else
    c = getopt(argc, argv, shortopts);
#endif
    if (c < 0) break;
    switch (c) {
      case 'v':
      case 'R': /* readonly */
        hexer_readonly = 1;
	break;
      case 'r': /* recover */
        printf("recover from file `%s'.\n", optarg);
	break;
      case 'c': /* command */
        startup_commands[startup_commands_n++] = optarg;
	break;
      case 'd': /* debug */
        setbuf(stdout, 0);
        break;
      case 't': /* tite - turn off the ti/te sequence */
        ++tio_tite_f;
        break;
      case 'h': /* help */
      default:
        puts(usage_message);
	exit_f = 1;
	break;
    }
  }

  if (!exit_f) {
    if (optind < argc) /* read the files */
      while (optind < argc) {
        if (!he_open_buffer(argv[optind], argv[optind])) {
          open_f = 1;
          if (!first_buffer) first_buffer = argv[optind];
          if (hexer_readonly) he_set_buffer_readonly(argv[optind]);
        }
        ++optind;
      }
    if (!open_f) /* create an empty buffer */
      he_open_buffer("*scratch*", 0);
    if (first_buffer) he_select_buffer(first_buffer);
    /* execute the startup commands (if any).  some of the startup commands
     * may open or close buffers or even quit the editor, so we have to
     * check if `current_buffer' is 0 after each command. */
    hexer_init();
    if (startup_commands_n && current_buffer)
      for (i = 0; i < startup_commands_n; ++i) {
	// I think this is a false positive - current_buffer is global! :)
	// cppcheck-suppress nullPointer
        exh_command(current_buffer->hedit, startup_commands[i]);
        if (!current_buffer) break;
      }
    if (current_buffer) he_status_message(0);
  }
  return exit_f;
}
