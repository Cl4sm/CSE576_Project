                           called among program initializations */
{
#ifdef  X11
  char                  *display_name = NULL;   /* Server to connect to      */
  Pixmap                icon_pixmap;            /* Icon                      */
  XSizeHints            size_hints;             /* Preferred sizes           */
  XSetWindowAttributes  win_attribs;            /* Window attributes         */
  XWMHints              wm_hints;               /* Window manager hints      */
  XClassHint            class_hints;            /* Class hints               */
  XTextProperty         window_name, icon_name; /* Names for Icon & Window   */
  XGCValues             gc_values;              /* Graphics Context values   */
  static char           *colorname[GR_COLORS] = {
                                GR_COLOR00, GR_COLOR01, GR_COLOR02, GR_COLOR03,
                                GR_COLOR04, GR_COLOR05, GR_COLOR06, GR_COLOR07,
                                GR_COLOR08, GR_COLOR09, GR_COLOR10, GR_COLOR11,
                                GR_COLOR12, GR_COLOR13, GR_COLOR14, GR_COLOR15
                        };
  XColor                x_color;                /* X11 Color structure       */
  unsigned long         mask;                   /* Mask for selections       */
  int                   i;                      /* loop counter variable     */

#else   /* PC version */
int     errcode;
#endif

/***************************/
/* generic initializations */
/***************************/
  gr_x = gr_y = gr_dir = 0.0;
  gr_max_x = gr_max_y = gr_max_color = 0;

  gr_graphicsavail = 0; /* DEFAULT is no graphics - you can do without */

/********************************************/
/*****  Initialize X11 turtlegraphics   *****/
/********************************************/
#ifdef X11
        /* connect to X server */
        if( (gr_display = XOpenDisplay(display_name)) != NULL )
        {

          /*****************************/
          /* connection to X server OK */
          /*****************************/

          gr_screen = DefaultScreen( gr_display );      /* X screen number */

          /* Create a window with Black background and border */
          gr_win
            = XCreateSimpleWindow( gr_display,
                                  RootWindow( gr_display, gr_screen),
                                  0, 0, /* initial placement */
                                  GR_DEF_XSIZE, GR_DEF_YSIZE,
                                  3, /* border width */
                                  /* border pixel value */
                                  BlackPixel(gr_display, gr_screen),
                                  /* background pixel value */
                                  BlackPixel(gr_display, gr_screen) );

          /* Select input (events) for the window */
          XSelectInput( gr_display, gr_win,
                        StructureNotifyMask|ExposureMask );

          /* Check for backing store capability */
          if( !DoesBackingStore(DefaultScreenOfDisplay(gr_display)) )
          {
            fprintf( stderr, "%s: Warning: \
X server does not offer backing store capability.\n\
Window cannot be redrawn if obscured. Sorry...\n", gr_progname );
          }
          else
          {
              /* Enable the backing store feature of X server
                 and set bit gravity */
              win_attribs.bit_gravity   = NorthWestGravity;
              win_attribs.backing_store = Always;
              mask                      = CWBitGravity | CWBackingStore;
              XChangeWindowAttributes( gr_display, gr_win, mask, &win_attribs );
          }

          /* Make names of Window and Icon for window manager */
          if( XStringListToTextProperty(&gr_windowname, 1, &window_name) == 0 ) {
            (void)fprintf( stderr, "%s: Structure allocation for windowName\
 failed.\n", gr_progname );
            exit( 42 );
          }
          if( XStringListToTextProperty(&gr_iconname, 1, &icon_name) == 0 ) {
            (void)fprintf( stderr, "%s: Structure allocation for iconName\
 failed.\n", gr_progname );
            exit( 42 );
          }

          /* Create the icon */
          icon_pixmap = XCreateBitmapFromData( gr_display, gr_win, turtle_bits,
                                               turtle_width, turtle_height );

          /* Window size, state, icon etc. hints for the window manager */
          size_hints.flags = PPosition | PMaxSize | PMinSize | USSize;
          /* position and desired size are given to XCreateSimpleWindow call */
          size_hints.min_width          = GR_MIN_XSIZE;
          size_hints.min_height         = GR_MIN_YSIZE;
          size_hints.max_width          = GR_MAX_XSIZE;
          size_hints.max_height         = GR_MAX_YSIZE;
          wm_hints.flags = StateHint | IconPixmapHint | InputHint;
          wm_hints.initial_state        = NormalState;
          wm_hints.input                = False;
          wm_hints.icon_pixmap          = icon_pixmap;
          class_hints.res_name          = gr_progname;
          class_hints.res_class         = gr_classname;
          XSetWMProperties( gr_display, gr_win, &window_name, &icon_name,
                            gr_argv, gr_argc,
                            &size_hints, &wm_hints, &class_hints );


          /* Handle colors; this is quite complicated in X11 */

          if( DefaultDepth( gr_display, gr_screen ) == 1 )
          {
            /* Only 1 bitplane, BW screen */
            /* Emulate colors with 0 as Black and 1-15 White */
            gr_colortbl[0] = BlackPixel( gr_display, gr_screen );
            for( i = 1; i < GR_COLORS; ++i )
              gr_colortbl[i] = WhitePixel( gr_display, gr_screen );
# ifdef TESTING
            fprintf( stderr, "%s: 1-plane system, substituting White for \
colors 1-15.\n", gr_progname );
            fprintf( stderr, "%s: Pixel value is %lu for Black, \
%lu for White\n", gr_progname, gr_colortbl[0], gr_colortbl[1] );
# endif
          }
          else
          {
              /* more than 1 bitplane */
              for( i = 0; i < GR_COLORS; ++i )
              {
                  /* Initialize the colortable using named colors */
                  if( XParseColor( gr_display,
                                   DefaultColormap(gr_display, gr_screen),
                                   colorname[ i ], &x_color ) )
                  {
                      if( !XAllocColor( gr_display,
                                        DefaultColormap(gr_display, gr_screen),
                                        &x_color ) )
                      {
                          fprintf( stderr, "%s: Can't allocate color \
\"%s\" (%d). Substituting White.\n",
                                   gr_progname,
                                   colorname[ i ], i );
                          gr_colortbl[i] = WhitePixel( gr_display, gr_screen );
                      }
                      else
                      {
                          /* succeeded in allocating color */
                          gr_colortbl[ i ] = x_color.pixel;
# ifdef TESTING
                          fprintf( stderr, "%s: Pixel value is %lu for %s.\n",
                                   gr_progname, gr_colortbl[i], colorname[i] );
# endif
                      }
                  }
                  else
                  {
                      /* could not parse color */
                      fprintf( stderr,
                               "%s: Color name \"%s\" (%d) not in database. \
Substituting White.\n",
                               gr_progname, colorname[i], i );
                      gr_colortbl[i] = WhitePixel( gr_display, gr_screen );
                  }
              } /* for */
          } /* else */
          gr_max_color = GR_COLORS - 1;

          /* Create and initialize a default GC */
          gr_gc = XCreateGC( gr_display, gr_win, 0L, &gc_values );

          /* Initialize the drawing color, default's black */
          XSetForeground( gr_display, gr_gc, gr_colortbl[ 0 ] );
          XSetBackground( gr_display, gr_gc, gr_colortbl[ 0 ] );
          gr_color = 0;

          /* OK, we _do_ have graphics available */
          gr_graphicsavail = 1;

# ifdef __STDC__
          /* Let's do the Right Thing if possible :) */
          atexit( close_turtlegr );
# endif
      } /* if */
      else {
          gr_graphicsavail = 0;
      }
/********************************************/
/*****  Initialize PC turtlegraphics    *****/
/********************************************/
#else   /* PC version */
          gr_drivernum = DETECT;

          detectgraph( &gr_drivernum, &gr_max_display_mode );
          if( gr_drivernum != grNotDetected ) {
              if( !getenv( BGIDIR_ENVSTRING ) )
                 fprintf( stderr,
                        "You really should set the %s environment variable.\n",
                        BGIDIR_ENVSTRING );
              initgraph( &gr_drivernum, &gr_max_display_mode,
                         getenv( BGIDIR_ENVSTRING ) );
              errcode = graphresult();
              if( errcode != grOk ) {
                  fputs( "Graphics error: ", stderr );
                  fputs( grapherrormsg( errcode ), stderr );
                  exit( EXIT_FAILURE );
              }
              moveto( 0, 0 );
              gr_x = gr_y = 0.0;
              setcolor( 0 );
              gr_color = 0;
              gr_max_x = getmaxx();
              gr_max_y = getmaxy();
              gr_max_color = getmaxcolor();
              gr_max_display_mode = getmaxmode();
              restorecrtmode();
              gr_graphicsavail = 1;
              atexit( close_turtlegr );
          }
          else {
              gr_graphicsavail = 0;
          }
#endif

/* generic */
        init_iprocs( graph0, tc7_subr_0 );
        init_iprocs( graph1, tc7_subr_1 );
        init_iprocs( graph2, tc7_subr_2 );
        init_iprocs( graph3, tc7_subr_3 );
        gr_grmode_on = 0;

#ifndef X11
  /* PC version clears screen so this must be repeated */
  init_banner();
#endif

  fputs("\nSCM Turtlegraphics Copyright (C) 1992 sjm@cc.tut.fi, jtl@cc.tut.fi\n\
Type `(help-gr)' or `(help-turtlegr)' for a quick reference of\n\
the new primitives.\n", stderr);

  if( !gr_graphicsavail ) {
#ifdef X11
        fprintf( stderr, "%s: No X server found. \
Turtlegraphics not available.\n", gr_progname );
#else
        fputs( "No graphics adapter detected. \
Turtlegraphics not available.\n", stderr );
#endif
  }
  else {
#ifdef X11
        gr_events(0);
#else
        ;
#endif
  }
} /* init_turtlegr() */
