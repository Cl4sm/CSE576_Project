static void parse_arguments(int argc, char **argv) {
    int i, integer;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            print_help(argv[0]), exit(0);
        } else if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")) {
            printf("%s version %s\n", PACKAGE, VERSION), exit(0);
        } else if (!strcmp(argv[i], "--display") || !strcmp(argv[i], "-d")) {
            display_name = argv[i + 1];
            i++;
        } else if (!strcmp(argv[i], "--backlight") || !strcmp(argv[i], "-bl")) {
            backlight = LIGHTON;
        } else if (!strcmp(argv[i], "--light-color") || !strcmp(argv[i], "-lc")) {
            if (argc == i + 1)
                fprintf(stderr, "%s: argument \"%s\" needs an option.\n", argv[0], argv[i]), exit(1);
            SET_STRING(light_color, argv[i + 1]);
            i++;
        } else if (!strcmp(argv[i], "--windowed") || !strcmp(argv[i], "-w")) {
            dockapp_iswindowed = True;
        } else if (!strcmp(argv[i], "--broken-wm") || !strcmp(argv[i], "-bw")) {
            dockapp_isbrokenwm = True;
        } else if (!strcmp(argv[i], "--no-blink") || !strcmp(argv[i], "-nb")) {
            switch_authorized = False;
        } else if (!strcmp(argv[i], "--h12") || !strcmp(argv[i], "-12")) {
            h12 = True;
        } else if (!strcmp(argv[i], "--time-mode") || !strcmp(argv[i], "-tm")) {
            if (argc == i + 1)
                fprintf(stderr, "%s: error parsing argument for option %s\n",
                        argv[0], argv[i]), exit(1);
            if (sscanf(argv[i + 1], "%i", &integer) != 1)
                fprintf(stderr, "%s: error parsing argument for option %s\n",
                        argv[0], argv[i]), exit(1);
            if ((integer < 0) || (integer > 2))
                fprintf(stderr, "%s: argument %s must be in [0,2]\n",
                        argv[0], argv[i]), exit(1);
            time_mode = integer;
            i++;
        } else if (!strcmp(argv[i], "--alarm-cmd") || !strcmp(argv[i], "-c")) {
            SET_STRING(command, argv[i + 1]);
            i++;
        } else if (!strcmp(argv[i], "--message-cmd") || !strcmp(argv[i], "-mc")) {
            SET_STRING(msgcmd, argv[i + 1]);
            i++;
        } else if (!strcmp(argv[i], "--alarm") || !strcmp(argv[i], "-a")) {
            alrm_add(&alarms, argv[i + 1]);
            i++;
        } else if (!strcmp(argv[i], "--cfg-file") || !strcmp(argv[i], "-f")) {
            SET_STRING(config_file, argv[i + 1]);
            i++;
        } else if (!strcmp(argv[i], "--no-locale") || !strcmp(argv[i], "-nl")) {
            use_locale = False;
        } else if (!strcmp(argv[i], "--style") || !strcmp(argv[i], "-s")) {
            SET_STRING(style_name, argv[i + 1]);
            i++;
        } else if (!strcmp(argv[i], "--style-dir") || !strcmp(argv[i], "-sd")) {
            SET_STRING(style_dir, argv[i + 1]);
            i++;
        } else if (!strcmp(argv[i], "--label") || !strcmp(argv[i], "-l")) {
            SET_STRING(label, argv[i + 1]);
            i++;
        } else if (!strcmp(argv[i], "--show-cal") || !strcmp(argv[i], "-sc")) {
            showcal = True;
        } else if (!strcmp(argv[i], "--cal-alrm") || !strcmp(argv[i], "-ca")) {
            calalrms = True;
            load_calalrms();
        } else {
            fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], argv[i]);
            print_help(argv[0]), exit(1);
        }
    }
}