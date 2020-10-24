void
rxvt_tabbar_expose (rxvt_t* r, XEvent *ev)
{
    Region region;
    
    UNSET_REGION(region);

    if( ev && ev->type == Expose)
    {
	region = XCreateRegion();

	do
	{
	    XRectangle rect;

	    rect.x	= ev->xexpose.x;
	    rect.y	= ev->xexpose.y;
	    rect.width	= ev->xexpose.width;
	    rect.height	= ev->xexpose.height;

	    XUnionRectWithRegion( &rect, region, region);
	} while( XCheckTypedWindowEvent( r->Xdisplay, r->tabBar.win,
		    Expose, ev));
    }
    else XClearWindow (r->Xdisplay, r->tabBar.win);

    /* draw the tabs and blank space*/
    rxvt_draw_tabs(r, region);

    /* draw the buttons */
    rxvt_tabbar_draw_buttons (r);

    if (IS_REGION(region))
	XDestroyRegion( region );
}