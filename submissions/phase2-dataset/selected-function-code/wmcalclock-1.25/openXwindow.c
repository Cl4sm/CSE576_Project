void openXwindow(int argc, char *argv[], char *pixmap_bytes[], char *pixmask_bits, 
    int pixmask_width, int pixmask_height) {

    unsigned int	borderwidth = 1;
    XClassHint		classHint;
    char		*wname = argv[0];
    XTextProperty	name;
    XGCValues		gcv;
    unsigned long	gcm;
    int			dummy=0, red, grn, blu;
    XpmColorSymbol 	cols[10]={	{"Back", NULL, 0},
    					{"Color1", NULL, 0},
    					{"Color2", NULL, 0},
    					{"Color3", NULL, 0},
    					{"Color4", NULL, 0},
    					{"Color5", NULL, 0},
    					{"Color6", NULL, 0},
    					{"Color7", NULL, 0},
    					{"Color8", NULL, 0},
    					{"Color9", NULL, 0}};


    

    /* 
     *  Create Pixmap
     */
    cols[0].pixel		  = getColor(BackgroundColor, 1.0000, &red, &grn, &blu);
    cols[1].pixel		  = getBlendedColor(TimeColor, 0.1522, red, grn, blu);
    cols[2].pixel		  = getBlendedColor(TimeColor, 0.2602, red, grn, blu);
    cols[3].pixel		  = getBlendedColor(TimeColor, 0.3761, red, grn, blu);
    cols[4].pixel		  = getBlendedColor(TimeColor, 0.4841, red, grn, blu);
    cols[5].pixel		  = getBlendedColor(TimeColor, 0.5922, red, grn, blu);
    cols[6].pixel		  = getBlendedColor(TimeColor, 0.6980, red, grn, blu);
    cols[7].pixel		  = getBlendedColor(TimeColor, 0.7961, red, grn, blu);
    cols[8].pixel		  = getBlendedColor(TimeColor, 0.8941, red, grn, blu);
    cols[9].pixel		  = getBlendedColor(TimeColor, 1.0000, red, grn, blu);

    wmgen.attributes.numsymbols   = 10;
    wmgen.attributes.colorsymbols = cols;
    wmgen.attributes.exactColors  = False;
    wmgen.attributes.closeness    = 40000;
    wmgen.attributes.valuemask    = XpmReturnPixels | XpmReturnExtensions | XpmColorSymbols 
							| XpmExactColors | XpmCloseness | XpmSize;
    if (XpmCreatePixmapFromData(display, Root, pixmap_bytes, 
      &(wmgen.pixmap), &(wmgen.mask), &(wmgen.attributes)) != XpmSuccess){
	fprintf(stderr, "Not enough free colorcells.\n");
	exit(1);
    }




    /* 
     *  Create a window 
     */
    mysizehints.flags = USSize | USPosition;
    mysizehints.x = 0;
    mysizehints.y = 0;

    back_pix = getColor("white", 1.0, &red, &grn, &blu);
    fore_pix = getColor("black", 1.0, &red, &grn, &blu);

    XWMGeometry(display, screen, Geometry, NULL, borderwidth, &mysizehints,
				&mysizehints.x, &mysizehints.y,&mysizehints.width,&mysizehints.height, &dummy);

    mysizehints.width = 64;
    mysizehints.height = 64;
		


    win = XCreateSimpleWindow(display, Root, mysizehints.x, mysizehints.y,
				mysizehints.width, mysizehints.height, borderwidth, fore_pix, back_pix);
	
    iconwin = XCreateSimpleWindow(display, win, mysizehints.x, mysizehints.y,
				mysizehints.width, mysizehints.height, borderwidth, fore_pix, back_pix);



    /* 
     *  Activate hints 
     */
    XSetWMNormalHints(display, win, &mysizehints);
    classHint.res_name = wname;
    classHint.res_class = wname;
    XSetClassHint(display, win, &classHint);



    /*
     *  Set up the xevents that you want the relevent windows to inherit
     *  Currently, its seems that setting KeyPress events here has no
     *  effect. I.e. for some you will need to Grab the focus and then return
     *  it after you are done...
     */
    XSelectInput(display, win, ButtonPressMask | ExposureMask | ButtonReleaseMask 
		| PointerMotionMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask 
						| KeyPressMask | KeyReleaseMask);
    XSelectInput(display, iconwin, ButtonPressMask | ExposureMask | ButtonReleaseMask 
		| PointerMotionMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask 
						| KeyPressMask | KeyReleaseMask);


    if (XStringListToTextProperty(&wname, 1, &name) == 0) {
        fprintf(stderr, "%s: can't allocate window name\n", wname);
        exit(1);
    }


    XSetWMName(display, win, &name);

    /* 
     *   Create Graphics Context (GC) for drawing 
     */
    gcm = GCForeground | GCBackground | GCGraphicsExposures;
    gcv.foreground = fore_pix;
    gcv.background = back_pix;
    gcv.graphics_exposures = 0;
    NormalGC = XCreateGC(display, Root, gcm, &gcv);



    pixmask = XCreateBitmapFromData(display, win, pixmask_bits, pixmask_width, pixmask_height);
    XShapeCombineMask(display, win, ShapeBounding, 0, 0, pixmask, ShapeSet);
    XShapeCombineMask(display, iconwin, ShapeBounding, 0, 0, pixmask, ShapeSet);


    mywmhints.initial_state = WithdrawnState;
    mywmhints.icon_window = iconwin;
    mywmhints.icon_x = mysizehints.x;
    mywmhints.icon_y = mysizehints.y;
    mywmhints.window_group = win;
    mywmhints.flags = StateHint | IconWindowHint | IconPositionHint | WindowGroupHint;


    XSetWMHints(display, win, &mywmhints);


    XSetCommand(display, win, argv, argc);
    XMapWindow(display, win);

}
