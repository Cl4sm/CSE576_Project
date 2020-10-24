FDlg *fdlg_create( 
                   SDL_Surface *lbox_frame, int alpha, int border,
                   SDL_Surface *lbox_buttons, int lbox_button_w, int lbox_button_h,
                   int cell_h,
                   SDL_Surface *conf_frame,
                   SDL_Surface *conf_buttons, int conf_button_w, int conf_button_h,
                   int id_ok, 
                   Label *label, 
                   void (*lbox_cb)( void*, SDL_Surface* ),
                   void (*file_cb)( const char*, SDL_Surface* ),
                   SDL_Surface *surf, int x, int y )
{
    int info_w, info_h, button_count;
    int cell_count, cell_w;
    FDlg *dlg = calloc( 1, sizeof( FDlg ) );
    /* listbox */
    cell_w = lbox_frame->w - 2 * border;
    cell_count = ( lbox_frame->h - 2 * border - lbox_button_h ) / ( cell_h + 1 );
    if ( ( dlg->lbox = lbox_create( lbox_frame, alpha, border,
                                    lbox_buttons, lbox_button_w, lbox_button_h, 
                                    label, 
                                    cell_count, 4, cell_w, cell_h, 1, 0x0000ff, 
                                    lbox_cb, surf, x, y ) ) == 0 )
        goto failure;
    /* frame */
    button_count = conf_buttons->h / conf_button_h;
    if ( ( dlg->group = group_create( conf_frame, alpha, conf_buttons, conf_button_w, conf_button_h, button_count, id_ok, label, 
                                      surf, x + lbox_frame->w, y ) ) == 0 )
        goto failure;
    /* buttons */
    dlg->button_y = conf_frame->h - border - conf_button_h;
    dlg->button_x = conf_frame->w;
    dlg->button_dist = 10 + conf_button_w;
    group_add_button( dlg->group, id_ok, dlg->button_x - dlg->button_dist * 2, dlg->button_y, 0, tr("Ok") );
    group_add_button( dlg->group, id_ok + 1, dlg->button_x - dlg->button_dist, dlg->button_y, 0, tr("Cancel") );
    /* file callback */
    dlg->file_cb = file_cb;
    /* info region */
    info_w = conf_frame->w - 2 * border;
    info_h = conf_frame->h - 3 * border - conf_button_h;
    dlg->info_x = border;
    dlg->info_y = border;
    dlg->info_buffer = create_surf( info_w, info_h, SDL_SWSURFACE );
    /* path */
    strcpy( dlg->root, "/" );
    dlg->subdir[0] = 0;
    return dlg;
failure:
    fdlg_delete( &dlg );
    return 0;
}