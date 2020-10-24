SCM     x, y, c;
{
int     xi, yi, ci;

        ASRTER( NUMBERP(x), x, ARG1, s_gr_setdot );
        ASRTER( NUMBERP(y), y, ARG2, s_gr_setdot );
        ASRTER( NUMBERP(c), c, ARG3, s_gr_setdot );
        if( !gr_grmode_on )
                return  BOOL_F;

        if( INUMP(x) )
                xi = (int)(INUM(x));
        else
                xi = (int)(REALPART(x));

        if( INUMP(y) )
                yi = (int)(INUM(y));
        else
                yi = (int)(REALPART(y));

        if( INUMP(c) )
                ci = (int)(INUM(c));
        else
                ci = (int)(REALPART(c));
#ifdef TESTING
        fprintf( stderr, "set-dot! called (%d,%d,%d)\n", xi, yi, ci );
#endif
        if( !valid_XYC( xi, yi, ci ) )
                return  BOOL_F;
#ifdef  X11
        /* Set the drawing color */
        XSetForeground( gr_display, gr_gc, gr_colortbl[ ci ] );
        XDrawPoint( gr_display, gr_win, gr_gc, xi, yi );
        /* Restore the drawing color */
        XSetForeground( gr_display, gr_gc, gr_colortbl[ gr_color ] );
        gr_events(0);
#else   /* PC version */
        putpixel( xi, yi, ci );
#endif
        return  BOOL_T;
} /* gr_setdot() */
