SCM     gr_moveTo( x, y )
SCM     x, y;
{
float   xf, yf;

        ASRTER( NUMBERP(x), x, ARG1, s_gr_moveTo );
        ASRTER( NUMBERP(y), y, ARG2, s_gr_moveTo );
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
        fprintf( stderr, "move-to! called (%d,%d)\n", (int)xf, (int)yf );
#endif
        if( !valid_XYC( (int)xf, (int)yf, 0 ) )
                return  BOOL_F;
        gr_x = xf;
        gr_y = yf;
        return  BOOL_T;
} /* gr_moveTo() */
