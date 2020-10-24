void on_filesAdd_activate(GtkMenuItem *menuitem, gpointer user_data) {
    GSList* files;
    int64_t targetFol = 0;
    //uint32_t tmpFolderID = 0;
    // Set the Playlist ID to be asked if needed.
    if (Preferences.auto_add_track_to_playlist == TRUE) {
        addTrackPlaylistID = GMTP_REQUIRE_PLAYLIST;
    } else {
        addTrackPlaylistID = GMTP_NO_PLAYLIST;
    }
    // Get the files, and add them.
    files = getFileGetList2Add();

    // See if a folder is selected in the folder view, and if so add the files to that folder.
    if ((targetFol = folderListGetSelection()) != -1) {
        //tmpFolderID = currentFolderID;
        currentFolderID = (uint32_t) targetFol;
    }
    AlbumErrorIgnore = FALSE;
    if (files != NULL){
	displayProgressBar(_("File Upload"));
        g_slist_foreach(files, (GFunc) __filesAdd, NULL);
	destroyProgressBar();
    }

    // Now clear the GList;
    g_slist_foreach(files, (GFunc) g_free, NULL);
    g_slist_free(files);

    // Restore the current folder ID is we added to another folder.
    if (targetFol != -1) {
        // Disable this, so the user is taken to the folder in which the files were added to.
        //currentFolderID = tmpFolderID;
    }
    // Now do a device rescan to see the new files.
    deviceRescan();
    deviceoverwriteop = MTP_ASK;
} // end on_filesAdd_activate()
