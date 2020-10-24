FreeIconGC( IconGC* igc )
{
    if( igc )
    {
	if( igc->maskGC != None )
	{
	    XFreeGC( Xdisplay, igc->maskGC );
	    igc->maskGC  = None;
	}
	if( igc->maskGC_0 != None )
	{
	    XFreeGC( Xdisplay, igc->maskGC_0 );
	    igc->maskGC_0  = None;
	}
	if( igc->whiteGC == None )
	{
	    XFreeGC( Xdisplay, igc->whiteGC );
	    igc->whiteGC  = None;
	}
	if( igc->darkGC == None )
	{
	    XFreeGC( Xdisplay, igc->darkGC );
	    igc->darkGC  = None;
	}
        if( igc->blackGC != None )
	{
	    XFreeGC( Xdisplay, igc->blackGC );
	    igc->blackGC  = None;
	}
    }
}
