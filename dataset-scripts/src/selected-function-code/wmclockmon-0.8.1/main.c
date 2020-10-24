int main(int argc, char **argv) {
    XEvent event;
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;
#ifdef SA_NOCLDWAIT
    sa.sa_flags = SA_NOCLDWAIT;
#else
    sa.sa_flags = 0;
#endif
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, NULL);

    /* Set default for style dir : */
    SET_STRING(style_dir, DATADIR);

    /* Init time */
    time_update();

    /* Load default configuration file */
    if (! config_file) {
        char *Home = robust_home();
        config_file = xmalloc(strlen(Home) + strlen(DEFAULT_CFGFILE) + 2);
        sprintf(config_file, "%s/%s", Home, DEFAULT_CFGFILE);
    }
    load_cfgfile();
    FREE(config_file);

    /* Parse CommandLine */
    parse_arguments(argc, argv);

    if (! config_file) {
        char *Home = robust_home();
        config_file = xmalloc(strlen(Home) + strlen(DEFAULT_CFGFILE) + 2);
        sprintf(config_file, "%s/%s", Home, DEFAULT_CFGFILE);
    } else {
        load_cfgfile();
    }
    style = default_style;
    load_style(style_name);

    if (use_locale) {
        setlocale(LC_TIME, "");
        setlocale(LC_CTYPE, "");
        setlocale(LC_COLLATE, "");
    }

    /* Initialize Application */
    dockapp_open_window(display_name, argv[0], SIZE, SIZE, argc, argv);
    dockapp_set_eventmask(ButtonPressMask);

    graphics_init();

    if (showcal) show_cal();

    /* Main loop */
    while (1) {
        if (dockapp_nextevent_or_timeout(&event, update_interval * 1000 - 10)) {
            /* Next Event */
            switch (event.type) {
                case ButtonPress:
                    control(event.xbutton.button,
                            event.xbutton.x,
                            event.xbutton.y,
                            event.xbutton.state);
                    break;
                default: break;
            }
        } else {
            /* Time Out */
            update();
        }
    }

    return 0;
}