void displayPlaylistDialog(void) {
    //LIBMTP_playlist_t* tmpplaylist;
    LIBMTP_track_t* tmptrack;
    GtkTreeIter rowIter;
    gchar * tmp_string;

    if (windowPlaylistDialog != NULL) {
        gtk_widget_hide(windowPlaylistDialog);
        gtk_widget_destroy(windowPlaylistDialog);
    }
    windowPlaylistDialog = create_windowPlaylist();
    playlist_number = 0;
    // Clear the track and playlist lists;
    gtk_list_store_clear(GTK_LIST_STORE(playlist_PL_List));
    gtk_list_store_clear(GTK_LIST_STORE(playlist_TrackList));
    // Populate the playlist changebox.
    devicePlayLists = getPlaylists();
    deviceTracks = getTracks();
    setPlayListComboBox();

    // Populate the available track list.
    if (deviceTracks != NULL) {
        // Populate the track list;
        tmptrack = deviceTracks;
        while (tmptrack != NULL) {
            if ((tmptrack->storage_id == DeviceMgr.devicestorage->id) && (LIBMTP_FILETYPE_IS_AUDIO(tmptrack->filetype))) {
                gtk_list_store_append(GTK_LIST_STORE(playlist_TrackList), &rowIter);
                tmp_string = g_strdup_printf("%d:%.2d", (int) ((tmptrack->duration / 1000) / 60), (int) ((tmptrack->duration / 1000) % 60));
                gtk_list_store_set(GTK_LIST_STORE(playlist_TrackList), &rowIter, COL_ARTIST, tmptrack->artist, COL_ALBUM, tmptrack->album,
                        COL_TRACKID, tmptrack->item_id, COL_TRACKNAME, tmptrack->title, COL_TRACKDURATION, tmp_string, -1);
                g_free(tmp_string);
                tmp_string = NULL;
            }
            tmptrack = tmptrack->next;
        }
    }
    gtk_widget_show(GTK_WIDGET(windowPlaylistDialog));
    // Save the current selected playlist if needed.
}
