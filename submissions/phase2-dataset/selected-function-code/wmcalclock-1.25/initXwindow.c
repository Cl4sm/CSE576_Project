
    int		 i;
    char	*display_name = NULL;

    for (i=1; argv[i]; ++i) {
        if (!strcmp(argv[i], "-display")) display_name = argv[i+1];
    }


    if (!(display = XOpenDisplay(display_name))) {
        fprintf(stderr, "%s: can't open display %s\n",
                argv[0], XDisplayName(display_name));
        exit(1);
    }


    screen  	 = DefaultScreen(display);
    Root    	 = RootWindow(display, screen);
    DisplayDepth = DefaultDepth(display, screen);
    x_fd    	 = XConnectionNumber(display);

}
