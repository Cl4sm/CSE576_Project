void
RenderPixmap(int DontCheckSource )
{
	XGCValues       gcvalue;
	GC              gc;
	int    width = TermWin_TotalWidth();
	int    height = TermWin_TotalHeight();
 	unsigned int    fin_width, fin_height ;
	int (*oldXErrorHandler) (Display *, XErrorEvent *) ;

   	set_background_updated();	
	
    /* we have some nice processing of all the X errors built in */
    /* so let's not let us crash if anything goes wrong          */
    oldXErrorHandler = XSetErrorHandler (pixmap_error_handler);
#ifdef DEBUG_BACKGROUND_PMAP	   
	XSetErrorHandler (oldXErrorHandler);
#endif	

    if( !DontCheckSource )  ValidateSrcPixmap( 0 );

	if( IsTransparentPixmap() )
	{
		if( ExtWM.current_desktop != ExtWM.aterm_desktop &&
			get_flags( ExtWM.flags, WM_SupportsDesktops ) ) 	   
		{
			XSetErrorHandler (oldXErrorHandler);
			return;
		}		
	}		   



    if( BG.srcPixmap == None
#ifdef _MYSTYLE_
	&& BG.trgType != BGT_MyStyle
#endif
      )
    {
#ifdef TRANSPARENT
		if(!(Options & Opt_transparent) || BG.trgType == BGT_None)
#endif
		{
	    	XSetErrorHandler (oldXErrorHandler);
	    	return ; /* nothing to do here */
		}
		if( DontCheckSource )  
			ValidateSrcPixmap( 0 );
    }
    
	/* for convinience only */
    fin_width = width ;
    fin_height = height ;
/*fprintf(stderr, "\n aterm: entering RenderPixmap, window size is %dx%d, trg_type = %d", width, height, BG.trgType );
*/
    gcvalue.foreground = PixColors[Color_bg];
#ifdef HAVE_AFTERIMAGE
    gc = create_visual_gc(asv, TermWin.vt, GCForeground, &gcvalue);
#else
	gc = XCreateGC(Xdisplay, TermWin.vt, GCForeground, &gcvalue);
#endif


/*fprintf(stderr, "\n aterm RenderPixmap(): freeing target pixmap ...");
*/
    if( BG.trgPixmap != BG.srcPixmap )	
		FreeTargetPixmap();
    
#define SHADING ((BG.bMySource)?NULL:&(BG.Shading))
	
	if( BG.trgType == BGT_Tile )  /* just copying source PixampID into trgPixmapID */
	{	
		if( BG.bMySource || NO_NEED_TO_SHADE(BG.Shading))
		{
    		    BG.trgPixmap = BG.srcPixmap ;
		    fin_width = BG.Width ;
		    fin_height = BG.Height;
		}else if( (BG.finWidth != width || BG.finHeight!=height ) &&
			  	  (BG.Width != BG.finWidth || BG.Height != BG.finHeight))
		{
		    fin_width = min(BG.Width, width);
		    fin_height = min(BG.Height, height);
		    BG.trgPixmap = ShadePixmap(BG.srcPixmap, 0, 0, fin_width, fin_height, gc, SHADING);
		}
	}
#ifdef HAVE_AFTERIMAGE
	else if( BG.trgType == BGT_Scale )
	{	
		if( BG.Width != width || BG.Height!=height )
		{
#ifdef DEBUG_BACKGROUND_PMAP	   
fprintf(stderr,  "aterm RenderPixmap(): (BGT_Scale) src pixmap is [%lX], scaling from %dx%d to %dx%d \n",BG.srcPixmap, BG.Width, BG.Height, width, height);
#endif
	    	    BG.trgPixmap = ScalePixmap( BG.srcPixmap,
											BG.Width, BG.Height,
		                                	width, height, gc, SHADING );
				fin_width = width ; 
				fin_height = height ; 
#ifdef DEBUG_BACKGROUND_PMAP	   
fprintf(stderr, "aterm RenderPixmap(): trg pixmap is [%lX]\n",BG.trgPixmap);		
#endif
		}
	}else if( BG.trgType == BGT_ScaleH )
	{	
		if( BG.Width != width )
		{
	    	    BG.trgPixmap = ScalePixmap( BG.srcPixmap,
						BG.Width, BG.Height,
		                                width, BG.Height, gc, SHADING );
			    fin_height = BG.Height;
				fin_width = width ; 
		}
	}else if( BG.trgType == BGT_ScaleV )
	{	
		if( BG.Height!=height )
		{
#ifdef DEBUG_BACKGROUND_PMAP	   
fprintf(stderr,  "aterm RenderPixmap(): (BGT_Scale) src pixmap is [%lX], scaling from %dx%d to %dx%d \n",BG.srcPixmap, BG.Width, BG.Height, BG.Width, height);
#endif
	    	    BG.trgPixmap = ScalePixmap( BG.srcPixmap,
						BG.Width, BG.Height,
		                                BG.Width, height, gc, SHADING );
		    fin_width = BG.Width ;
			fin_height = height ; 
		}
	}
#endif
	else if( BG.trgType == BGT_Cut )
	{	
#ifdef DEBUG_BACKGROUND_PMAP	   
		fprintf(stderr, "aterm RenderPixmap(): (BG_Cut)src pixmap is [%lX] %dx%d %dx%d\n", BG.srcPixmap, BG.Width, BG.Height, width, height);
#endif
		BG.trgPixmap = CutWinPixmap( TermWin.vt, BG.srcPixmap,
					  				 BG.Width, BG.Height, width, height, gc,
					  				 SHADING );
#ifdef DEBUG_BACKGROUND_PMAP	   
		fprintf(stderr, "aterm RenderPixmap(): (BG_Cut)trg pixmap is [%lX]\n", BG.trgPixmap);
#endif
	}		
#ifdef _MYSTYLE_
	else if( BG.trgType == BGT_MyStyle )
	{	
		BG.trgPixmap = RenderMyStylePixmap( BG.mystyle, BG.srcPixmap,
					            BG.Width, BG.Height,
		                                    width, height );
		if( BG.mystyle->texture_type == TEXTURE_PIXMAP)
		{
		    BG.srcPixmap = BG.mystyle->back_icon.pix ; /* so not to free it later */
		    BG.bMySource = False ;
		}

		/* fprintf(stderr, "Mystyle generated : %lX\n", BG.trgPixmap ); */
	}
#endif

    XFreeGC(Xdisplay, gc); /* don't need anymore */
    if( BG.trgPixmap != None )
    {
		BG.finWidth = fin_width ;
		BG.finHeight = fin_height;
		XSync(Xdisplay, 0);
#ifdef DEBUG_BACKGROUND_PMAP	   
		fprintf(stderr, "Setting background to %lX. srcPixmap = %lX\n", BG.trgPixmap, BG.srcPixmap );
#endif
        XSetWindowBackgroundPixmap(Xdisplay, TermWin.vt,
    	                           BG.trgPixmap);
    	XSync(Xdisplay, 0);	
		TermWin.LastPixmapUsed = BG.srcPixmap ;
		TermWin.LastPixmap_root_x = TermWin.root_x ; 
		TermWin.LastPixmap_root_y = TermWin.root_y ; 
		TermWin.LastPixmap_width  = TermWin.root_width ; 
		TermWin.LastPixmap_height = TermWin.root_height ; 
		BG.trgPixmapSet = 1 ;


		if( BG.trgPixmap != BG.srcPixmap )
		{/* don't need it anymore server has it */
	    	XFreePixmap( Xdisplay, BG.trgPixmap );
	    	XSync(Xdisplay, 0);
		}
		BG.trgPixmap = None ;
    }
    /* restore old handler so we can crash again ;) */
    XSetErrorHandler (oldXErrorHandler);

} /******************************* RenderPixmap **********************/
