SCM     gr_validXYC( x, y, c )
SCM     x, y, c;
{
int     xi, yi, ci;

        ASRTER( NUMBERP(x), x, ARG1, s_gr_validXYC );
        ASRTER( NUMBERP(y), y, ARG2, s_gr_validXYC );
        ASRTER( NUMBERP(c), c, ARG3, s_gr_validXYC );
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

/* valid_XYC() calls gr_events() */

        if( valid_XYC( xi, yi, ci ) )
                return  BOOL_T;
        else
                return  BOOL_F;
} /* gr_validXYC() */
