SCM     S;
{
float   xf, yf;
float   sf;
int     ok;

        ASRTER( NUMBERP(S), S, ARG1, s_gr_move );
        if( !gr_grmode_on )
                return  BOOL_F;
        if( INUMP(S) )
                sf = (float)(INUM(S));
        else
                sf = REALPART(S);
#ifdef TESTING
        fprintf( stderr, "move called (%f)\n", sf );
#endif
        ok = 1;
        xf = gr_x + ( COS( gr_dir ) * sf );
        yf = gr_y + ( SIN( gr_dir ) * sf );

        if( (int)xf > gr_max_x ) {
                xf = (float)gr_max_x;
                ok = 0;
        }
        else if( xf < 0.0 ) {
                xf = 0.0;
                ok = 0;
        }
        if( (int)yf > gr_max_y ) {
                yf = (float)gr_max_y;
                ok = 0;
        }
        else if( yf < 0.0 ) {
                yf = 0.0;
                ok = 0;
        }
        gr_x = xf;
        gr_y = yf;
        if( ok )
                return  BOOL_T;
        else
                return  BOOL_F;
} /* gr_move() */
