int fdlg_handle_button( FDlg *fdlg, int button_id, int cx, int cy, Button **button )
{
    char path[512];
    void *item = 0;
    char *fname;
    if ( !fdlg->group->frame->img->bkgnd->hide ) {
        if ( group_handle_button( fdlg->group, button_id, cx, cy, button ) )
            return 1;
        if ( lbox_handle_button( fdlg->lbox, button_id, cx, cy, button, &item ) ) {
            if ( item ) {
                SDL_FillRect( fdlg->group->frame->contents, 0, 0x0 );
                fname = (char*)item;
                if ( fname[0] == '*' ) {
                    /* switch directory */
                    if ( fname[1] == '.' ) {
                        /* one up */
                        if ( strrchr( fdlg->subdir, '/' ) )
                            (strrchr( fdlg->subdir, '/' ))[0] = 0;
                        else
                            fdlg->subdir[0] = 0;
                    }
                    else {
                        if ( fdlg->subdir[0] != 0 )
                            strcat( fdlg->subdir, "/" );
                        strcat( fdlg->subdir, fname + 1 );
                    }
                    if ( fdlg->subdir[0] == 0 )
                        strcpy( path, fdlg->root );
                    else
                        sprintf( path, "%s/%s", fdlg->root, fdlg->subdir );
                    lbox_set_items( fdlg->lbox, dir_get_entries( path, fdlg->root, 0 ) );
                    (fdlg->file_cb)( 0, fdlg->info_buffer );
                }
                else {
                    /* file info */
                    if ( fdlg->subdir[0] == 0 )
                        strcpy( path, fname );
                    else
                        sprintf( path, "%s/%s", fdlg->subdir, fname );
                    (fdlg->file_cb)( path, fdlg->info_buffer );
                }
                DEST( fdlg->group->frame->contents, fdlg->info_x, fdlg->info_y, fdlg->info_buffer->w, fdlg->info_buffer->h );
                SOURCE( fdlg->info_buffer, 0, 0 );
                blit_surf();
                frame_apply( fdlg->group->frame );
            }
        }
        return 0;
    }
    return 0;
}