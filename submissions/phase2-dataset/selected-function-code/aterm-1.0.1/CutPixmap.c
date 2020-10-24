CutPixmap ( Pixmap src, Pixmap trg,
            int x, int y,
	    unsigned int src_w, unsigned int src_h,
	    unsigned int width, unsigned int height,
	    GC gc, ShadingInfo * shading)
{
  Bool my_pixmap = (trg == None )?True:False ;
  int screen_w, screen_h ;
  int offset_x = 0, offset_y = 0 ;

  screen_w = DisplayWidth( Xdisplay, Xscreen );
  screen_h = DisplayHeight( Xdisplay, Xscreen );

  while( x+(int)width < 0 )  x+= screen_w ;
  while( x >= screen_w )  x-= screen_w ;
  while( y+(int)height < 0 )  y+= screen_h ;
  while( y >= screen_h )  y-= screen_h ;

  if (width < 2 || height < 2 )
    return trg;
  if( x < 0 )
  {
    offset_x = (-x) ;
    x = 0 ;
    width -= offset_x ;
  }
  if( y < 0 )
  {
    offset_y = (-y) ;
    y = 0 ;
    height -= offset_y ;
  }
  if( x+width >= screen_w ) width = screen_w - x ;
  if( y+height >= screen_h ) height = screen_h - y ;

  if (src == None) /* we don't have root pixmap ID */
    { /* we want to create Overrideredirect window overlapping out window
         with background type of Parent Relative and then grab it */
     XSetWindowAttributes attr ;
     XEvent event ;
     int tick_count = 0 ;
     Bool grabbed = False ;
        attr.background_pixmap = ParentRelative ;
	attr.backing_store = Always ;
	attr.event_mask = ExposureMask ;
	attr.override_redirect = True ;
	src = XCreateWindow(Xdisplay, Xroot, x, y, width, height,
	                    0,
			    CopyFromParent, CopyFromParent, CopyFromParent,
			    CWBackPixmap|CWBackingStore|CWOverrideRedirect|CWEventMask,
			    &attr);

	if( src == None ) return trg ;
	XGrabServer( Xdisplay );
	grabbed = True ;
	XMapRaised( Xdisplay, src );
	XSync(Xdisplay, False );
	/* now we have to wait for our window to become mapped - waiting for Expose */
	for( tick_count = 0 ; !XCheckWindowEvent( Xdisplay, src, ExposureMask, &event ) && tick_count < 100 ; tick_count++)
	    sleep_a_little(100);

	if( tick_count < 100 )
	{
	    if( trg == None )    trg = CREATE_TRG_PIXMAP (width+offset_x, height+offset_y);
	    if (trg != None)
	    {	/* custom code to cut area, so to ungrab server ASAP */
	        if (shading)
	        {
	          XImage *img;
		  img = XGetImage (Xdisplay, src, 0, 0, width, height, AllPlanes, ZPixmap);
    		  XDestroyWindow( Xdisplay, src );
	          src = None ;
		  XUngrabServer( Xdisplay );
		  grabbed = False ;
		  if (img != NULL)
		  {
    		    ShadeXImage (img, shading, gc);
		    XPutImage (Xdisplay, trg, gc, img, 0, 0, offset_x, offset_y, width, height);
#ifdef BENCHMARK_SHADING
	    {
		int i;
		time_t before, after;
		double diff;

		before = time (NULL);
		for (i = 0; i < BENCHMARK_SHADING; i++)
		    ShadeXImage (img, shading, gc);
		after = time (NULL);

		diff = difftime (after, before);
		printf ("CutPixmap(): %d shading runs took %.0f seconds\n", BENCHMARK_SHADING, diff);
	    }
#endif
		    XDestroyImage (img);
		  }else if( my_pixmap )
		  {

		    XFreePixmap( Xdisplay, trg );
		    trg = None ;
		  }
		}else
		    XCopyArea (Xdisplay, src, trg, gc, 0, 0, width, height, offset_x, offset_y);
	    }
        }

	if( src )
	    XDestroyWindow( Xdisplay, src );
	if( grabbed )
	    XUngrabServer( Xdisplay );
	return trg ;
    }
  /* we have root pixmap ID */
  /* find out our coordinates relative to the root window */
  if (x + width > src_w || y + height > src_h)
    {			/* tiled pixmap processing here */
      Pixmap tmp ;
      width = min (width, src_w);
      height = min (height, src_h);

      tmp = CREATE_TRG_PIXMAP (width, height);
      if (tmp != None)
      {
        ShadeTiledPixmap (src, tmp, src_w, src_h, x, y, width,
			  height, gc, shading);
        if( trg == None )
           trg = CREATE_TRG_PIXMAP (width+offset_x, height+offset_y);
	if( trg != None )
	    XCopyArea (Xdisplay, tmp, trg, gc, 0, 0, width, height, offset_x, offset_y);

	XFreePixmap( Xdisplay, tmp );
        return trg;
      }
    }

  /* create target pixmap of the size of the window */
  if( trg == None )    trg = CREATE_TRG_PIXMAP (width+offset_x, height+offset_y);
  if (trg != None)
    {
      /* cut area */
      CopyAndShadeArea (src, trg, x, y, width, height, offset_x, offset_y, gc, shading);

    }

  return trg;
}
