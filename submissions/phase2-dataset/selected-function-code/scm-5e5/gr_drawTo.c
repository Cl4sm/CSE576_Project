SCM     gr_drawTo( x, y )
SCM     x, y;
{
float   xf, yf;

        ASRTER( NUMBERP(x), x, ARG1, s_gr_drawTo );
        ASRTER( NUMBERP(y), y, ARG2, s_gr_drawTo );
        if( !gr_grmode_on )
                return  BOOL_F;
        if( INUMP(x) )
                xf = (float)(INUM(x));
        else
                xf = (REALPART(x));

        if( INUMP(y) )
                yf = (float)(INUM(y));
        else
                yf = (REALPART(y));
#ifdef TESTING
        fprintf( stderr, "draw-to! called (%d,%d)\n", (int)xf, (int)yf );
#endif
        if( !valid_XYC( (int)xf, (int)yf, 0 ) )
                return  BOOL_F;
#ifdef  X11
        XDrawLine( gr_display, gr_win, gr_gc,
                   (int)gr_x, (int)gr_y,
                   (int)xf, (int)yf );
        gr_events(0);
#else   /* PC version */
        line( (int)gr_x, (int)gr_y, (int)xf, (int)yf );
#endif
        gr_x = xf;
        gr_y = yf;
        return  BOOL_T;
} /* gr_drawTo() */
