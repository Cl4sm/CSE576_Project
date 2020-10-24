void lbox_apply( LBox *lbox )
{
    int i;
    void *item;
    int sx = lbox->cell_x, sy = lbox->cell_y;
    SDL_Surface *contents = lbox->group->frame->contents;
    SDL_FillRect( contents, 0, 0x0 );
    /* items */
    SDL_FillRect( contents, 0, 0x0 );
    if ( lbox->items ) {
        for ( i = 0; i < lbox->cell_count; i++ ) {
            item = list_get( lbox->items, i + lbox->cell_offset );
            if ( item ) {
                if ( item == lbox->cur_item ) {
                    DEST( contents, sx, sy, lbox->cell_w, lbox->cell_h );
                    fill_surf( lbox->cell_color );
                }
                (lbox->render_cb)( item, lbox->cell_buffer );
                DEST( contents, sx, sy, lbox->cell_w, lbox->cell_h );
                SOURCE( lbox->cell_buffer, 0, 0 );
                blit_surf();
                sy += lbox->cell_h + lbox->cell_gap;
            }
        }
    }
    frame_apply( lbox->group->frame );
}