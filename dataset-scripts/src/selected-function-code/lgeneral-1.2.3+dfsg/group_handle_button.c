int group_handle_button( Group *group, int button_id, int x, int y, Button **button )
{
    int i;
    if ( button_id == BUTTON_LEFT ) 
    if ( !group->frame->img->bkgnd->hide )
    if ( x >= group->frame->img->bkgnd->surf_rect.x && y >= group->frame->img->bkgnd->surf_rect.y )
    if ( x < group->frame->img->bkgnd->surf_rect.x + group->frame->img->bkgnd->surf_rect.w )
    if ( y < group->frame->img->bkgnd->surf_rect.y + group->frame->img->bkgnd->surf_rect.h ) {
        for ( i = 0; i < group->button_count; i++ )
            if ( group->buttons[i].active )
                if ( button_focus( &group->buttons[i], x, y ) ) {
                    *button = &group->buttons[i];
                    (*button)->down = !(*button)->down;
                    if ( (*button)->down )
                        (*button)->button_rect.x = 2 * group->w;
                    else
                        (*button)->button_rect.x = 0;
                    return 1;
                }
    }
    return 0;
}