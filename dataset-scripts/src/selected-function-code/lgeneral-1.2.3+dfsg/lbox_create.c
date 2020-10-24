LBox *lbox_create( SDL_Surface *frame, int alpha, int border, SDL_Surface *buttons, int button_w, int button_h, 
                   Label *label, 
                   int cell_count, int step, int cell_w, int cell_h, int cell_gap, int cell_color,
                   void (*cb)(void*, SDL_Surface*), 
                   SDL_Surface *surf, int x, int y )
{
    int bx, by;
    LBox *lbox = calloc( 1, sizeof( LBox ) );
    /* group */
    if ( ( lbox->group = group_create( frame , alpha, buttons, button_w, button_h, 2, ID_INTERN_UP, label, surf, x, y ) ) == 0 )
        goto failure;
    /* cells */
    lbox->step = step;
    lbox->cell_count = cell_count;
    lbox->cell_w = cell_w;
    lbox->cell_h = cell_h;
    lbox->cell_gap = cell_gap;
    lbox->cell_color = cell_color;
    lbox->cell_x = lbox->cell_y = border;
    lbox->cell_buffer = create_surf( cell_w, cell_h, SDL_SWSURFACE );
    lbox->render_cb = cb;
    /* up/down */
    bx = ( frame->w / 2 - button_w ) / 2;
    by = frame->h - border - button_h;
    group_add_button( lbox->group, ID_INTERN_UP, bx, by, 0, tr("Up") ); 
    group_add_button( lbox->group, ID_INTERN_DOWN, frame->w - bx - button_w, by, 0, tr("Down") ); 
    return lbox;
failure:
    free( lbox );
    return 0;
}