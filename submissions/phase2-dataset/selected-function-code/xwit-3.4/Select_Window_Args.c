Window Select_Window_Args(rargc, argv)
     int *rargc;
     char **argv;
#define ARGC (*rargc)
{
	int nargc=1;
	int argc;
	char **nargv;
	Window w=0;

	nargv = argv+1; argc = ARGC;
#define OPTION argv[0]
#define NXTOPTP ++argv, --argc>0
#define NXTOPT if (++argv, --argc==0) usage()
#define COPYOPT nargv++[0]=OPTION, nargc++

	while (NXTOPTP) {
		if (!strcmp(OPTION, "-")) {
			COPYOPT;
			while (NXTOPTP)
			  COPYOPT;
			break;
		}
		if (!strcmp(OPTION, "-root")) {
			w=RootWindow(dpy, screen);
			continue;
		}
		if (!strcmp(OPTION, "-name")) {
			NXTOPT;
			w = Window_With_Name(dpy, RootWindow(dpy, screen),
					     OPTION);
			if (!w)
			  Fatal_Error("No window with name %s exists!",OPTION);
			continue;
		}
		if (!strcmp(OPTION, "-id")) {
			NXTOPT;
			w=0;
			sscanf(OPTION, "0x%lx", &w);
			if (!w)
			  sscanf(OPTION, "%lu", &w);
			if (!w)
			  Fatal_Error("Invalid window id format: %s.", OPTION);
			continue;
		}
		COPYOPT;
	}
	ARGC = nargc;
	
	return(w);
}