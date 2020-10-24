  }
  return;
}

int main(int argc, char *argv[])
{

  command *c;
  int dummy_int;
  int option_tile = FALSE;
  int option_one_at_a_time = FALSE;
  unsigned int dummy_unsigned_int;
  Window dummy_window;
  Display *dpy = 0;
  Display *dpy2 = 0;
  Display *dpy_of_event = 0;
  PLOTTER pl;
  coord x_synch_bb_left;
  coord y_synch_bb_bottom;
  coord x_synch_bb_right;
  coord y_synch_bb_top;

  XEvent event;
  int i;
  /* Are x and/or y axis of the various windows locked together? */
  bool x_synch = FALSE, y_synch = FALSE;

  global_argc = argc;
  global_argv = argv;

  /* Look for -v or -version argument */
  for (i = 1; i < argc && *argv[i] == '-'; i++) {
    if (strcmp ("-v", argv[i]) == 0
	|| strcmp ("-version", argv[i]) == 0) {
      extern char* version_string;
      printf("xplot version %s\n", version_string);
#if 0
      printf("sizeof command is %d\n", sizeof(command));
      printf("sizeof plotter is %d\n", sizeof(struct plotter));
#endif
      exit(0);
    }
  }

  i = 1;

  /* Look for -x and/or -y options, and look for -t option*/
  for (; i < argc && *argv[i] == '-'; i++)
    {
      if (strcmp ("-x", argv[i]) == 0)
	x_synch = TRUE;
      else if (strcmp ("-y", argv[i]) == 0)
	y_synch = TRUE;
      else if (strcmp ("-thick", argv[i]) == 0)
	option_thick = TRUE;
      else if (strcmp ("-tile", argv[i]) == 0)
	option_tile = TRUE;
      else if (strcmp ("-mono", argv[i]) == 0)
	option_mono = TRUE;
      else if (strcmp ("-1", argv[i]) == 0)
	option_one_at_a_time = TRUE;
      else if (strcmp ("-d", argv[i]) == 0
	       || strcmp ("-display", argv[i]) == 0) {
	i++;
	if (dpy == 0) {
	  dpy = XOpenDisplay(argv[i]);
	  if (dpy == NULL) panic("could not open display");
	} else {
	  if (dpy2 == 0) {
	    dpy2 = XOpenDisplay(argv[i]);
	    if (dpy2 == NULL) panic("could not open display");
	  } else {
	    panic("too many displays");
	  }
	}
      }
      else if (strcmp("-d2", argv[i]) == 0) {
	i++;
	if (dpy2 == 0) {
	  dpy2 = XOpenDisplay(argv[i]);
	  if (dpy2 == NULL) panic("could not open display");
	} else {
	  panic("too many d2's");
	}
      }
      else
	/* Give the user the benefit of the doubt and assume that
	   they want a file that starts with '-' */
	break;
    }
	
	       
  if (dpy == 0) {
    dpy = XOpenDisplay("");
    if (dpy == NULL) panic("could not open display");
  }

  {
    int numbase = i;
    int numwins = argc - i;
    int k;

    if (i < argc)
      for (k = i; k < argc; k++) {
	/*      for (k = argc-1; k>=i; k--) { */
	FILE *fp = 0;
	int len;
	len = strlen(argv[k]);
	if (strcmp(&argv[k][len-3],".gz") == 0) {
	  char *command;
	  command = (char *) malloc(50 + len);
	  if (command != 0) {
	    sprintf(command, "zcat %s", argv[k]);
	    fp = popen(command, "r");
	    free(command);
	  }
	} else {
	  fp = fopen(argv[k],"r");
	}
	if (fp) {
	  if (option_tile) {
	    new_plotter(fp, dpy, numwins, k-numbase, 0);
	  } else {
	    new_plotter(fp, dpy, 0, 0, 0);
	  }
	  fclose(fp);
	}
      }
    else
      /* 1 window, 0th */
      new_plotter(stdin, dpy, 1, 0, 0);

    if (dpy2 != 0) {

      if (i < argc)
	for (k = i; k < argc; k++) {
	  /*      for (k = argc-1; k>=i; k--) { */
	  FILE *fp = 0 ;
	  int len;
	  len = strlen(argv[k]);
	  if (strcmp(&argv[k][len-3],".gz") == 0) {
	    char *command;
	    command = (char *) malloc(50 + len);
	    if (command != 0) {
	      sprintf(command, "zcat %s", argv[k]);
	      fp = popen(command, "r");
	      free(command);
	    }
	  } else {
	    fp = fopen(argv[k],"r");
	  }
	  if (fp) {
	    if (option_tile) {
	      new_plotter(fp, dpy2, numwins, k-numbase, 0);
	    } else {
	      new_plotter(fp, dpy2, 0, 0, 0);
	    }
	    fclose(fp);
	  }
	}
      else {
	/* 1 window, 0th */
	panic("can't do dpy2 with stdin (yet)");
	new_plotter(stdin, dpy2, 1, 0, 0);
      }


    }

  }

  if ( ! the_plotter_list )
    {
      fprintf(stderr, "NO PLOTTERS\n");
      goto doexit;
    }

  /* Check that we are dealing with the same coordinate types. */
  if (the_plotter_list)
    {
#define BUTFIRSTPLOTTERS pl = the_plotter_list->next; pl != NULL; pl = pl->next
      if (x_synch)
	{
	  coord_type t = the_plotter_list->x_type;
	  for (BUTFIRSTPLOTTERS)
	    if (pl->x_type != t)
	      panic ("Attempt to synchronize different coordinate types");
	}
      if (y_synch)
	{
	  coord_type t = the_plotter_list->y_type;
	  for (BUTFIRSTPLOTTERS)
	    if (pl->y_type != t)
	      panic ("Attempt to synchronize different coordinate types");
	}
    }

#define ALLPLOTTERS pl = the_plotter_list ; pl != NULL; pl = pl->next
  for (ALLPLOTTERS) {
    int virgin = 1;
    for (c = pl->commands; c != NULL; c = c->next)
      switch(c->type) {
      case LINE:
      case DLINE:
	if (virgin || ccmp(pl->x_type, c->xb, pl_x_left, <))
	  pl_x_left = c->xb;
	if (virgin || ccmp(pl->x_type, c->xb, pl_x_right, >))
	  pl_x_right = c->xb;
	if (virgin || ccmp(pl->y_type, c->yb, pl_y_bottom, <))
	  pl_y_bottom = c->yb;
	if (virgin || ccmp(pl->y_type, c->yb, pl_y_top, >))
	  pl_y_top = c->yb;
	virgin = 0;
      case X:
      case DOT:
      case PLUS:
      case BOX:
      case DIAMOND:
      case UTICK:
      case DTICK:
      case LTICK:
      case RTICK:
      case HTICK:
      case VTICK:
      case UARROW:
      case DARROW:
      case LARROW:
      case RARROW:
      case INVISIBLE:
      case TEXT:
	if (virgin || ccmp(pl->x_type, c->xa, pl_x_left, <))
	  pl_x_left = c->xa;
	if (virgin || ccmp(pl->x_type, c->xa, pl_x_right, >))
	  pl_x_right = c->xa;
	if (virgin || ccmp(pl->y_type, c->ya, pl_y_bottom, <))
	  pl_y_bottom = c->ya;
	if (virgin || ccmp(pl->y_type, c->ya, pl_y_top, >))
	  pl_y_top = c->ya;
	virgin = 0;
      case TITLE:
      case XLABEL:
      case YLABEL:
	break;
      }
    pl_x_right = bump_coord(pl->x_type, pl_x_right);
    pl_y_top   = bump_coord(pl->y_type, pl_y_top);

    pl->viewno += 1;
    pl_x_left   = pl->x_left[0];
    pl_x_right  = pl->x_right[0];
    pl_y_top    = pl->y_top[0];
    pl_y_bottom = pl->y_bottom[0];
  }

  if (x_synch) {
    int virgin = 1;
    for (ALLPLOTTERS) {
      if (virgin || ccmp(pl->x_type, pl_x_left, x_synch_bb_left, <))
	x_synch_bb_left = pl_x_left;
      if (virgin || ccmp(pl->x_type, pl_x_right, x_synch_bb_right, >))
	x_synch_bb_right = pl_x_right;
      virgin = 0;
    }
    for (ALLPLOTTERS) {
      pl_x_left = x_synch_bb_left;
      pl_x_right = x_synch_bb_right;
    }
  }

  if (y_synch) {
    int virgin = 1;
    for (ALLPLOTTERS) {
      if (virgin || ccmp(pl->y_type, pl_y_bottom, y_synch_bb_bottom, <))
	y_synch_bb_bottom = pl_y_bottom;
      if (virgin || ccmp(pl->y_type, pl_y_top, y_synch_bb_top, >))
	y_synch_bb_top = pl_y_top;
      virgin = 0;
    }
    for (ALLPLOTTERS) {
      pl_y_top = y_synch_bb_top;
      pl_y_bottom = y_synch_bb_bottom;
    }
  }


  for (ALLPLOTTERS) {
    if (option_one_at_a_time == FALSE
	|| pl->next == 0
	) {
      display_plotter(pl);
    }
  }
    
  do {
    int SAVx, SAVy, SAVc, SAVd;
    lXPoint a,b;
    if (XPending(dpy) == 0
	&& (dpy2 == 0 || XPending(dpy2) == 0)
	) {
      int visible_count = 0;
      for (ALLPLOTTERS) {

#if 0
	/* if option_one_at_a_time, ensure that at least last plotter
	   on list is visible if no others are */ 
	if (visible_count == 0
	    && pl->next == 0
	    && pl->win == 0
	    && option_one_at_a_time
	    ) 
	  display_plotter(pl);
#endif
	/* if plotter is not yet displayed, do nothing */
	if (pl->win == 0) continue;

	visible_count++;
	
	if (pl->size_changed) {
	  int i;
	  XRectangle xr[1];

	  pl->size_changed = 0;
	  pl->clean = 0;
	  size_window(pl);
	  XClearWindow(pl->dpy, pl->win);
	  pl->pointer_marks_on_screen = FALSE;

	  xr[0].x = pl->origin.x;
	  xr[0].y = pl->origin.y - 2;
	  xr[0].width = pl->size.x + 2;
	  xr[0].height = pl->size.y + 2;

	  for (i = 0; i < NColors; i++)
	    XSetClipRectangles(pl->dpy, pl->gcs[i], 0, 0, xr, 1, YXBanded);

	}
	if (pl->new_expose) {
	  pl->clean = 0;
	  for (c = pl->commands; c != NULL; c = c->next) {
	    if (c->mapped)
	      c->needs_redraw = TRUE;
	  }
	  pl->new_expose = 0;
	}
	if (pl->visibility != VisibilityFullyObscured && pl->clean == 0) {
	  SAVx = -100000; SAVy = -100000; SAVc = SAVd = 0;
	  for (c = pl->commands; c != NULL; c = c->next)
	    if (c->mapped)
	      if (c->needs_redraw) {
		GC gc;
		dXPoint da,db;
		c->needs_redraw = FALSE;
		if (c->decoration
		    || c->type == TITLE
		    || c->type == XLABEL
		    || c->type == YLABEL)
		  gc = pl->decgc;
		else
		  if ( c->color >= 0 && c->color < NColors)
		    gc = pl->gcs[c->color];
		  else
		    gc = pl->gcs[0];

#ifndef WINDOW_COORDS_IN_COMMAND_STRUCT
		da.x = map_coord(pl->x_type, pl_x_left, pl_x_right, pl->size.x,
				 c->xa);
		da.y = (pl->size.y - 1) -
		  map_coord(pl->y_type, pl_y_bottom, pl_y_top, pl->size.y,
			    c->ya);

		db.x = map_coord(pl->x_type, pl_x_left, pl_x_right, pl->size.x,
				 c->xb);
		db.y = (pl->size.y - 1) -
		  map_coord(pl->y_type, pl_y_bottom, pl_y_top, pl->size.y,
			    c->yb);

		da = tomain(pl,da);
		db = tomain(pl,db);
#else
		da.x = c->a.x;
		da.y = c->a.y;
		db.x = c->b.x;
		db.y = c->b.y;
#endif
		{
#if 1 /* Xqdss has bugs, so we need to clamp at just a few thousand */
#define CLAMP 3000.0
#else
#define CLAMP 10000.0
#endif
		  if (da.x >  CLAMP) {
		    if (db.x-da.x != 0)
		      da.y -= (db.y-da.y)*(da.x-CLAMP)/(db.x-da.x);
		    da.x =  CLAMP;}
		  if (da.x < -CLAMP) {
		    if (db.x-da.x != 0)
		      da.y -= (db.y-da.y)*(da.x+CLAMP)/(db.x-da.x);
		    da.x = -CLAMP;}
		  if (da.y >  CLAMP) {
		    if (db.y-da.y != 0)
		      da.x -= (db.x-da.x)*(da.y-CLAMP)/(db.y-da.y);
		    da.y =  CLAMP;}
		  if (da.y < -CLAMP) {
		    if (db.y-da.y != 0)
		      da.x -= (db.x-da.x)*(da.y+CLAMP)/(db.y-da.y);
		    da.y = -CLAMP;}

		  if (db.x >  CLAMP) {
		    if (da.x-db.x != 0)
		      db.y -= (da.y-db.y)*(db.x-CLAMP)/(da.x-db.x);
		    db.x =  CLAMP;}
		  if (db.x < -CLAMP) {
		    if (da.x-db.x != 0)
		      db.y -= (da.y-db.y)*(db.x+CLAMP)/(da.x-db.x);
		    db.x = -CLAMP;}
		  if (db.y >  CLAMP) {
		    if (da.y-db.y != 0)
		      db.x -= (da.x-db.x)*(db.y-CLAMP)/(da.y-db.y);
		    db.y =  CLAMP;}
		  if (db.y < -CLAMP) {
		    if (da.y-db.y != 0)
		      db.x -= (da.x-db.x)*(db.y+CLAMP)/(da.y-db.y);
		    db.y = -CLAMP;}

		  a.x = (int) rint(da.x);
		  a.y = (int) rint(da.y);
		  b.x = (int) rint(db.x);
		  b.y = (int) rint(db.y);
		}
		
		switch (c->type) {
		case DLINE:
		  XDrawLine(pl->dpy, pl->win, gc, a.x, a.y-2, a.x, a.y+2);
		  XDrawLine(pl->dpy, pl->win, gc, a.x-2, a.y, a.x+2, a.y);
		  XDrawLine(pl->dpy, pl->win, gc, b.x, b.y-2, b.x, b.y+2);
		  XDrawLine(pl->dpy, pl->win, gc, b.x-2, b.y, b.x+2, b.y);
		  /*fall through*/
		case LINE:
		  XDrawLine(pl->dpy, pl->win, gc, a.x, a.y, b.x, b.y);
		  break;
		case X:
		  XDrawLine(pl->dpy, pl->win, gc,
			    a.x - 2, a.y - 2, a.x + 2, a.y + 2);
		  XDrawLine(pl->dpy, pl->win, gc,
			    a.x - 2, a.y + 2, a.x + 2, a.y - 2);
		  break;
		case DOT:
		  /* Lines are much faster on some displays */
		  if ( SAVx == a.x && SAVy == a.y )
		    {
		      SAVc++;
		    }
		  else
		    {
		      SAVd++;
		      SAVx = a.x; SAVy = a.y;
#if 0
		      /*    
		            ***
			   *****
                           *****
                           *****
                            ***
		       */

		      XDrawLine(pl->dpy, pl->win, gc,a.x-1,a.y-2, a.x+1,a.y-2);
		      XDrawLine(pl->dpy, pl->win, gc,a.x-2,a.y-1, a.x+2,a.y-1);
		      XDrawLine(pl->dpy, pl->win, gc,a.x-2,a.y  , a.x+2,a.y  );
		      XDrawLine(pl->dpy, pl->win, gc,a.x-2,a.y+1, a.x+2,a.y+1);
		      XDrawLine(pl->dpy, pl->win, gc,a.x-1,a.y+2, a.x+1,a.y+2);
#else
#if 1
		      /*
			     *
			    ***
			     *
		       */


		      XDrawLine(pl->dpy, pl->win, gc, a.x, a.y-1, a.x, a.y+1);
		      XDrawLine(pl->dpy, pl->win, gc, a.x-1, a.y, a.x+1, a.y);
#else
		      /*
			     *                           
		       */

		      XDrawPoint(pl->dpy, pl->win, gc, a.x, a.y);
#endif		      
#endif
		    }
		  break;
		case PLUS:
		  XDrawLine(pl->dpy, pl->win, gc,
			    a.x, a.y - 2, a.x, a.y + 2);
		  XDrawLine(pl->dpy, pl->win, gc,
			    a.x - 2, a.y, a.x + 2, a.y);
		  break;
		case BOX:
		  { XSegment segs[4];
		    int x,y;
		    const int BOXRADIUS=3;

		    /* use XDrawSegments so that things can
		       still be collected into one big PolySegment by xlib
		       
		       --0-|
		       |   |
		       3   1
		       |   |
		       |-2--  */

		    x = a.x;
		    segs[0].x1 = segs[3].x1 = segs[3].x2 = x-BOXRADIUS;
		    segs[2].x2 = x-(BOXRADIUS-1);
		    segs[0].x2 = x+(BOXRADIUS-1);
		    segs[1].x1 = segs[1].x2 = segs[2].x1 = x+BOXRADIUS;

		    y = a.y;
		    segs[0].y1 = segs[0].y2 = segs[1].y1 = y-BOXRADIUS;
		    segs[3].y2 = y-(BOXRADIUS-1);
		    segs[1].y2 = y+(BOXRADIUS-1);
		    segs[2].y1 = segs[2].y2 = segs[3].y1 = y+BOXRADIUS;
		    
		    XDrawSegments(pl->dpy, pl->win, gc, segs, 4);
		  }
		  break;
		case DIAMOND:
		  { XSegment segs[4];
		    int x,y;
		    /*
		          /
		         1 \
		        /   2
		       \     \
		        0   / 
		         \ 3
			  /
			 
			  */
		    x = a.x;
		    y = a.y;
		    segs[0].x1 = x-1; segs[0].y1 = y+2;
		    segs[0].x2 = x-3; segs[0].y2 = y;
		    segs[1].x1 = x-2; segs[1].y1 = y-1;
		    segs[1].x2 = x;   segs[1].y2 = y-3;
		    segs[2].x1 = x+1; segs[2].y1 = y-2;
		    segs[2].x2 = x+3; segs[2].y2 = y;
		    segs[3].x1 = x+2; segs[3].y1 = y+1;
		    segs[3].x2 = x;   segs[3].y2 = y+3;
		    
		    XDrawSegments(pl->dpy, pl->win, gc, segs, 4);
		  }
		  break;
#define D (pl->thick? 6: 3)
		case UTICK:
		  XDrawLine(pl->dpy, pl->win, gc, a.x, a.y, a.x, a.y-D);
		  break;
		case DTICK:
		  XDrawLine(pl->dpy, pl->win, gc, a.x, a.y, a.x, a.y+D);
		  break;
		case RTICK:
		  XDrawLine(pl->dpy, pl->win, gc, a.x, a.y, a.x+D, a.y);
		  break;
		case LTICK:
		  XDrawLine(pl->dpy, pl->win, gc, a.x, a.y, a.x-D, a.y);
		  break;
		case HTICK:
		  XDrawLine(pl->dpy, pl->win, gc, a.x-D, a.y, a.x+D, a.y);
		  break;
		case VTICK:
		  XDrawLine(pl->dpy, pl->win, gc, a.x, a.y-D, a.x, a.y+D);
		  break;
#undef D
#define D (pl->thick? 4: 2)
		case UARROW:
		  XDrawLine(pl->dpy, pl->win, gc, a.x - D, a.y + D, a.x, a.y);
		  XDrawLine(pl->dpy, pl->win, gc, a.x + D, a.y + D, a.x, a.y);
		  break;
		case DARROW:
		  XDrawLine(pl->dpy, pl->win, gc, a.x - D, a.y - D, a.x, a.y);
		  XDrawLine(pl->dpy, pl->win, gc, a.x + D, a.y - D, a.x, a.y);
		  break;
		case RARROW:
		  XDrawLine(pl->dpy, pl->win, gc, a.x - D, a.y - D, a.x, a.y);
		  XDrawLine(pl->dpy, pl->win, gc, a.x - D, a.y + D, a.x, a.y);
		  break;
		case LARROW:
		  XDrawLine(pl->dpy, pl->win, gc, a.x + D, a.y - D, a.x, a.y);
		  XDrawLine(pl->dpy, pl->win, gc, a.x + D, a.y + D, a.x, a.y);
#undef D
		  break;
		case TEXT:
		case TITLE:
		case XLABEL:
		case YLABEL:
		  { 
		    lXPoint p;
		    int direction;
		    int font_ascent;
		    int font_descent;
		    XCharStruct xcs;
		    /* int height; */
		    int width;
		    int space = 5;

		    if (c->type == TITLE) {
		      p.x = (int) rint(pl->size.x / 2);
		      p.y = 0;
		      p = lXPoint_from_dXPoint(tomain(pl,
						      dXPoint_from_lXPoint(p)
						      ));
		      c->position = ABOVE;
		    } else if (c->type == XLABEL) {
		      p.x = (int) pl->size.x;
		      p.y = (int) pl->size.y + 22;
		      p = lXPoint_from_dXPoint(tomain(pl,
						      dXPoint_from_lXPoint(p)
						      ));
		      c->position = TO_THE_LEFT;
		    } else if (c->type == YLABEL) {
		      p.x = 0;
		      p.y = -5;
		      p = lXPoint_from_dXPoint(tomain(pl,
						      dXPoint_from_lXPoint(p)
						      ));
		      c->position = ABOVE;
		    } else
		      p = a;

		    XTextExtents(pl->font_struct, c->text, strlen(c->text),
				 &direction, &font_ascent, &font_descent, &xcs);
		    width = xcs.width;
		    /* height = font_ascent + font_descent; */
		    switch (c->position) {
		    case CENTERED:
		    case ABOVE:
		    case BELOW:        p.x -= width/2;       break;
		    case TO_THE_LEFT:  p.x -= width + space; break;
		    case TO_THE_RIGHT: p.x += space;         break;
		    default: panic("drawloop, case TEXT: unknown text positioning");
		    }
		    switch (c->position) {
		    case CENTERED:
		    case TO_THE_LEFT:
		    case TO_THE_RIGHT: p.y += font_ascent/2;        break;
		    case ABOVE:        p.y -= space + font_descent; break;
		    case BELOW:        p.y += space + font_ascent;  break;
		    default: panic("drawloop, case TEXT: unknown text positioning");
		    }

		    XDrawString(pl->dpy, pl->win, gc, p.x, p.y,
				c->text, strlen(c->text));
		  }
		  break;
		default:
		  panic("unknown command type");
		}
		/* if something has happened, stop drawing and go handle it */
		if (pl->state != NORMAL) XSync(pl->dpy,False);
		if (XEventsQueued(dpy, QueuedAlready) != 0) break;
		if (dpy2 != 0 &&
		    XEventsQueued(dpy2, QueuedAlready) != 0) break;
	      }
	  if (c == NULL) pl->clean = 1;
/* #define SAVE_PRINTOUTS */
#ifdef SAVE_PRINTOUTS
	  fprintf(stderr, "saved %d/%d %f%%\n",
		  SAVc, SAVc+SAVd, SAVc*100.0/(SAVc+SAVd) );
#endif
	}
      }
      if (visible_count == 0) break; /* will exit */
    }

    do {
      if (XPending(dpy) != 0) {
	XNextEvent(dpy, &event);	
	dpy_of_event = dpy;
	break;
      } else if (dpy2 != 0 && XPending(dpy2) != 0) {
	XNextEvent(dpy2, &event);
	dpy_of_event = dpy2;
	break;
      } else {
	fd_set fds;
	int maxfd_plus_1 = 0;
	int r;

	FD_ZERO(&fds);
	FD_SET(ConnectionNumber(dpy), &fds);
	maxfd_plus_1 = ConnectionNumber(dpy) + 1;
	if (dpy2 != 0) {
	  FD_SET(ConnectionNumber(dpy2), &fds);
	  maxfd_plus_1 = max(maxfd_plus_1, ConnectionNumber(dpy2)+1);
	}
	r = select (maxfd_plus_1, &fds, 0, 0, 0);
	if (r < 0) {
	  perror("select");
	  exit(1);
	}
      }
    } while(1);

    for (ALLPLOTTERS)
      if (pl->dpy == dpy_of_event
	  && pl->win == event.xany.window)
	break;
    if (pl == 0)
      continue; /* this happens when windows get deleted */

#if 0
    fprintf(stderr,"event %d\n", event.type);
#endif
    switch(event.type) {
    case Expose:
      pl->new_expose = 1;
      if (pl->pointer_marks_on_screen) {
	draw_pointer_marks(pl, pl->bacgc);
	pl->pointer_marks_on_screen = FALSE;
      }
      continue;
    case VisibilityNotify:
      pl->visibility = event.xvisibility.state;
      break;
    case ConfigureNotify:
      /* see if the window has changed size */
      if (pl->mainsize.x != event.xconfigure.width ||
	  pl->mainsize.y != event.xconfigure.height)
	{
	  pl->mainsize.x = event.xconfigure.width;
	  pl->mainsize.y = event.xconfigure.height;

	  pl->size_changed = 1;
	}
      break;
    case MapNotify:
      if (pl->mainsize.x == 0 && pl->mainsize.y == 0) {
	pl->mainsize.x = pl->xsh.width;
	pl->mainsize.y = pl->xsh.height;
	pl->size_changed = 1;
      }
      break;
    case ButtonPress:
      if (pl->pointer_marks_on_screen) {
	draw_pointer_marks(pl, pl->xorgc);
	pl->pointer_marks_on_screen = FALSE;
      }
      if (x_synch && y_synch) {
	PLOTTER savepl = pl;
	
	for (ALLPLOTTERS) {
	  if (pl == savepl) continue;
	  if (pl->pointer_marks_on_screen) {
	    draw_pointer_marks(pl, pl->xorgc);
	    pl->pointer_marks_on_screen = FALSE;
	  }
	}
	pl = savepl;
      }
      pl->buttonsdown += 1;
      switch (pl->state) {
      case NORMAL:
	switch(event.xbutton.button) {
	case Button1:
	  if (event.xbutton.state & ShiftMask)
	    pl->state = PRINTING;
	  else
	    if (event.xbutton.y > pl->size.y + pl->origin.y)
	      pl->state = HZOOM;
	    else if (event.xbutton.x < pl->origin.x)
	      pl->state = VZOOM;
	    else
	      pl->state = ZOOM;
	  break;
	case Button2:
	  if (event.xbutton.state & ShiftMask)
	    pl->state = FIGING;
	  else
	    if (event.xbutton.y > pl->size.y + pl->origin.y)
	      pl->state = HDRAG;
	    else if (event.xbutton.x < pl->origin.x)
	      pl->state = VDRAG;
	    else
	      pl->state = DRAG;
	  break;
	case Button3:
	  if (event.xbutton.state & ControlMask) {
	    pl->state = EXITING;
	  } else {
	    if (option_one_at_a_time) {
	      if (event.xbutton.state & ShiftMask) {
		pl->state = BACKINGUP;
	      } else {
		pl->state = ADVANCING;
	      }
	    } else {
	      if (event.xbutton.state & ShiftMask) {
		pl->state = THINFIGING;
	      } else {
		pl->state = EXITING;
	      }
	    }
	  }
	  break;
	default:
	  pl->state = WEDGED;
	}

	/***** should draw rectangle for HZOOM & VZOOM
	  For HZOOM, should setup dragend.y with opposite end of
	  desired rectangle and leave it be during the rest of the
	  HZOOM operation.  (same for VZOOM) 

	  should rethink HDRAG and VDRAG visual feedback
	  (maybe use filled solid rectangles drawn in xor mode?)

	  */


	pl->raw_dragstart.x = event.xbutton.x;
	pl->raw_dragstart.y = event.xbutton.y;

	pl->dragstart = pl->raw_dragstart;
	switch(pl->state) {
	case HZOOM:
	  pl->dragstart.y = pl->origin.y;
	  break;
	case VZOOM:
	  pl->dragstart.x = pl->origin.x;
	  break;
	default:
	  break;
	}
	pl->dragstart = detent(pl, pl->dragstart);
	switch (pl->state) {
	case HDRAG:
	  pl->dragstart.y = pl->origin.y + pl->size.y - 1;
	  break;
	case VDRAG:
	  pl->dragstart.x = pl->origin.x;
	  break;
	default:
	  break;
	}
	if (XQueryPointer(pl->dpy, pl->win, &dummy_window, &dummy_window,
			  &dummy_int, &dummy_int,
			  &(pl->pointer.x), &(pl->pointer.y),
			  &dummy_unsigned_int)
	    == 0) {
	  pl->state = WEDGED;
	  break;
	}
	pl->pointer_marks = detent(pl, pl->pointer);

	pl->dragend = detent(pl, pl->pointer);
	switch(pl->state) {
	case HZOOM:
	  pl->dragend.y = pl->origin.y + pl->size.y;
	  break;
	case VZOOM:
	  pl->dragend.x = pl->origin.x + pl->size.x;
	  break;
	default:
	  break;
	}

	if (x_synch && y_synch) {
	  PLOTTER savepl = pl;

	  for (ALLPLOTTERS) {
	    if (pl == savepl) continue;

	    if (pl -> state == NORMAL) {
	      pl->state = SLAVE;
	      pl->master = savepl;
	      pl->master_state = savepl->state;
	      pl->dragstart = map_pl_pl(savepl, pl, savepl->dragstart);
	      pl->dragend = map_pl_pl(savepl, pl, savepl->dragend);
	    }
	  }
	  pl = savepl;
	}

	break;

	/* remaining cases of a button down event when not in state NORMAL
	   in each case, go to state WEDGED after cleaning up the state we
	   are coming from */
      case ZOOM:
      case HZOOM:
      case VZOOM:
      case HDRAG:
      case VDRAG:
      case DRAG:
	pl->state = WEDGED; /* this must be after undrawing on slaves */
	break;
      case EXITING:
      case PRINTING:
      case FIGING:
      case THINFIGING:
      case ADVANCING:
      case BACKINGUP:
	pl->state = WEDGED;
	break;
      case WEDGED:
	break;
      case SLAVE:
	XBell(pl->dpy,100);
	break;
      default:
	panic("unknown state");
	break;
      }
      
      draw_pointer_marks(pl, pl->xorgc);
      pl->pointer_marks_on_screen = TRUE;

      if (x_synch && y_synch) {
	PLOTTER savepl = pl;

	for (ALLPLOTTERS) {
	  if (pl == savepl) continue;

	  draw_pointer_marks(pl, pl->xorgc);
	  pl->pointer_marks_on_screen = TRUE;
	}
	pl = savepl;
      }
      
      break;
    case ButtonRelease:
      pl->buttonsdown -= 1;
      if (pl->state != SLAVE) {
	if (pl->pointer_marks_on_screen) {
	  draw_pointer_marks(pl, pl->xorgc);
	  pl->pointer_marks_on_screen = FALSE;
	}
	if (x_synch && y_synch) {
	  PLOTTER savepl = pl;
	  
	  for (ALLPLOTTERS) {
	    if (pl == savepl) continue;
	    if (pl->pointer_marks_on_screen) {
	      draw_pointer_marks(pl, pl->xorgc);
	      pl->pointer_marks_on_screen = FALSE;
	    }
	  }
	  pl = savepl;
	}
	
	if (x_synch && y_synch) {
	  PLOTTER savepl = pl;
	  for (ALLPLOTTERS) {
	    if (pl == savepl) continue;
	    if (pl->state == SLAVE) {
	      if (pl->buttonsdown == 0)
		pl->state = NORMAL;
	      else
		pl->state = WEDGED;
	    }
	  }
	  pl = savepl;
	}

      }

      switch (pl->state) {
      case ZOOM:
      case HZOOM:
      case VZOOM:
	pl->dragend.x = event.xbutton.x;
	pl->dragend.y = event.xbutton.y;
	switch(pl->state) {
	case HZOOM:
	  pl->dragend.y = pl->origin.y + pl->size.y;
	  break;
	case VZOOM:
	  pl->dragend.x = pl->origin.x + pl->size.x;
	  break;
	default:
	  break;
	}
	pl->dragend = detent(pl, pl->dragend);
	break;
      case HDRAG:
	pl->dragend.x = event.xbutton.x;
	pl->dragend.y = pl->dragstart.y;
	pl->dragend = detent(pl, pl->dragend);
	break;
      case VDRAG:
	pl->dragend.x = pl->dragstart.x;
	pl->dragend.y = event.xbutton.y;
	pl->dragend = detent(pl, pl->dragend);
	break;
      case DRAG:
	pl->dragend.x = event.xbutton.x;
	pl->dragend.y = event.xbutton.y;
	pl->dragend = detent(pl, pl->dragend);
	break;
      default:
	break;
      }
      switch (pl->state) {
      case ZOOM:
      case HZOOM:
      case VZOOM:
      case DRAG:
      case HDRAG:
      case VDRAG:
	pl->dragstart =
	  lXPoint_from_dXPoint(tosub(pl, dXPoint_from_lXPoint(pl->dragstart)));
	pl->dragend =
	  lXPoint_from_dXPoint(tosub(pl, dXPoint_from_lXPoint(pl->dragend)));
	break;
      default:
	break;
      }
      switch (pl->state) {
      case ZOOM:
      case HZOOM:
      case VZOOM:
	{
	  lXPoint dragstart, dragend;
	  PLOTTER savepl = pl;
	  bool must_pop_others = FALSE, do_x = FALSE, do_y = FALSE;

	  dragstart = pl->dragstart;
	  dragend = pl->dragend;

	  /* Do the plotter in which the button was pressed first, then do
	     all the others if we are synchronizing (and need to). */
	  if ((abs(pl->raw_dragstart.x - event.xbutton.x) > 7) ||
	      (abs(pl->raw_dragstart.y - event.xbutton.y) > 7)) {

	    if (((pl->state != VZOOM) && (abs(dragstart.x - dragend.x) > 7))
		|| ((pl->state != HZOOM) && (abs(dragstart.y - dragend.y) > 7))) {

	      int newviewno;
	      
	      newviewno = pl->viewno + 1;
	      if (newviewno == NUMVIEWS) newviewno -= 1;
	
	      if (abs(dragstart.x - dragend.x) > 7) {
		zoom_in_coord(pl->x_type, pl_x_left, pl_x_right,
			      dragstart.x, dragend.x,
			      pl->size.x,
			      &(pl->x_left[newviewno]),
			      &(pl->x_right[newviewno]));
		do_x = x_synch;
	      } else {
		pl->x_left[newviewno] = pl_x_left;
		pl->x_right[newviewno] = pl_x_right;
	      }
	      if (abs(dragstart.y - dragend.y) > 7) {
		zoom_in_coord(pl->y_type, pl_y_bottom, pl_y_top,
			      pl->size.y - dragstart.y,
			      pl->size.y - dragend.y,
			      pl->size.y,
			      &(pl->y_bottom[newviewno]),
			      &(pl->y_top[newviewno]));
		do_y = y_synch;
	      } else {
		pl->y_bottom[newviewno] = pl_y_bottom;
		pl->y_top[newviewno] = pl_y_top;
	      }
	      pl->viewno = newviewno;
	    } else {
	      /* do nothing  (don't zoom and don't pop) */
	      if (0) goto G0093;
	    }
	  } else {
	    if (pl->viewno > 1)
	      {
		/* Only pop the others if the synchronized axis changed. */
		if ((x_synch
		     && (xcmp(pl_x_left, pl->x_left[pl->viewno-1], !=)
			 || xcmp(pl_x_right, pl->x_right[pl->viewno-1], !=)))
		    || (y_synch
			&& (ycmp(pl_y_top, pl->y_top[pl->viewno-1], !=)
			    || ycmp(pl_y_bottom, pl->y_bottom[pl->viewno-1], !=))))
		  must_pop_others = TRUE;
		pl->viewno--;
	      }
	    else {
	      pl_x_left   = pl->x_left[0];
	      pl_x_right  = pl->x_right[0];
	      pl_y_top    = pl->y_top[0];
	      pl_y_bottom = pl->y_bottom[0];
	    }
	  }
	  XClearWindow(pl->dpy, pl->win);
	  pl->pointer_marks_on_screen = FALSE;
	  pl->size_changed = 1;
	  pl->state = NORMAL;
	  /* If we need to, loop through all the plotters. */
	  if (do_x || do_y || must_pop_others)
	    for (ALLPLOTTERS)
	      {
		if (pl == savepl) continue;
		if (must_pop_others)
		  {
		    if (pl->viewno > 1) pl->viewno--;
		    if (x_synch)
		      {
			pl_x_left   = savepl->x_left[savepl->viewno];
			pl_x_right  = savepl->x_right[savepl->viewno];
		      }
		    if (y_synch)
		      {
			pl_y_top    = savepl->y_top[savepl->viewno];
			pl_y_bottom = savepl->y_bottom[savepl->viewno];
		      }
		  }
		else
		  {
		    int newviewno;
		    newviewno = pl->viewno + 1;
		    if (newviewno == NUMVIEWS) newviewno -= 1;

		    if (do_x)  {
		      pl->x_left[newviewno] = savepl->x_left[savepl->viewno];
		      pl->x_right[newviewno] = savepl->x_right[savepl->viewno];
		    } else {
		      pl->x_left[newviewno] = pl_x_left;
		      pl->x_right[newviewno] = pl_x_right;
		    }
		    if (do_y)  {
		      pl->y_bottom[newviewno] =
			savepl->y_bottom[savepl->viewno];
		      pl->y_top[newviewno] = savepl->y_top[savepl->viewno];
		    } else {
		      pl->y_bottom[newviewno] = pl_y_bottom;
		      pl->y_top[newviewno] = pl_y_top;
		    }
		    pl->viewno = newviewno;

		    if ( do_x && ! y_synch)
		      shrink_to_bbox(pl,0,1);

		  }		
		pl->size_changed = 1;
	      }
	G0093:
	  pl = savepl;		/* Don't know if I have to do this, but... */
	}
        break;
      case DRAG:
      case HDRAG:
      case VDRAG:
	{
	  PLOTTER savepl = pl;

	  drag_coord(pl->x_type, pl_x_left, pl_x_right,
		     pl->dragend.x,
		     pl->dragstart.x,
		     pl->size.x,
		     &(pl_x_left), &(pl_x_right));
	  drag_coord(pl->y_type, pl_y_bottom, pl_y_top,
		     pl->size.y - pl->dragend.y,
		     pl->size.y - pl->dragstart.y,
		     pl->size.y,
		     &(pl_y_bottom), &(pl_y_top));
	
	  pl->size_changed = 1;
	  if (x_synch || y_synch)
	    for (ALLPLOTTERS)
	      {
		if (pl == savepl) continue;
		if (x_synch && savepl->state != VDRAG)
		  {
		    pl_x_left = savepl->x_left[savepl->viewno];
		    pl_x_right = savepl->x_right[savepl->viewno];
		    if (!y_synch) shrink_to_bbox(pl,0,1);
		    pl->size_changed = 1;
		  }
		if (y_synch && savepl->state != HDRAG)
		  {
		    pl_y_top = savepl->y_top[savepl->viewno];
		    pl_y_bottom = savepl->y_bottom[savepl->viewno];
		    if (!x_synch) shrink_to_bbox(pl,1,0);
		    pl->size_changed = 1;
		  }
	      }
	  pl = savepl;
	}
        XClearWindow(pl->dpy, pl->win);
	pl->pointer_marks_on_screen = FALSE;
        pl->state = NORMAL;
        break;
      case EXITING:
	pl->state = NORMAL;  
	undisplay_plotter(pl, 0);
	break;
      case PRINTING:
      case FIGING:
      case THINFIGING:
	{
	  FILE *fp, *make_name_open_file();
	  if ((fp = make_name_open_file(pl))) {
	    emit_PS(pl, fp, pl->state);
	    (void) fclose(fp);
	    pl->size_changed = 1;
	  }
	}
        pl->state = NORMAL;
        break;
      case ADVANCING:
	pl->state = NORMAL;
	undisplay_plotter(pl, -1);
	break;
      case BACKINGUP:
	pl->state = NORMAL;
	undisplay_plotter(pl, 1);
	break;
      case WEDGED:
	if (pl->buttonsdown == 0)
	  pl->state = NORMAL;
	break;
      case SLAVE:
	/* do nothing */
	break;
      default:
	/* Do nothing.  Note that this really can happen due to the semantics
	 * of the implicit grab associated with a button press event.
	 */
	break;
	
      }
      break;
    case EnterNotify:
      /*      printf("enter %d\n", pl->win); */
      break;
    case LeaveNotify:
      /*      printf("leave %d\n", pl->win); */
      if (pl->pointer_marks_on_screen) {
	draw_pointer_marks(pl, pl->xorgc);
	pl->pointer_marks_on_screen = FALSE;
      }
      if (x_synch && y_synch) {
	PLOTTER savepl = pl;

	for (ALLPLOTTERS) {
	  if (pl == savepl) continue;

	  if (pl->pointer_marks_on_screen) {
	    draw_pointer_marks(pl, pl->xorgc);
	    pl->pointer_marks_on_screen = FALSE;
	  }
	}
	pl = savepl;
      }

      break;
    case MotionNotify:
      if (pl->pointer_marks_on_screen) {
	draw_pointer_marks(pl, pl->xorgc);
	pl->pointer_marks_on_screen = FALSE;
      }
      if (XQueryPointer(pl->dpy, pl->win, &dummy_window, &dummy_window,
			&dummy_int, &dummy_int,
			&(pl->pointer.x), &(pl->pointer.y),
			&dummy_unsigned_int)
	  == 0) {
	pl->state = WEDGED;
	break;
      }
      if ( pl->size.x == 0 || pl->size.y == 0 )
	{
	  fprintf(stderr, "MotionNotify while size is zero ignored.\n");
	  break;
	}
      pl->pointer_marks = detent(pl, pl->pointer);

      if (pl->state != SLAVE) {
	pl->dragend = detent(pl, pl->pointer);
	switch(pl->state) {
	case HZOOM:
	  pl->dragend.y = pl->origin.y + pl->size.y;
	  break;
	case VZOOM:
	  pl->dragend.x = pl->origin.x + pl->size.x;
	  break;
	default:
	  break;
	}

      }

      draw_pointer_marks(pl, pl->xorgc);
      pl->pointer_marks_on_screen = TRUE;

      if (x_synch && y_synch) {
	PLOTTER savepl = pl;
	
	for (ALLPLOTTERS) {
	  if (pl == savepl) continue;
	  if (pl->win == 0) continue; /* -1 commandline option. Is this correct? */

	  if (pl->slave_draw_in_progress) {
	    pl->slave_motion_pending = TRUE;
	    pl->master_pointer = map_pl_pl(savepl, pl, savepl->pointer_marks);
	  } else {
	    if (pl->pointer_marks_on_screen) {
	      draw_pointer_marks(pl, pl->xorgc);
	      pl->pointer_marks_on_screen = FALSE;
	    }
	    pl->pointer_marks = map_pl_pl(savepl, pl, savepl->pointer_marks);
	    pl->dragend = pl->pointer_marks;
	    switch(pl->master_state) {
	    case HZOOM:
	      pl->dragend.y = pl->origin.y + pl->size.y;
	      break;
	    case VZOOM:
	      pl->dragend.x = pl->origin.x + pl->size.x;
	      break;
	    default:
	      break;
	    }
	    draw_pointer_marks(pl, pl->xorgc);
	    pl->pointer_marks_on_screen = TRUE;
	    pl->slave_draw_in_progress = TRUE;
	    {
	      XEvent e;
	      int r; 

	      e.xclient.type = ClientMessage;
	      e.xclient.serial = 0;
	      e.xclient.send_event = 1;
	      e.xclient.display = pl->dpy;
	      e.xclient.window = pl->win;
	      e.xclient.message_type = pl->xplot_nagle_atom;
	      e.xclient.format = 8;

	      r = XSendEvent(pl->dpy, pl->win, False, 0, &e);
	      if (r != 1)
		printf("DEBUG: XSendEvent returned %d\n", r);
	    }
	  }
	}
	pl = savepl;
      }
      
      break;
    case ClientMessage:
      if (event.xclient.message_type == pl->xplot_nagle_atom) {

#if 0
	printf("XPLOT_NAGLE\n");
#endif
	pl->slave_draw_in_progress = FALSE;

	if (pl->slave_motion_pending) {
	  if (pl->pointer_marks_on_screen) {
	    draw_pointer_marks(pl, pl->xorgc);
	    pl->pointer_marks_on_screen = FALSE;
	  }
	  pl->pointer_marks = pl->master_pointer;
	  pl->dragend = pl->master_pointer;
	  switch(pl->master_state) {
	  case HZOOM:
	    pl->dragend.y = pl->origin.y + pl->size.y;
	    break;
	  case VZOOM:
	    pl->dragend.x = pl->origin.x + pl->size.x;
	    break;
	  default:
	    break;
	  }
	  draw_pointer_marks(pl, pl->xorgc);
	  pl->pointer_marks_on_screen = TRUE;
	  pl->slave_motion_pending = FALSE;
	  pl->slave_draw_in_progress = TRUE;
	  {
	    XEvent e;
	    int r; 

	    e.xclient.type = ClientMessage;
	    e.xclient.serial = 0;
	    e.xclient.send_event = 1;
	    e.xclient.display = pl->dpy;
	    e.xclient.window = pl->win;
	    e.xclient.message_type = pl->xplot_nagle_atom;
	    e.xclient.format = 8;

	    r = XSendEvent(pl->dpy, pl->win, False, 0, &e);
	    if (r != 1)
	      printf("DEBUG: XSendEvent returned %d\n", r);
	  }
	}

      } else {
	printf("event ClientMessage unknown type %ld, %s\n",
	       event.xclient.message_type,
	       XGetAtomName(pl->dpy, event.xclient.message_type)
	       );
      }
      break;
    default:
#if 0      
      /* other events do happen (for example, ReparentNotify)
	 so just silently ignore them */ 
      printf("unknown event type %d\n", event.type);
#endif
      break;
    }
  } while (the_plotter_list != 0);
