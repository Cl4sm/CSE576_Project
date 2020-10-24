int get_close_hex_pos( int x, int y, int id, int *dest_x, int *dest_y )
{
    if ( id == 0 ) {
        *dest_x = x;
        *dest_y = y - 1;
    }
    else
    if ( id == 1 ) {
        if ( x & 1 ) {
            *dest_x = x + 1;
            *dest_y = y;
        }
        else {
            *dest_x = x + 1;
            *dest_y = y - 1;
        }
    }
    else
    if ( id == 2 ) {
        if ( x & 1 ) {
            *dest_x = x + 1;
            *dest_y = y + 1;
        }
        else {
            *dest_x = x + 1;
            *dest_y = y;
        }
    }
    else
    if ( id == 3 ) {
        *dest_x = x;
        *dest_y = y + 1;
    }
    else
    if ( id == 4 ) {
        if ( x & 1 ) {
            *dest_x = x - 1;
            *dest_y = y + 1;
        }
        else {
            *dest_x = x - 1;
            *dest_y = y;
        }
    }
    else
    if ( id == 5 ) {
        if ( x & 1 ) {
            *dest_x = x - 1;
            *dest_y = y;
        }
        else {
            *dest_x = x - 1;
            *dest_y = y - 1;
        }
    }
	if ( *dest_x <= 0 || *dest_y <= 0 || *dest_x >= map_w-1 || 
							*dest_y >= map_h-1 )
		return 0;
    return 1;
}