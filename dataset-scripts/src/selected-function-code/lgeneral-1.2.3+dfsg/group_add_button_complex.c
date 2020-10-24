int group_add_button_complex( Group *group, int id, int icon_id, int x, int y, int lock, const char *tooltip )
{
    if ( group->button_count == group->button_limit ) {
        fprintf( stderr, tr("This group has reached it's maximum number of buttons.\n") );
        return 0;
    }
    group->buttons[group->button_count].surf_rect.x = x + group->frame->img->bkgnd->surf_rect.x;
    group->buttons[group->button_count].surf_rect.y = y + group->frame->img->bkgnd->surf_rect.y;
    group->buttons[group->button_count].surf_rect.w = group->w;
    group->buttons[group->button_count].surf_rect.h = group->h;
    group->buttons[group->button_count].button_rect.x = 0;
    group->buttons[group->button_count].button_rect.y = icon_id * group->h;
    group->buttons[group->button_count].button_rect.w = group->w;
    group->buttons[group->button_count].button_rect.h = group->h;
    group->buttons[group->button_count].id = id;
    group->buttons[group->button_count].active = 1;
    strcpy_lt( group->buttons[group->button_count].tooltip, tooltip, 31 );
    group->buttons[group->button_count].lock = lock;
    group->button_count++;
    return 1;
}