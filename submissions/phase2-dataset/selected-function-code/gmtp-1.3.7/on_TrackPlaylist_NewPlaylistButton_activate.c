void on_TrackPlaylist_NewPlaylistButton_activate(GtkWidget *button, gpointer user_data) {
    gchar *playlistname = NULL;
    gint combobox_entries = 0;

    playlistname = displayPlaylistNewDialog();
    if (playlistname != NULL) {
        // Add in playlist to MTP device.
        playlistAdd(playlistname);
        // Refresh our playlist information.
        devicePlayLists = getPlaylists();

        // Add it to our combobox
#if HAVE_GTK3 == 0
        gtk_combo_box_append_text(GTK_COMBO_BOX(combobox_AddTrackPlaylist), g_strdup(playlistname));
#else
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_AddTrackPlaylist), g_strdup(playlistname));
#endif
        g_free(playlistname);

        // Set the active combobox item.
        combobox_entries = gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(combobox_AddTrackPlaylist)), NULL);
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_AddTrackPlaylist), combobox_entries - 1);
    }
}
