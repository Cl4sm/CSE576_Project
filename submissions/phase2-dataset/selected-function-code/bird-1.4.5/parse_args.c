static void
parse_args(int argc, char **argv)
{
  int c;

  bird_name = get_bird_name(argv[0], "bird");
  if (argc == 2)
    {
      if (!strcmp(argv[1], "--version"))
	{
	  fprintf(stderr, "BIRD version " BIRD_VERSION "\n");
	  exit(0);
	}
      if (!strcmp(argv[1], "--help"))
	usage();
    }
  while ((c = getopt(argc, argv, opt_list)) >= 0)
    switch (c)
      {
      case 'c':
	config_name = optarg;
	break;
      case 'd':
	debug_flag |= 1;
	break;
      case 'D':
	log_init_debug(optarg);
	debug_flag |= 2;
	break;
      case 'p':
	parse_and_exit = 1;
	break;
      case 's':
	path_control_socket = optarg;
	break;
      case 'P':
	pid_file = optarg;
	break;
      case 'u':
	use_user = optarg;
	break;
      case 'g':
	use_group = optarg;
	break;
      case 'f':
	run_in_foreground = 1;
	break;
      case 'R':
	graceful_restart_recovery();
	break;
      default:
	usage();
      }
  if (optind < argc)
    usage();
}
