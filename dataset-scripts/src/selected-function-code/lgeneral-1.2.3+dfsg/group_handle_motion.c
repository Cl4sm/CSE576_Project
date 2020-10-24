int group_handle_motion( Group *group, int x, int y )
{
    int i;
    if ( !group->frame->img->bkgnd->hide )
    if ( x >= group->frame->img->bkgnd->surf_rect.x && y >= group->frame->img->bkgnd->surf_rect.y )
    if ( x < group->frame->img->bkgnd->surf_rect.x + group->frame->img->bkgnd->surf_rect.w )
    if ( y < group->frame->img->bkgnd->surf_rect.y + group->frame->img->bkgnd->surf_rect.h ) {
        label_hide( group->label, 1 );
        for ( i = 0; i < group->button_count; i++ )
            if ( group->buttons[i].active ) {
                if ( button_focus( &group->buttons[i], x, y ) ) {
                    label_write( group->label, 0, group->buttons[i].tooltip );
                    if ( !group->buttons[i].down )
                        group->buttons[i].button_rect.x = group->w;
                }
                else
                    if ( !group->buttons[i].down || !group->buttons[i].lock )
                        group->buttons[i].button_rect.x = 0;
            }
        return 1;
    }
    for ( i = 0; i < group->button_count; i++ )
        if ( group->buttons[i].active && ( !group->buttons[i].down || !group->buttons[i].lock ) )
            group->buttons[i].button_rect.x = 0;
    return 0;
}