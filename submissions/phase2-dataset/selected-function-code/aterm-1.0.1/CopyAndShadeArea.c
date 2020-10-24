void CopyAndShadeArea( Drawable src, Pixmap trg,
                       int x, int y, int w, int h,
		       int trg_x, int trg_y,
		       GC gc, ShadingInfo* shading )
{
  int (*oldXErrorHandler) (Display *, XErrorEvent *) ;
   /* we need to check if pixmap is still valid */
    oldXErrorHandler = XSetErrorHandler (pixmap_error_handler);

    if( shading )
    {
      XImage* img ;

        if( x <0 || y <0  ) return ;
        if((img = XGetImage (Xdisplay, src, x, y, w, h, AllPlanes, ZPixmap))!= NULL )
        {
    	    ShadeXImage( img, shading, gc );
    	    XPutImage(Xdisplay, trg, gc, img, 0, 0, trg_x, trg_y, w, h);
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
		printf ("CopyAndShadeArea(): %d shading runs took %.0f seconds\n", BENCHMARK_SHADING, diff);
	    }
#endif
	    XDestroyImage( img );
	    return ;
	}
    }
    if( !XCopyArea (Xdisplay, src, trg, gc, x, y, w, h, trg_x, trg_y))
		XFillRectangle( Xdisplay, trg, gc, trg_x, trg_y, w, h );
    XSetErrorHandler (oldXErrorHandler);
}
