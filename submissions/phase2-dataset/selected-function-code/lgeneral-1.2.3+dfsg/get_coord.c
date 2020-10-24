void get_coord( const char *str, int *x, int *y )
{
    int i;
    char *cur_arg;

    *x = *y = 0;

    /* get position of comma */
    for ( i = 0; i < strlen( str ); i++ )
        if ( str[i] == ',' ) break;
    if ( i == strlen( str ) ) {
        fprintf( stderr, tr("get_coord: no comma found in pair of coordinates '%s'\n"), str );
        return; /* no comma found */
    }

    /* y */
    cur_arg = (char *)str + i + 1;
    if ( cur_arg[0] == 0 )
        fprintf( stderr, tr("get_coord: warning: y-coordinate is empty (maybe you left a space between x and comma?)\n") );
    *y = atoi( cur_arg );
    /* x */
    cur_arg = strdup( str ); cur_arg[i] = 0;
    *x = atoi( cur_arg );
    FREE( cur_arg );
}