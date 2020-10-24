
int  i;
  
    for (i = 1; i < argc; i++) {

        if (!strcmp(argv[i], "-display")){

            ++i;

        } else if (!strcmp(argv[i], "-jazz")){

	    UseJazzPoster = 1;

        } else if (!strcmp(argv[i], "-arial")){

	    UseArial = 1;

        } else if (!strcmp(argv[i], "-tekton")){

	    UseTekton = 1;

        } else if (!strcmp(argv[i], "-luggerbug")){

	    UseLuggerbug = 1;

        } else if (!strcmp(argv[i], "-comicsans")){

	    UseComicSans = 1;

        } else if (!strcmp(argv[i], "-tc")){

            if ((i+1 >= argc)||(argv[i+1][0] == '-')) {
                fprintf(stderr, "wmCalClock: No color found\n");
                print_usage();
                exit(-1);
            }
            strcpy(TimeColor, argv[++i]);

        } else if (!strcmp(argv[i], "-bc")){

            if ((i+1 >= argc)||(argv[i+1][0] == '-')) {
                fprintf(stderr, "wmCalClock: No color found\n");
                print_usage();
                exit(-1);
            }
            strcpy(BackgroundColor, argv[++i]);

        } else if (!strcmp(argv[i], "-24")){

	    Show24HourTime = 1;

        } else if (!strcmp(argv[i], "-b")){

            if ((i+1 >= argc)||(argv[i+1][0] == '-')) {
                fprintf(stderr, "wmCalClock: No volume given\n");
                print_usage();
                exit(-1);
            }
	    Beep = 1;
	    Volume = atoi(argv[++i]);

        } else if (!strcmp(argv[i], "-e")){

            if ((i+1 >= argc)||(argv[i+1][0] == '-')) {
                fprintf(stderr, "wmCalClock: No command given\n");
                print_usage();
                exit(-1);
            }
	    strcpy(ExecuteCommand, argv[++i]);
	    HasExecute = 1;

        } else if (!strcmp(argv[i], "-g")){

	    ShowGreenwichTime = 1;

        } else if (!strcmp(argv[i], "-S")){

	    ShowSeconds = 0;

        } else if (!strcmp(argv[i], "-s")){

	    ShowSiderealTime = 1;
	    Longitude = 0.0;

        } else if (!strcmp(argv[i], "-L")){

            if ((i+1 >= argc)||(argv[i+1][0] == '-')) {
                fprintf(stderr, "wmCalClock: No longitude given\n");
                print_usage();
                exit(-1);
            }
	    ShowSiderealTime = 1;
	    Longitude = atof(argv[++i]);

        } else if (!strcmp(argv[i], "-l")){

	    UseLowColorPixmap = 1;

        } else {

	    print_usage();
            exit(1);
	}

    }
    
    if (!ShowSeconds && !UseArial && !UseJazzPoster 
	&& !UseComicSans && !UseLuggerbug) UseTekton = 1;



}
