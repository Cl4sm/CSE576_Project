SCM     gr_getdot( x, y )
SCM     x, y;
{
int                     xi, yi;
#ifdef  X11
XImage                  *xim;
XWindowAttributes       wattr;
unsigned long           dot;
int                     i;
#endif
        ASRTER( NUMBERP(x), x, ARG1, s_gr_getdot );
        ASRTER( NUMBERP(y), y, ARG2, s_gr_getdot );
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
        fprintf( stderr, "get-dot called (%d,%d)\n", xi, yi );
#endif
        if( !valid_XYC( xi, yi, 0 ) )
                return  BOOL_F;
#ifdef  X11
        /* Now, this IS ugly. But it's there if you need it.            */

        /* Have to make sure that the window is mapped.  Tough...       */
        XGetWindowAttributes( gr_display, gr_win, &wattr );
        if( wattr.map_state == IsUnmapped ) {
            XMapWindow( gr_display, gr_win );
            gr_typedevent( MapNotify );
        }
        /* I KNOW this sucks.                                           */
        xim = XGetImage( gr_display, gr_win, xi, yi, 1, 1, AllPlanes, XYPixmap );
        dot = XGetPixel( xim, 0, 0 );
        for( i = 0; i < GR_COLORS; ++i ) {
            if( gr_colortbl[i] == dot )
                return MAKINUM( (long)i );
        }
        /* This should never happen. There's garbage in the window!     */
        fprintf( stderr, "%s: %s: Got an illegal pixel value %lu. \
Is there garbage?\n", gr_progname, s_gr_getdot, dot );
        return BOOL_F;
#else   /* PC version */
        return MAKINUM( (long)getpixel( xi, yi ) );
#endif
} /* gr_getdot() */
