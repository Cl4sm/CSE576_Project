        GtkSelectionData *selection_data, guint info, guint time, gpointer user_data) {
#if HAVE_GTK3 == 0
    if (selection_data->data)
#else
    if (gtk_selection_data_get_data(selection_data))
#endif
    {
        GSList* files;
        displayProgressBar(_("File Upload"));
#if HAVE_GTK3 == 0
        files = getFilesListURI((gchar *) selection_data->data);
#else
        files = getFilesListURI((gchar *) gtk_selection_data_get_data(selection_data));
#endif
        // Set the Playlist ID to be asked if needed.
        if (Preferences.auto_add_track_to_playlist == TRUE) {
            addTrackPlaylistID = GMTP_REQUIRE_PLAYLIST;
        } else {
            addTrackPlaylistID = GMTP_NO_PLAYLIST;
        }
        AlbumErrorIgnore = FALSE;
        // Add the files.
        if (files != NULL) {
            g_slist_foreach(files, (GFunc) __filesAdd, NULL);
        }
        destroyProgressBar();
        // Now clear the GList;
        g_slist_foreach(files, (GFunc) g_free, NULL);
        g_slist_free(files);
        // Now do a device rescan to see the new files.
        deviceRescan();
        deviceoverwriteop = MTP_ASK;
    }
} // gmtp_drag_data_received()
