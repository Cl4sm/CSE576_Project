    LIBMTP_playlist_t* tmpplaylist = NULL;
    
    // We need to remove all entries in the combo box before starting.
    // This is a little bit of a hack - but does work.
    runPlaylistHandler = FALSE; // disable the handler from running on the control.
    // For some reason the "changed" event is triggered on a remove_all operation.
#if HAVE_GTK3 == 0
    //gtk_list_store_clear(GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(comboboxentry_playlist))));
    while(comboboxentry_playlist_entries-- > 0){
        gtk_combo_box_remove_text(GTK_COMBO_BOX(comboboxentry_playlist), 0);
    }
#else
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(comboboxentry_playlist));
#endif
    comboboxentry_playlist_entries = 0;
    
    if (devicePlayLists != NULL) {
        // Populate the playlist dropdown box;
        //comboboxentry_playlist;
        tmpplaylist = devicePlayLists;
        while (tmpplaylist != NULL) {
            if (tmpplaylist->storage_id == DeviceMgr.devicestorage->id) {
#if HAVE_GTK3 == 0
                gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxentry_playlist), g_strdup(tmpplaylist->name));
#else
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboboxentry_playlist), g_strdup(tmpplaylist->name));
#endif
                comboboxentry_playlist_entries++;
            }
            tmpplaylist = tmpplaylist->next;
        }
    }
    if (devicePlayLists != NULL) {
        // Set our playlist to the first one.
        gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxentry_playlist), 0);
        playlist_number = 0;
        // Now populate the playlist screen with it's details.
        setPlaylistField(0);
    } else {
        playlist_number = -1;
    }
    // If no playlists set parts of dialog to disabled.
    if (devicePlayLists == NULL) {
        SetPlaylistButtonState(FALSE);
    } else {
        SetPlaylistButtonState(TRUE);
    }
    runPlaylistHandler = TRUE;
}
