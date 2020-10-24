SetSrcPixmap(Pixmap p)
{
#ifdef DEBUG_BACKGROUND_PMAP	   
	fprintf(stderr, "srcPixmap = %lx, new srcPixmap = %lx.\n", BG.srcPixmap, p );
#endif
    
	if( BG.srcPixmap != None && BG.bMySource && BG.srcPixmap != p )
    {
		XFreePixmap( Xdisplay, BG.srcPixmap );
		BG.srcPixmap = p ;
		BG.bMySource = 0 ;
    }

    BG.srcPixmap = p ;

    BG.Width = 0 ;
    BG.Height = 0 ;

    if( BG.srcPixmap != None )
    {
		Window root;
      	unsigned int dum, w, h;
      	int dummy;
#ifdef DEBUG_BACKGROUND_PMAP	   
		fprintf(stderr, "Querying geometry of the source pixmap %lX...", BG.srcPixmap );
#endif
		if (XGetGeometry (Xdisplay, BG.srcPixmap, &root, &dummy, &dummy, &w, &h, &dum, &dum))
		{
	    	BG.Width = w ;
	    	BG.Height = h ;
#ifdef DEBUG_BACKGROUND_PMAP	   
			fprintf(stderr, "success : geometry is %dx%d.\n", w, h );
#endif
		}else
		{
#ifdef DEBUG_BACKGROUND_PMAP	   
			fprintf(stderr, "failed.\n" );
#endif
		    BG.srcPixmap = None ;
		}	 
    }
}
