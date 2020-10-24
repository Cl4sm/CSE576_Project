void
resize_window(XEvent* ev)
{
	int root_x = 0, root_y = 0; 
	Window        wdumm;
	XConfigureEvent *xconf = &(ev->xconfigure);
	
	while( XCheckTypedWindowEvent( Xdisplay, TermWin.parent, ConfigureNotify, ev ) );
	/*fprintf( stderr, "config_geom = %dx%d\n", xconf->width, xconf->height );*/
    resize_window1(xconf->width, xconf->height);
#if 1
	XTranslateCoordinates (Xdisplay, TermWin.parent, Xroot, 0, 0, &root_x, &root_y, &wdumm);

	/*fprintf( stderr, "root_geom = %dx%d%+d%+d, root_size = %dx%d\n", xconf->width, xconf->height, root_x, root_y, XdisplayWidth, XdisplayHeight ); */
	TermWin.root_x = root_x ; 
	TermWin.root_y = root_y ; 
	TermWin.root_width = xconf->width ; 
	TermWin.root_height = xconf->height ; 
	
	if( TransparentPixmapNeedsUpdate() )
		request_background_update();
	else
	{	
		Bool cancel = True ;
		Bool request = False ;

#ifdef _MYSTYLE_
		if( TermWin.background.trgType == BGT_MyStyle ) 
		{	
			cancel = False ;	
			if( TermWin.LastPixmap_width  != TermWin.root_width ||
				TermWin.LastPixmap_height != TermWin.root_height )
				request = True;
		}

#endif	
#ifdef HAVE_AFTERIMAGE
		if( TermWin.background.trgType == BGT_Scale || 
			TermWin.background.trgType == BGT_ScaleH || 
			TermWin.background.trgType == BGT_ScaleV || 
			TermWin.background.trgType == BGT_Cut ) 
		{
			cancel = False ;	
			if( TermWin.LastPixmap_width  != TermWin.root_width ||
				TermWin.LastPixmap_height != TermWin.root_height )
				request = True;
		}
#endif	
		if( cancel ) 	   
			cancel_background_update();
		else if( request )
			request_background_update();
	}
	
#else 
	request_background_update();
#endif
#if 0
#if defined(TRANSPARENT) || defined(BACKGROUND_IMAGE) || defined(_MYSTYLE_)
    XGetGeometry(Xdisplay, TermWin.vt,
		 &root, &x, &y, &vt_width, &vt_height, &border, &depth);

    refresh_transparent_scrollbar();

    if( (TermWin.bMapped && (Options & Opt_transparent)) ||
         TermWin.background.trgType != BGT_None )
    {
	  int abs_x, abs_y;
          static int old_abs_x=0, old_abs_y=0,
	             old_width=0, old_height=0;

	    if( GetMyPosition(&abs_x, &abs_y))
	    {
/*	    fprintf( stderr, "\naterm:resize_windows():old = %dx%d+%d+%d, new = %dx%d+%d+%d",
		     old_width, old_height, old_abs_x, old_abs_y,
		     vt_width, vt_height, abs_x, abs_y   );
*/
#ifdef _MYSTYLE_
	        if( TermWin.background.trgType == BGT_MyStyle )
		{ /* we want to avoid unneeded redraws if we
		     only have gradients */
		    if( TransparentMS(TermWin.background.mystyle) || vt_width != old_width || vt_height!= old_height )
			{
			    RenderPixmap(1);
			    old_width = vt_width ;
			    old_height = vt_height ;
			}
		    RenderPixmap(1);
	        scr_clear();
			scr_touch();
			return ;
		}
#endif
		if( TermWin.background.trgType != BGT_None )
		{
		    ValidateSrcPixmap( 1 );
	    	    if( abs_x != old_abs_x || abs_y != old_abs_y ||
	    		vt_width != old_width || vt_height!= old_height ||
			TermWin.LastPixmapUsed != TermWin.background.srcPixmap )
		    {
    			RenderPixmap(1);
			/* we've already validated source pixmap
			   of course user can change it in any time
			   but for performance considerations we
			   don't want to recheck it*/
    			scr_clear();
            		scr_touch();

			old_width = vt_width ;
			old_height = vt_height ;
            		old_abs_x = abs_x ;
			old_abs_y = abs_y ;
		    }
		}
#ifdef TRANSPARENT
		else if( abs_x != old_abs_x || abs_y != old_abs_y )
		{
		    scr_clear();
        	    scr_touch();

        	    old_abs_x = abs_x ;
		    old_abs_y = abs_y ;
		}
#endif
	    }
        }
    }
    else
#endif
#endif
    {
        scr_clear();
        scr_touch();
    }
}
