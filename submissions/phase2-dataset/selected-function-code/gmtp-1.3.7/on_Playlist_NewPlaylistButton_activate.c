void on_Playlist_NewPlaylistButton_activate(GtkMenuItem *menuitem, gpointer user_data) {
    //g_printf("Clicked on new playlist button\n");
    gchar *playlistname = NULL;

    // Save our current selected playlist!
    if (devicePlayLists != NULL)
        playlist_SavePlaylist(playlist_number);

    playlistname = displayPlaylistNewDialog();
    if (playlistname != NULL) {
        // Add in playlist to MTP device.
        playlistAdd(playlistname);
        // Refresh our playlist information.
        devicePlayLists = getPlaylists();
        gtk_list_store_clear(GTK_LIST_STORE(playlist_PL_List));
        // Add it to our combobox

#if HAVE_GTK3 == 0
        gtk_combo_box_append_text(GTK_COMBO_BOX(comboboxentry_playlist), g_strdup(playlistname));
#else
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboboxentry_playlist), g_strdup(playlistname));
#endif
        g_free(playlistname);

        // Set the active combobox item.
        comboboxentry_playlist_entries++;
        playlist_number = comboboxentry_playlist_entries - 1;
        gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxentry_playlist), comboboxentry_playlist_entries - 1);
        SetPlaylistButtonState(TRUE);
        setPlaylistField(playlist_number);
    }
} // end on_Playlist_NewPlaylistButton_activate()
