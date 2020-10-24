scrollbar_show_cached(int update, int check_cache)
{
    /* old (drawn) values */
    static int last_top, last_bot, last_len;
    static int scrollbar_len;		/* length of slider */
    Pixmap buffer;
    int height = scrollBar.end + SB_BUTTONS_HEIGHT+sb_shadow ;

    if (paintGC == None)
	init_stuff();

    if (update)
    {
	int             top = (TermWin.nscrolled - TermWin.view_start);
	int             bot = top + (TermWin.nrow - 1);
	int             len = max((TermWin.nscrolled + (TermWin.nrow - 1)),1);

#define MIN_SCROLL_LENGTH (SCROLLER_DIMPLE_HEIGHT+3)
	scrollBar.top = (scrollBar.beg +
			 (top * (scrollbar_size()-SB_BORDER_SIZE-MIN_SCROLL_LENGTH)) / len);
	scrollBar.bot = (scrollBar.beg + MIN_SCROLL_LENGTH+
			 (bot * (scrollbar_size()-SB_BORDER_SIZE-MIN_SCROLL_LENGTH)) / len);

	scrollbar_len = scrollBar.bot - scrollBar.top;
    /* no change */
	if ((scrollBar.top == last_top) && (scrollBar.bot == last_bot))
	    return 0;

    }

    last_top = scrollBar.top;
    last_bot = scrollBar.bot;
    last_len = scrollbar_len;

    buffer = scrollbar_fill_back( height, check_cache );
    if (TermWin.nscrolled > 0)
    {
#ifdef TRANSPARENT
	if( !IS_TRANSP_SCROLL )
	{
	    XFillRectangle(Xdisplay, buffer, grayGC,
		       SB_BORDER_WIDTH+BEVEL_HI_WIDTH,
	               scrollBar.top+BEVEL_HI_WIDTH+SB_BORDER_WIDTH-SIDE_STEP_WIDTH,
	               SB_WIDTH-SB_BORDER_WIDTH-BEVEL_LO_WIDTH, scrollbar_len);
	}
#endif
	drawBevel(buffer, SB_BORDER_WIDTH+BEVEL_HI_WIDTH,
			  scrollBar.top+BEVEL_HI_WIDTH+SB_BORDER_WIDTH,
			  SB_WIDTH-SB_BORDER_SIZE-SIDE_STEP_WIDTH,
			  scrollbar_len);

	drawBevel(buffer, SB_BORDER_WIDTH+BEVEL_HI_WIDTH,
			  height-SB_BUTTONS_HEIGHT+BEVEL_HI_WIDTH-SB_BORDER_WIDTH,
			  SB_WIDTH-SB_BORDER_SIZE-SIDE_STEP_WIDTH,
			  SB_BUTTON_HEIGHT);
	drawBevel(buffer, SB_BORDER_WIDTH+BEVEL_HI_WIDTH,
			  height-SB_BUTTON_HEIGHT+BEVEL_HI_WIDTH-SB_BORDER_WIDTH,
			  SB_WIDTH-SB_BORDER_SIZE-SIDE_STEP_WIDTH,
			  SB_BUTTON_HEIGHT);

	PlaceIcon(&dimple,
		  ((SB_WIDTH)>>1)-(SCROLLER_DIMPLE_WIDTH>>1)+1-SIDE_STEP_WIDTH,
		  scrollBar.top + BEVEL_HI_WIDTH + SB_BORDER_WIDTH +
		  ((scrollbar_len-SCROLLER_DIMPLE_HEIGHT)>>1),
		  buffer );

	PlaceIcon((scrollbar_isUp())?&UP_ARROW_HI:&UP_ARROW,
		  ((SB_WIDTH)>>1)-(ARROW_WIDTH>>1)+1-SIDE_STEP_WIDTH,
		  height - SB_BUTTONS_HEIGHT + BEVEL_HI_WIDTH+1-SB_BORDER_WIDTH,
		  buffer );

	PlaceIcon((scrollbar_isDn())?&DOWN_ARROW_HI:&DOWN_ARROW,
		  ((SB_WIDTH)>>1)-(ARROW_WIDTH>>1)+1-SIDE_STEP_WIDTH,
		  height - SB_BUTTON_HEIGHT+BEVEL_HI_WIDTH+1-SB_BORDER_WIDTH,
		  buffer );

#ifdef TRANSPARENT
	if( IS_TRANSP_SCROLL )
	    XSetClipMask( Xdisplay, paintGC, None );
#endif

    }

    if (Options & Opt_scrollBar_right)
	XCopyArea(Xdisplay, buffer, scrollBar.win, paintGC, 0, 0,
		  SB_WIDTH+SB_BORDER_WIDTH, height, 0, 0);
    else
	XCopyArea(Xdisplay, buffer, scrollBar.win, paintGC, 0, 0,
		  SB_WIDTH+SB_BORDER_WIDTH, height, 0-SB_BORDER_WIDTH, 0);

    XFreePixmap(Xdisplay, buffer);

    return 1;
}
