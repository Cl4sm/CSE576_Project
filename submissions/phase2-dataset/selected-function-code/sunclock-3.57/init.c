void init(int argc,char *argv[])
{
		int screen;
		XWMHints *wmh;
		XSizeHints *xsh;
		XClassHint *classh;
		XColor tmp;
		int i, x, y;

		/* Default settings */
		*ewin.name = '\0';
		amax = AMAX;
		bmax = BMAX;
		umax = UMAX;

		/* engine screen width, height, font */
		screen_height = DEFAULT_HEIGHT - 1;
		screen_width = DEFAULT_WIDTH;

#ifndef MINIMAL
		memset((void *)binding, 0, 26*sizeof(char *));

		for (i=1; i<argc; i++) {
			if (i<argc-1 && !strcasecmp(argv[i], "-rc"))
			RcFile = strdup(argv[++i]);
		}
		read_rc();
#endif /* MINIMAL */

		/* get command line options */
		for (i=1; i<argc; i++) {
			if ((i<argc-1) &&
				(argv[i][0]=='-'
#ifndef MINIMAL
|| argv[i][0]=='@'
#endif
				)) {
					parse_arg(argv[i], argv[i+1]);
					++i;
			} else {
				if (*ewin.name) break;
				/* case of syntax  'filename:linenumber' */
				{
                                    char* p=strchr(argv[i],':');
                                    if (p) {
                                         *p++ = '\0';
                                         ewin.jump = atoi(p);
                                    }
				}
				strcpy(ewin.name, argv[i]);
			}
		}

                /* alloc memory for the main buffer and block buffer */
		edbuf = (char *) malloc((size_t)(amax+tabsize+1));
                bb = (char *) malloc((size_t)(bmax+tabsize+1));
		unbuf = (void *) malloc((size_t)(umax+tabsize+1));

		if (!edbuf || !bb || !unbuf) {
			fprintf(stderr,"Memory allocation failed, aborting !!\n");
			exit(1);
                }

		/* open the display */
		dpy=XOpenDisplay(DisplayName);
		if(dpy==NULL)  {
				fprintf(stderr,"Can't open display: %s\n",DisplayName);
				sys_exit(1);
		}

		/* setup to gracefully respond to exit requests from X */
		screen=DefaultScreen(dpy);
		DeleteWindow = XInternAtom(dpy, "WM_DELETE_WINDOW", False);

		/* establish window manager hints data structure */
		xsh=XAllocSizeHints();
		wmh=XAllocWMHints();
		classh=XAllocClassHint();

		x = 1; y=0;

		/* setup font(s) */
		if(FontName == NULL) { FontName = FONTNAME; }

		font=XLoadQueryFont(dpy, FontName);
		if(font==NULL) {
				fprintf(stderr, FONTS_NOT_FOUND"\n");
				sys_exit(1);
		}

		fheight = font->ascent + font->descent;
		Height = fheight * screen_height + font->descent + 2;

		/* engine screen width */
		fwidth = XTextWidth(font,"8",1);

		Width = (fwidth * screen_width)+13;
		xsh->flags = PSize; xsh->width = Width; xsh->height = Height;

		/* initialize clreol string to all blanks */
		memset(eolbuf, ' ', sizeof(eolbuf));

		/* create the only window */
		win=XCreateSimpleWindow(dpy,RootWindow(dpy,screen), x, y,
                                Width, Height, 0,
				BlackPixel(dpy,screen),WhitePixel(dpy,screen));

		/* setup window hints */
		wmh->initial_state=NormalState;
		wmh->input=True;
		wmh->window_group = win;
		wmh->flags = StateHint | InputHint | WindowGroupHint;

		/* setup window class resource names */
		classh->res_name = (AppName==NULL)?EDIT:AppName;
		classh->res_class = "Xedit";

		/* name that window */
		XmbSetWMProperties(dpy, win, EDIT, EDIT, argv, argc,
										   xsh, wmh, classh);
		/* notify X on how to force emx to exit */
		XSetWMProtocols(dpy, win, &DeleteWindow, 1);

		/* specify accepted XEvent loop events */
		XSelectInput(dpy, win,
						KeyPressMask|\
						FocusChangeMask|\
						StructureNotifyMask|\
						ButtonPressMask|\
						ButtonReleaseMask|\
						ExposureMask|\
						PropertyChangeMask|\
						Button1MotionMask|\
						Button2MotionMask|\
						Button3MotionMask|\
						VisibilityChangeMask
		);
		keve = (XKeyEvent *)&event;

		/* create the Graphic Context for drawing purposes */
		gc=XCreateGC(dpy,win,0,NULL);

		/* allocate required colors */
		XAllocNamedColor(dpy,DefaultColormap(dpy,screen),FgColor,&FgXColor,&tmp);
		XAllocNamedColor(dpy,DefaultColormap(dpy,screen),BgColor,&BgXColor,&tmp);
		XAllocNamedColor(dpy,DefaultColormap(dpy,screen),CrColor,&CrXColor,&tmp);
		XAllocNamedColor(dpy,DefaultColormap(dpy,screen),HiFgColor,&HiFgXColor,&tmp);
		XAllocNamedColor(dpy,DefaultColormap(dpy,screen),HiBgColor,&HiBgXColor,&tmp);

		/* apply colors to window */
		XSetForeground(dpy,gc,FgXColor.pixel);
		XSetWindowBackground(dpy,win,BgXColor.pixel);

		/* set the font */
		XSetFont(dpy,gc,font->fid);

		/* map the window real */
		XMapWindow(dpy, win);
}
