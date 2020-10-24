Bool 
TransparentPixmapNeedsUpdate()
{
#ifdef DEBUG_BACKGROUND_PMAP	
	fprintf( stderr, "%s: checking if transparent\n", __FUNCTION__ );
#endif
	if( !IsTransparentPixmap() ) 
		return False;

#ifdef DEBUG_BACKGROUND_PMAP	   
	fprintf( stderr, "%s: checking if same desktop\n", __FUNCTION__ );
#endif
	if( ExtWM.current_desktop != ExtWM.aterm_desktop && ExtWM.aterm_desktop != 0xFFFFFFFF &&
		get_flags( ExtWM.flags, WM_SupportsDesktops )&& !get_flags( ExtWM.flags, WM_AtermStateSticky ) ) 	   
		return False;

	if( get_flags( ExtWM.flags, WM_AtermStateShaded|WM_AtermStateHidden ) ) 	   
		return False;

#ifdef DEBUG_BACKGROUND_PMAP	   
	fprintf( stderr, "%s: checking if pixmap/position changed\n", __FUNCTION__ );
	fprintf( stderr, "%s: last pixmap = %lX new pixmap = %lX\n", __FUNCTION__, TermWin.LastPixmapUsed, BG.srcPixmap );
#endif
	if( TermWin.LastPixmapUsed == BG.srcPixmap &&
		TermWin.LastPixmap_root_x == TermWin.root_x && 
		TermWin.LastPixmap_root_y == TermWin.root_y &&
		TermWin.LastPixmap_width  == TermWin.root_width &&
		TermWin.LastPixmap_height == TermWin.root_height )
		return False; 
#ifdef DEBUG_BACKGROUND_PMAP	   
   	fprintf( stderr, "%s: checking if visible\n", __FUNCTION__ );
#endif
	if( TermWin.root_x+TermWin.root_width <= 0 || TermWin.root_y+TermWin.root_height <= 0 ||
		TermWin.root_x >= XdisplayWidth || TermWin.root_y >= XdisplayHeight )
		return False;
#ifdef DEBUG_BACKGROUND_PMAP	   
	fprintf( stderr, "%s: all clear - needs update\n", __FUNCTION__ );
#endif

	return True;		   
}	 
