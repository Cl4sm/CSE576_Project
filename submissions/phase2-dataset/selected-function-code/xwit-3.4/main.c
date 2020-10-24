int
main(int argc, char *argv[])
{
	Window window = 0;
	int *pargc = &argc;

	program_name = argv[0] + strlen(argv[0]);
	while (program_name != argv[0] && program_name[-1] != '/')
		program_name--;

	Setup_Display_And_Screen(pargc, argv);

	Winidmode = WID_env;

	while (argv++, --argc > 0) {
		/* argv[0] = next argument */
		/* argc = # of arguments left */
		if (matchopt("a*ll", 0, pargc, argv)) {
			Winidmode = WID_names;
			names = allwindows;
			numnames = 1;
		}
		else if (matchopt("ba*ckingstore", 0, pargc, argv) ||
			matchopt("bs", 0, pargc, argv)) {
			function |= FBIT(F_winattr);
			Gbs = 1;
		}
		else if (matchopt("b*itmap", 1, pargc, argv)) {
			function |= FBIT(iconbitmap);
			bitmapname = *++argv;
		}
		else if (matchopt("colo*rmap", 1, pargc, argv) ||
			matchopt("cm*ap", 1, pargc, argv)) {
			function |= FBIT(colormap);
			cmap = (Colormap) getxid(*++argv);
		}
		else if (matchopt("co*lumns", 1, pargc, argv)) {
			function |= FBIT(columns);
			ncolumns = atoi(*++argv);
		}
		else if (matchopt("store*buffer", 1, pargc, argv)) {
			nbuffer = atoi(*++argv);
                        StoreBuffer( dpy, nbuffer );
		}
		else if (matchopt("fetch*buffer", 1, pargc, argv)) {
			nbuffer = atoi(*++argv);
                        FetchBuffer( dpy, nbuffer );
		}
		else if (matchopt("c*urrent", 0, pargc, argv)) {
			Winidmode = WID_curr;
		}
		else if (matchopt("iconm*ove", 2, pargc, argv)) {
			function |= FBIT(iconmove);
			Giconx = atoi(argv[1]);
			Gicony = atoi(argv[2]);
			argv += 2;
		}
		else if (matchopt("iconn*ame", 1, pargc, argv) ||
			matchopt("in*ame", 1, pargc, argv)) {
			function |= FBIT(iconname);
			wmiconname = *++argv;
		}
		else if (matchopt("id", 1, pargc, argv)) {
			Winidmode = WID_num;
			window = (Window) getxid(*++argv);
		}
		else if (matchopt("i*conify", 0, pargc, argv)) {
			function |= FBIT(icon);
		}
		else if (matchopt("k*eyrepeat", 1, pargc, argv) ||
			matchopt("nok*eyrepeat", 1, pargc, argv)) {
			int i;

			function |= argv[0][1] == 'n' ?
				FBIT(nokeyrepeat) : FBIT(keyrepeat);

			i = atoi(*++argv);
			if (i < 0)
				usage();

			while (1) {
				keys[i & 0xff] = 1;
				if (argc <= 0)
					break;
				if (strcmp(argv[0], "-") == 0) {
					int from = i;

					argc--, argv++;
					if (argc < 0 || (i = atoi(argv[0])) <= 0)
						usage();
					while (from <= i)
						keys[from++ & 0xff] = 1;
					argc--, argv++;
					if (argc <= 0)
						break;
				}
				if ((i = atoi(argv[0])) <= 0)
					break;
				argc--, argv++;
			}
		}
		else if (matchopt("li*st", 1, pargc, argv) ||
			matchopt("names", 1, pargc, argv)) {
			Winidmode = WID_names;
			/* take rest of arg list */
			names = ++argv;
			numnames = argc;
			argc = 0;
		}
		else if (matchopt("l*abel", 1, pargc, argv)) {
			function |= FBIT(name);
			wmname = *++argv;
		}
		else if (matchopt("ma*sk", 1, pargc, argv)) {
			function |= FBIT(iconbitmap);
			maskname = *++argv;
		}
		else if (matchopt("m*ove", 2, pargc, argv)) {
			function |= FBIT(move);
			Gright = (argv[1][0] == '-');
			Gbottom = (argv[2][0] == '-');
			tox = atoi(argv[1]);
			toy = atoi(argv[2]);
			argv += 2;
		}
		else if (matchopt("noba*ckingstore", 0, pargc, argv) ||
			matchopt("nobs", 0, pargc, argv)) {
			function |= FBIT(F_winattr);
			Gbs = -1;
		}
		else if (matchopt("nosaveu*nder", 0, pargc, argv) ||
			matchopt("nosu", 0, pargc, argv)) {
			function |= FBIT(F_winattr);
			Gsu = -1;
		}
		else if (matchopt("nos*ave", 0, pargc, argv)) {
			function |= FBIT(nosave);
		}
		else if (matchopt("n*ame", 1, pargc, argv)) {
			function |= FBIT(name);
			wmname = *++argv;
		}
		else if (matchopt("o*pen", 0, pargc, argv)) {
			function |= FBIT(pop);
		}
		else if (matchopt("p*op", 0, pargc, argv)) {
			function |= FBIT(pop);
		}
		else if (matchopt("pr*int", 0, pargc, argv)) {
			function |= FBIT(print);
		}
		else if (matchopt("f*ocus", 0, pargc, argv)) {
			function |= FBIT(focus);
		}
		else if (matchopt("ra*ise", 0, pargc, argv)) {
			function |= FBIT(raise);
		}
		else if (matchopt("lo*wer", 0, pargc, argv)) {
			function |= FBIT(lower);
		}
		else if (matchopt("op*posite", 0, pargc, argv)) {
			function |= FBIT(opposite);
		}
		else if (matchopt("cir*culate", 0, pargc, argv)) {
			function |= FBIT(circulate);
		}
		else if (matchopt("uncir*culate", 0, pargc, argv)) {
			function |= FBIT(uncirculate);
		}
		else if (matchopt("ri*conmove", 2, pargc, argv)) {
			function |= FBIT(riconmove);
			Giconx = atoi(argv[1]);
			Gicony = atoi(argv[2]);
			argv += 2;
		}
		else if (matchopt("rm*ove", 2, pargc, argv)) {
			function |= FBIT(rmove);
			tox = atoi(argv[1]);
			toy = atoi(argv[2]);
			argv += 2;
		}
		else if (matchopt("roo*t", 0, pargc, argv)) {
			Winidmode = WID_root;
		}
		else if (matchopt("ro*ws", 1, pargc, argv)) {
			function |= FBIT(rows);
			nrows = atoi(*++argv);
		}
		else if (matchopt("rw*arp", 2, pargc, argv)) {
			function |= FBIT(rwarp);
			warpx = atoi(argv[1]);
			warpy = atoi(argv[2]);
			argv += 2;
		}
		else if (matchopt("r*esize", 2, pargc, argv)) {
			function |= FBIT(resize);
			towidth = atoi(argv[1]);
			toheight = atoi(argv[2]);
			argv += 2;
		}
		else if (matchopt("saveu*nder", 0, pargc, argv) ||
			matchopt("su", 0, pargc, argv)) {
			function |= FBIT(F_winattr);
			Gsu = argv[0][1] == 1;
		}
		else if (matchopt("se*lect", 0, pargc, argv)) {
			Winidmode = WID_select;
		}
		else if (matchopt("sy*nc", 0, pargc, argv)) {
			XSynchronize(dpy, True);
		}
		else if (matchopt("s*ave", 0, pargc, argv)) {
			function |= FBIT(save);
		}
		else if (matchopt("un*map", 0, pargc, argv)) {
			function |= FBIT(unmap);
		}
		else if (matchopt("w*arp", 2, pargc, argv)) {
			function |= FBIT(warp);
			warpx = atoi(argv[1]);
			warpy = atoi(argv[2]);
			argv += 2;
		} else if(matchopt("prop*erty",1, pargc,argv)) {
			property = XInternAtom(dpy,argv[1],False);
			if( None == property ) {
			    Fatal_Error("Unknown atom %s",argv[1]);
			}
			argv++;
		}
		else
			usage();
	}

	/* default function: pop */
	if (function == 0)
		function = FBIT(pop);

	if ((function & ~NOWINDOW) == 0)
		Winidmode = WID_none;

	root = DefaultRootWindow(dpy);

	switch (Winidmode) {
	case WID_env:
		{
			char *s;

			s = getenv("WINDOWID");
			if (s != 0)
				window = (Window) getxid(s);
			else {
				/* no WINDOWID, use window under cursor */
				window = xwit_select_window(dpy, 1);
				if (window == None)
					Fatal_Error("WINDOWID not set");
			}
		}
		break;
	case WID_root:
		window = root;
		break;
	case WID_curr:
		window = xwit_select_window(dpy, 1);
		break;
	case WID_select:
		window = xwit_select_window(dpy, 0);
		break;
	default:
	        break;
	}

	switch (Winidmode) {
	case WID_none:
		doit((Window) 0);
		break;
	case WID_names:
		downtree(root);
		break;
	default:
		if (!window)
			Fatal_Error("no window selected");
		doit(window);
		break;
	}

	XSync(dpy, True);
	(void) XCloseDisplay(dpy);
	return(!Gwinfound);
}