SCM     c;
{
int     color;

        ASRTER( NUMBERP(c), c, ARG1, s_gr_setcolor );
        if( !gr_grmode_on )
                return  BOOL_F;
        if( INUMP(c) )
                color = (int)(INUM(c));
        else
                color = (int)(REALPART(c));
#ifdef TESTING
        fprintf( stderr, "set-color! called (%d)\n", color );
#endif
        if( !valid_XYC( 0, 0, color ) )
                return  BOOL_F;
        gr_color = color;
#ifdef  X11
        /* Set the drawing color */
        XSetForeground( gr_display, gr_gc, gr_colortbl[ gr_color ] );
        gr_events(0);
#else   /* PC version */
        setcolor( gr_color );
#endif
        return  BOOL_T;
} /* gr_setcolor() */
