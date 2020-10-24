Group *group_create( SDL_Surface *frame, int alpha, SDL_Surface *img, int w, int h, int limit, int base_id, 
                     Label *label, SDL_Surface *surf, int x, int y )
{
    Group *group = calloc( 1, sizeof( Group ) );
    if ( ( group->frame = frame_create( frame, alpha, surf, x, y ) ) == 0 ) goto failure;
    if ( img == 0 ) {
        fprintf( stderr, "group_create: passed button surface is NULL: %s\n", SDL_GetError() );
        goto failure;
    }
    SDL_SetColorKey( img, SDL_SRCCOLORKEY, 0x0 );
    group->img = img;
    group->w = w; group->h = h;
    group->base_id = base_id;
    group->button_limit = limit;
    if ( ( group->buttons = calloc( group->button_limit, sizeof( Button ) ) ) == 0 ) {
        fprintf( stderr, tr("Out of memory\n") );
        goto failure;
    }
    group->label = label;
    return group;
failure:
    group_delete( &group );
    return 0;
}