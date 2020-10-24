void str_to_date( Date *date, char *str )
{
    int i;
    char aux_str[12];
    memset( aux_str, 0, sizeof( char ) * 12 );
    // day
    for ( i = 0; i < strlen( str ); i++ )
        if ( str[i] == '.' ) {
            strncpy( aux_str, str, i);
            date->day = atoi( aux_str );
            break;
        }
    str = str + i + 1;
    // month
    for ( i = 0; i < strlen( str ); i++ )
        if ( str[i] == '.' ) {
            strncpy( aux_str, str, i);
            date->month = atoi( aux_str ) - 1;
            break;
        }
    str = str + i + 1;
    // year
    date->year = atoi( str );
}