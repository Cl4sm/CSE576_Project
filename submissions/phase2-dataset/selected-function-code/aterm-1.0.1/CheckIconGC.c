CheckIconGC(IconGC* igc, Pixmap icon, Pixmap icon_mask)
{
  XGCValues values ;
  unsigned long valuemask = GCForeground|GCGraphicsExposures ;

    values.graphics_exposures = False;

    if( igc == NULL ) return ;
    if( igc->maskGC == None )
    {
        values.foreground = 1 ;
	igc->maskGC = XCreateGC( Xdisplay, icon_mask, valuemask, &values );
    }
    if( igc->maskGC_0 == None )
    {
        values.foreground = 0 ;
	igc->maskGC_0 = XCreateGC( Xdisplay, icon_mask, valuemask, &values );
    }
    if( igc->whiteGC == None )
    {
        values.foreground = whitePixel ;
	igc->whiteGC = XCreateGC( Xdisplay, icon, valuemask, &values );
    }
    if( igc->darkGC == None )
    {
        values.foreground = darkPixel ;
	igc->darkGC = XCreateGC( Xdisplay, icon, valuemask, &values );
    }
    if( igc->blackGC == None )
    {
        values.foreground = blackPixel ;
	igc->blackGC = XCreateGC( Xdisplay, icon, valuemask, &values );
    }
}
