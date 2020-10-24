        GtkSelectionData * selection_data, guint info, guint time, gpointer user_data) {

    //uint32_t mainFolderID = 0;
    int64_t targetFolderID = 0;

    g_signal_stop_emission_by_name((gpointer) treeviewFolders, "drag-data-received");

#if HAVE_GTK3 == 0
    if (selection_data->data)
#else
    if (gtk_selection_data_get_data(selection_data))
#endif
    {

        //mainFolderID = currentFolderID;

        // Get our target folder ID...
        targetFolderID = folderListGetSelection();
        if (targetFolderID != -1) {
            currentFolderID = targetFolderID;

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
                //displayProgressBar(_("File Upload"));
                g_slist_foreach(files, (GFunc) __filesAdd, NULL);
                //destroyProgressBar();
            }
            destroyProgressBar();
            // Now clear the GList;
            g_slist_foreach(files, (GFunc) g_free, NULL);
            g_slist_free(files);
            // Now do a device rescan to see the new files.

            // Restore our current Folder ID.
            //currentFolderID = mainFolderID;
        }

        deviceRescan();
        deviceoverwriteop = MTP_ASK;
    }
}
