int lbox_handle_button( LBox *lbox, int button_id, int cx, int cy, Button **button, void **item )
{
    int i;
	
    *item = 0;

    /* check focus, if not in listbox nothing to do */	
    if ( !FOCUS( cx, cy, lbox->group->frame->img->bkgnd->surf_rect.x,
               lbox->group->frame->img->bkgnd->surf_rect.y,
               group_get_width(lbox->group), group_get_height(lbox->group)) )
        return 0;

    /* see if up/down button was hit */	
    group_handle_button( lbox->group, button_id, cx, cy, button );

    /* handle button/wheel or selection if not hidden */
    if ( !lbox->group->frame->img->bkgnd->hide ) {
    if ( ( *button && (*button)->id == ID_INTERN_UP ) || button_id == WHEEL_UP ) {
        /* scroll up */
        lbox->cell_offset -= lbox->step;
        if ( lbox->cell_offset < 0 )
            lbox->cell_offset = 0;
            lbox_apply( lbox );
        return 1;
    }
    else
        if ( ( *button && (*button)->id == ID_INTERN_DOWN ) || button_id == WHEEL_DOWN ) {
            /* scroll down */
            if ( lbox->items == NULL || lbox->cell_count >= lbox->items->count )
                lbox->cell_offset = 0;
            else {
                lbox->cell_offset += lbox->step;
                if ( lbox->cell_offset + lbox->cell_count >= lbox->items->count )
                    lbox->cell_offset = lbox->items->count - lbox->cell_count;
            }
            lbox_apply( lbox );
            return 1;
        }
        else 
            if ( lbox->items ) 
                for ( i = 0; i < lbox->cell_count; i++ )
                    /* check if above cell */
                    if ( FOCUS( cx, cy, 
                                lbox->group->frame->img->bkgnd->surf_rect.x + lbox->cell_x,
                                lbox->group->frame->img->bkgnd->surf_rect.y + lbox->cell_y + i * ( lbox->cell_h + lbox->cell_gap ),
                                lbox->cell_w, lbox->cell_h ) ) {
                        lbox->cur_item = list_get( lbox->items, lbox->cell_offset + i );
                        lbox_apply( lbox );
                        *item = lbox->cur_item;
                        return 1;
                    }
    }
    return 0;
}