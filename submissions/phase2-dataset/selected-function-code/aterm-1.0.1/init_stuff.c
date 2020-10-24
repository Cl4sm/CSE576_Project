init_stuff(void)
{
    XGCValues gcvalue;
    XColor xcol;
    Pixmap stipple;
    unsigned long light;
    unsigned arrow_x_offset, arrow_y_offset ;
    IconGC icongc = {None,None,None,None};
    int i ;

    gcvalue.graphics_exposures = False;

    blackPixel = BlackPixelOfScreen(DefaultScreenOfDisplay(Xdisplay));
    whitePixel = WhitePixelOfScreen(DefaultScreenOfDisplay(Xdisplay));

    xcol.red = 0xaeba;
    xcol.green = 0xaaaa;
    xcol.blue = 0xaeba;
    if (!XAllocColor (Xdisplay, Xcmap, &xcol))
    {
	print_error ("can't allocate %s", "light gray");
#ifndef NO_BRIGHTCOLOR
	xcol.pixel = PixColors [Color_AntiqueWhite];
#else
	xcol.pixel = PixColors [Color_White];
#endif
    }

    light = gcvalue.foreground = xcol.pixel;
    grayGC =  XCreateGC(Xdisplay, scrollBar.win, GCForeground|GCGraphicsExposures,
			&gcvalue);

    xcol.red = 0x51aa;
    xcol.green = 0x5555;
    xcol.blue = 0x5144;
    if (!XAllocColor (Xdisplay, Xcmap, &xcol))
    {
	print_error ("can't allocate %s", "dark gray");
#ifndef NO_BRIGHTCOLOR
	xcol.pixel = PixColors [Color_Grey25];
#else
	xcol.pixel = PixColors [Color_Black];
#endif

    }

    darkPixel = xcol.pixel;

    renderIcon(SCROLLER_DIMPLE, &dimple, &icongc);

    if( !NeXTScrollArrows.bValid ) init_scroll_size();

    arrow_x_offset = (ARROW_SOURCE_WIDTH-ARROW_WIDTH)>>1;
#ifdef NEXT_SCROLL_SQUARE_ARROWS
    arrow_y_offset = arrow_x_offset ;
#else
    arrow_y_offset = 0; /* not implemented yet */
#endif

    for( i = 0 ; i < 4 ; i++ )
    {
	NeXTScrollArrows.Arrows[i].origin_x = arrow_x_offset ;
	NeXTScrollArrows.Arrows[i].origin_y = arrow_y_offset ;
	NeXTScrollArrows.Arrows[i].width = ARROW_WIDTH ;
	NeXTScrollArrows.Arrows[i].height = ARROW_HEIGHT ;
	renderIcon(NeXTScrollArrows.Data[i], &(NeXTScrollArrows.Arrows[i]), &icongc );
    }

    FreeIconGC( &icongc );

    gcvalue.foreground = whitePixel;
    paintGC = XCreateGC(Xdisplay, scrollBar.win, GCForeground|GCGraphicsExposures,
			&gcvalue);

    stipple = XCreateBitmapFromData(Xdisplay, scrollBar.win,
				    stp_bits, stp_width, stp_height);
    gcvalue.foreground = darkPixel;
    gcvalue.background = light;
    gcvalue.fill_style = FillStippled;
    gcvalue.stipple = stipple;

    stippleGC = XCreateGC(Xdisplay, scrollBar.win, GCForeground|GCBackground
			  |GCStipple|GCFillStyle|GCGraphicsExposures,
			  &gcvalue);

    scrollbar_show(1);
}
