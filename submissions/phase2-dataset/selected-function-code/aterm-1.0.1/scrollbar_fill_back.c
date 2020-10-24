Pixmap scrollbar_fill_back( unsigned int height, int check_cache )
{

  Pixmap buffer = None;
#ifdef TRANSPARENT
  static TransparencyCache tCache = {None, -1, -1, 0, None};
#endif

    /* create double buffer */
    buffer = XCreatePixmap(Xdisplay, scrollBar.win, SB_WIDTH+SB_BORDER_WIDTH, height, Xdepth);

#ifdef TRANSPARENT
    if( IS_TRANSP_SCROLL && scrollBar.state )
    {
      Pixmap root_pmap = None ;
      unsigned int root_width = 0, root_height = 0 ;
	if( check_cache == 1 )
	{
	  int cache_valid = 0 ;
    	    root_pmap = ValidatePixmap(root_pmap, 1, 1, &root_width, &root_height);
	    if( height == tCache.height && root_pmap == tCache.root )
	    {
	      int my_x, my_y ;
		if( GetWinPosition(scrollBar.win, &my_x, &my_y) )
		{
		    if( my_x== tCache.x && my_y == tCache.y ) cache_valid = 1 ;
		    else { tCache.x = my_x ; tCache.y = my_y ; }
		}else cache_valid = 1 ;
	    }else { tCache.root = root_pmap ; tCache.height = height ; }
	    if( cache_valid == 0 )
	    {
		if(tCache.cache) XFreePixmap( Xdisplay, tCache.cache );
		tCache.cache = CutWinPixmap( scrollBar.win, root_pmap, root_width, root_height, SB_WIDTH+SB_BORDER_WIDTH, height, grayGC, &(TermWin.background.Shading));
	    }
	}
	if( tCache.cache != None )
	{
	    FillPixmapWithTile( buffer, tCache.cache, 0, 0, SB_WIDTH+SB_BORDER_WIDTH, height, 0, 0 );
	    if( TermWin.tintGC)
    		XFillRectangle(Xdisplay, buffer, TermWin.tintGC, 0, 0, SB_WIDTH+SB_BORDER_WIDTH-1, height-1);
	    return buffer ;
	}
    }
#endif

    /* draw the background */
    XFillRectangle(Xdisplay, buffer, grayGC, 0, 0, SB_WIDTH+SB_BORDER_WIDTH, height);

    XSetForeground(Xdisplay, paintGC, blackPixel );
    XDrawRectangle(Xdisplay, buffer, paintGC, 0, 0, SB_WIDTH, height);

    if (TermWin.nscrolled > 0)
    {
        XFillRectangle( Xdisplay, buffer, stippleGC,
    	                SB_BORDER_WIDTH+SB_BORDER_WIDTH, SB_BORDER_WIDTH,
	                SB_WIDTH-SB_BORDER_SIZE-SIDE_STEP_WIDTH,
    	        	height-SB_BUTTONS_HEIGHT );

    }else {
        XFillRectangle(Xdisplay, buffer, stippleGC,
	           SB_BORDER_WIDTH+SB_BORDER_WIDTH, SB_BORDER_WIDTH,
	           SB_WIDTH-SB_BORDER_SIZE, height-SB_BORDER_SIZE);
    }

    return buffer ;
}
