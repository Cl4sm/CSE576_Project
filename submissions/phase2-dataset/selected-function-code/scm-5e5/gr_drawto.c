SCM     x, y;
{
int     xi, yi;

        ASRTER( NUMBERP(x), x, ARG1, s_gr_drawto );
        ASRTER( NUMBERP(y), y, ARG2, s_gr_drawto );
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
#ifdef TESTING
        fprintf( stderr, "draw-to called (%d,%d)\n", xi, yi );
#endif
        if( !valid_XYC( xi, yi, 0 ) )
                return  BOOL_F;
#ifdef  X11
        XDrawLine( gr_display, gr_win, gr_gc,
                   (int)gr_x, (int)gr_y, xi, yi );
        gr_events(0);
#else   /* PC version */
        line( (int)gr_x, (int)gr_y, xi, yi );
#endif
        return  BOOL_T;
} /* gr_drawto() */
