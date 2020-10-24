int lbox_handle_motion( LBox *lbox, int cx, int cy, void **item )
{
    int i;
    if ( !lbox->group->frame->img->bkgnd->hide ) {
        if ( !group_handle_motion( lbox->group, cx, cy ) ) {
            /* check if above cell */
            if (lbox->items == NULL)
                return 0;
            for ( i = 0; i < lbox->cell_count; i++ )
                if ( FOCUS( cx, cy, 
                            lbox->group->frame->img->bkgnd->surf_rect.x + lbox->cell_x,
                            lbox->group->frame->img->bkgnd->surf_rect.y + lbox->cell_y + i * ( lbox->cell_h + lbox->cell_gap ),
                            lbox->cell_w, lbox->cell_y ) ) {
                    *item = list_get( lbox->items, lbox->cell_offset + i );
                    return 1;
                }
        }
        else
            return 1;
    }
    return 0;
}