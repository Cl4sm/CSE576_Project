{
  int curOpt;
  int optionIndex = 0;
  static struct option validOptions[] = {
    {"help", no_argument, 0, 'h'},
    {"history-file", required_argument, 0, 'i'},
    {"version", no_argument, 0, 'v'},
    {"withdraw", no_argument, 0, 'w'},
    {"advanced", no_argument, 0, 'a'},
    {0, 0, 0, 0}
  };

  bzero(historyFilename, MAXPATHLEN + 1);

  while ((curOpt = getopt_long(argc, argv, "hi:vwa", validOptions, &optionIndex)) > -1) {
    switch (curOpt) {
      case 'h':
        usage();
        exit(0);
      case 'i':
        strncpy(historyFilename, optarg, MAXPATHLEN);
        break;
      case 'v':
        printf("BBrun Version %s\n", VERSION);
        exit(0);
      case 'w':
        withdrawnMode = 1;
        break;
      case 'a':
        advancedMode = 1;
        break;
      case '?': 	// getopt_long has already printed an error message, so just exit out.  
        exit( -1);
      default:
	// getopt_long thought the option was fine, but we haven't coded a proper handler for it yet.
        fprintf(stderr, "Invalid parameter '%c'.\n", curOpt);
        exit( -1);
    }
  }

  if (historyFilename[0] == '\0') {
    strncat(historyFilename, getenv("HOME"), MAXPATHLEN - strlen(DEFAULT_HIST_FILENAME) - 1);
    strncat(historyFilename, DEFAULT_HIST_FILENAME, strlen(DEFAULT_HIST_FILENAME));
  }

  if (__DEBUG__) {
    fprintf(stderr, "parseArguments() is complete.\n");
    fprintf(stderr, "historyFilename is '%s'.\n", historyFilename);
  }
}
