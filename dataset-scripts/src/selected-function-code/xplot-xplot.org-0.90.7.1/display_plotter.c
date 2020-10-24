  } while (r > 0);
  
}

void display_plotter(PLOTTER pl)
{
  XSetWindowAttributes attr;
  Window rootwindow;

  if (pl->win != 0) {
    fprintf(stderr,
	    "display_plotter called for already-displayed plotter\n");
    return;
  }

  rootwindow = XRootWindowOfScreen(pl->screen);

  /* set up foreground/background colors */

  {
    Colormap default_cmap = DefaultColormap(pl->dpy, DefaultScreen(pl->dpy));
    char *foreground_color_name;
    char *background_color_name;
    XColor exact_return;
    int i;

    foreground_color_name = XGetDefault(pl->dpy, global_argv[0], "foreground");
    if (!foreground_color_name) foreground_color_name = "white";
    i = XAllocNamedColor(pl->dpy, default_cmap, foreground_color_name,
			 &exact_return, &pl->foreground_color);
    if (i < 0)
    {
      fprintf(stderr, "XAllocNamedColor failed for %s: %d\n",
	      foreground_color_name, i);
      return;
    }

#if 0
    ColorNames[0] = strdup(foreground_color_name);
#else
    ColorNames[0] = (char *) malloc(strlen(foreground_color_name) + 1);
    if (ColorNames[0] == NULL) fatalerror("malloc returned null");
    strcpy(ColorNames[0], foreground_color_name);
#endif

    background_color_name = XGetDefault(pl->dpy, global_argv[0], "background");
    if (!background_color_name) background_color_name = "black";
    i = XAllocNamedColor(pl->dpy, default_cmap, background_color_name,
			 &exact_return, &pl->background_color);
    if (i < 0)
    {
      fprintf(stderr, "XAllocNamedColor failed for %s: %d\n",
	      background_color_name, i);
      return;
    }
  }      
        
  attr.background_pixel = pl->background_color.pixel;
  attr.border_pixel     = pl->foreground_color.pixel;
  attr.event_mask       = ButtonReleaseMask|ButtonPressMask|ExposureMask|
    EnterWindowMask|LeaveWindowMask|PointerMotionMask|PointerMotionHintMask|
    StructureNotifyMask|VisibilityChangeMask;
#if 1
  attr.cursor           = XCreateFontCursor(pl->dpy, XC_crosshair);
#else
  attr.cursor           = XCreateFontCursor(pl->dpy, XC_tcross);
#endif
  
  {
    XColor blk,wht;
    
    blk.red = blk.green = blk.blue = 0;
    wht.red = wht.green = wht.blue = -1;
    wht.flags = blk.flags = DoRed|DoGreen|DoBlue;
    
    XRecolorCursor(pl->dpy, attr.cursor, &wht, &blk);
  }
  
  {
    char *geom = XGetDefault (pl->dpy, global_argv[0], "geometry");
    int x = 10, y = 70, borderw = 1;
    unsigned int width = 400, height = 400;
    int flags = 0;
    int i;
    
    for (i = 1 ; i < global_argc ; i++) {
      if (strcmp("-geometry", global_argv[i]) == 0) {
	if (i+1 < global_argc) {
	  geom = global_argv[i+1];
	  break;
	} else {
	  static int virgin = 1;
	  if (virgin) {
	    fprintf(stderr, "-geometry on command line without any following argument\n");
	    virgin = 0;
	  }
	}
      }
    }
    if (geom)
     {
	flags = XParseGeometry (geom, &x, &y, &width, &height);
	if (flags & XValue)
	  {
	    if (flags & XNegative)
	      x += DisplayWidth (pl->dpy, ScreenNumberOfScreen(pl->screen))
		- width - borderw*2;
	    else
	      x += borderw;
	  }
	if (flags & YValue)
	  {
	    if (flags & YNegative)
	      y += DisplayHeight (pl->dpy, ScreenNumberOfScreen(pl->screen))
		- height - borderw*2;
	    else
	      y += borderw;
	  }
      }
    
    if (pl->numtiles > 0) {
      /* Now we have geometry in x, y, height, width for total xplot area */
      y += (height * pl->tileno) / pl->numtiles;
      height = height / pl->numtiles;
      /* we have now selected the nth 1/numtiles chunk */
    }

    pl->win = XCreateWindow(pl->dpy, rootwindow, x, y, width, height, borderw,
			    DefaultDepth(pl->dpy, ScreenNumberOfScreen(pl->screen)),
			    CopyFromParent, CopyFromParent,
			    CWBackPixel|CWBorderPixel|CWEventMask|CWCursor,
			    &attr);
    
    pl->xsh.flags = 0;
    if (flags & (WidthValue | HeightValue))
      pl->xsh.flags |= USSize;
    else
      pl->xsh.flags |= PSize;
    if (flags & (XValue | YValue))
      pl->xsh.flags |= USPosition;
    else
      pl->xsh.flags |= PPosition;
    
    pl->xsh.width = width;
    pl->xsh.height = height;
    pl->xsh.x = x;
    pl->xsh.y = y;
    XSetStandardProperties(pl->dpy, pl->win, "xplot", "xplot", None,
			   global_argv, global_argc, &pl->xsh);
  }
  
  XMapRaised(pl->dpy, pl->win);
  
  pl->visibility = VisibilityFullyObscured; /* initially true */
  {
    char *use_font = XGetDefault (pl->dpy, global_argv[0], "font");
    pl->font_struct = XLoadQueryFont(pl->dpy, use_font ? use_font : "fixed");
  }
  
  pl->gcv.foreground = pl->foreground_color.pixel;
  pl->gcv.background = pl->background_color.pixel;
  pl->gcv.font = pl->font_struct->fid;
  pl->gcv.line_width = 0;
  pl->gcv.cap_style = CapProjecting;
  if (pl->thick) {
    pl->gcv.line_width = 3;
    pl->gcv.cap_style = CapRound;
  }
  
  {
#define N_DPY_S 2

    static struct dpy_info {
      unsigned long line_plane_mask;
      Colormap clr_map;
      int depth;
      XColor clr;
      unsigned long pixel[NCOLORS];
      int Colors[NCOLORS];
      int virgin;
      int warned_color_alloc_failed;
      Atom xplot_nagle_atom;
      Display *saved_dpy;
    } d_i[N_DPY_S];

    int i;
    int d;
    
    for (d = 0; d < N_DPY_S; d++) {
      if (pl->dpy == d_i[d].saved_dpy)
	break;
      if (d_i[d].saved_dpy == 0) {
	d_i[d].saved_dpy = pl->dpy;
	d_i[d].virgin = 1;
	break;
      }
    }
    if (d >= N_DPY_S) {
      fprintf(stderr, "%s:%d:%s: bug -- pl->dpy != saved_dpy\n",
	      __FILE__, __LINE__, __FUNCTION__);
      panic("bug");
    }

    /* Allocate some color cells */
      
    if (d_i[d].virgin ) {
      int ci;

      d_i[d].virgin = 0;
      
      d_i[d].xplot_nagle_atom = XInternAtom(pl->dpy, "XPLOT_NAGLE", False);

      d_i[d].clr_map = DefaultColormap(pl->dpy, DefaultScreen(pl->dpy));
      
      d_i[d].depth = DisplayPlanes(pl->dpy, DefaultScreen(pl->dpy));

      ci = 0;

      /* if display/screen has only 1 bit of depth, don't try to
       * allocate any colors - just use BlackPixel and WhitePixel
       */
      if ( ! option_mono && d_i[d].depth > 1
	   && XAllocColorCells(pl->dpy, 
			       d_i[d].clr_map, 0,
			       &d_i[d].line_plane_mask, 1,
			       d_i[d].pixel, NColors)
	   )
	{
	  for ( ; ci < NColors; ci++)  {
	    XParseColor(pl->dpy, d_i[d].clr_map, ColorNames[ci], &d_i[d].clr);
	    d_i[d].clr.pixel = d_i[d].pixel[ci];
	    XStoreColor (pl->dpy, d_i[d].clr_map, &d_i[d].clr);
	    d_i[d].clr.pixel |= d_i[d].line_plane_mask;
	    XStoreColor (pl->dpy, d_i[d].clr_map, &d_i[d].clr);
	    d_i[d].Colors[ci] = d_i[d].clr.pixel;
	  }

	} else if (! option_mono && d_i[d].depth > 1 ) {
	  /* some visual types (e.g. TrueColor) do not support XAllocColorCells */

	  for ( ; ci < NColors; ci++) {
	    XColor exact_return;

	    i = XAllocNamedColor(pl->dpy, d_i[d].clr_map, ColorNames[ci],
				 &exact_return, &d_i[d].clr);
	    if ( i < 0 )
	      {
		fprintf(stderr, "XAllocNamedColor failed for %s: %d\n",
			ColorNames[i], i);
		break;
	      }

	    /* Here, we should check if the color is close enough. */
	    d_i[d].Colors[ci] = d_i[d].clr.pixel;

#if 0
	    /***** and on any failure, you should break out of this
	      loop leaving ci pointing at the first color (perhaps
	      zero) of the first color you've failed to allocate. */

	    /***** need to take care that this does the correct thing
	      on one-bit-plane displays.    There are two ways that this might occur:
	    
	      1. All pixel values obtained in this loop on a
	      one-bit-plane display are the same as WhitePixelOfScreen()

	      or
	      
	      2. On a one-bitplane display we break out of this loop
	      and leave the loop below to fill in WhitePixelOfScreen()
	      for each plot color.
	      */
#endif
	  }
	}
      for ( ; ci < NColors; ci++) {

	/* probably only one bit plane, or all the color cells are taken
	   (or option_mono)*/

	if (!d_i[d].warned_color_alloc_failed && !option_mono) {
	  fputs("unable to get all desired colors, will substitute white for some or all colors\n",
		stderr);
	  d_i[d].warned_color_alloc_failed = 1;
	}
	d_i[d].Colors[ci] = WhitePixelOfScreen(pl->screen);
      }
    }
    
    for (i = 0; i < NColors; i++) {

      pl->gcs[i] = XCreateGC(pl->dpy, pl->win,
			     GCForeground|GCFont|GCLineWidth|GCCapStyle,
			     &(pl->gcv));
    
      XSetForeground(pl->dpy, pl->gcs[i], d_i[d].Colors[i]);

    }

    pl->xplot_nagle_atom = d_i[d].xplot_nagle_atom;
#if 0
    printf("DEBUG: nagle_atom is %ld\n", pl->xplot_nagle_atom);
#endif
  }


  pl->decgc = XCreateGC(pl->dpy, pl->win,
			GCForeground|GCFont|GCLineWidth|GCCapStyle,
			&(pl->gcv));
  
  pl->gcv.foreground ^= pl->gcv.background;
  pl->gcv.function = GXxor;
  
  pl->xorgc = XCreateGC(pl->dpy, pl->win,
			GCForeground|GCFunction|GCFont|GCLineWidth|GCCapStyle,
			&(pl->gcv));
  
  pl->gcv.foreground = pl->gcv.background;

  /* just like dec gc but draws in background color */
  pl->bacgc = XCreateGC(pl->dpy, pl->win,