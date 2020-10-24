scr_clear_tint(int bWithTinting)
{
    if( !TermWin.bMapped ) return ;
#ifdef TRANSPARENT
    if (Options & Opt_transparent)
    {
/*	int n ;
	fprintf( stderr, "\n Clearing Parents ...");
	for( n = 0 ; n < ParentWinNum ; n++ )
*/
        XClearWindow(Xdisplay, TermWin.parent);
/*	XFlush( Xdisplay );
	sleep(5);
	fprintf( stderr, "Done.");
*/    }
#endif
    XClearWindow(Xdisplay, TermWin.vt);

#if defined(BACKGROUND_IMAGE) || defined(TRANSPARENT)
    if (TermWin.tintGC && bWithTinting)
    {
            XFillRectangle(Xdisplay, TermWin.vt, TermWin.tintGC, 0, 0,
                           TermWin.width+TermWin_internalBorders, TermWin.height+TermWin_internalBorders);
    }
#endif
}
