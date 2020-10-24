    // foldername is the name of the folder as the absolute path with leading /
    // We save the currentFolderID, create a new folder on the device,
    // and set currentFolderID to the new folders ID.
    // Then scan the folder (on the filesystem) adding in files as needed.
    // Found folders are always added first, so files are copied from
    // the deepest level of the folder hierarchy first as well, and we
    // work our way back down towards to the initial folder that was
    // dragged in.
    // Lastly we restore the currentFolderID back to what it was.
    GDir *fileImageDir;
    GSList* filelist;
    const gchar *filename;
    gchar* relative_foldername;
    gchar *tmpstring;
    uint32_t oldFolderID;

    filelist = NULL;
    // Save our current working folder.
    oldFolderID = currentFolderID;
    // Get just the folder name, as we are given a full absolute path.
    relative_foldername = basename(foldername);
    if (relative_foldername != NULL) {

        // Determine if the folder already exists in the current location, otherwise create it.

        if(folderExists(relative_foldername, currentFolderID)){
            currentFolderID = getFolder(relative_foldername, currentFolderID);        
        } else {
            // Add our folder to the mtp device and set our new current working folder ID.
            currentFolderID = folderAdd(relative_foldername);
        }    
    }

    // Start scanning the folder on the filesystem for our new files/folders.
    fileImageDir = g_dir_open(foldername, 0, NULL);
    // Now parse that directory looking for JPEG/PNG files (based on settings).
    // If we find one, we create a new GString and add it to the list.
    if (fileImageDir != NULL) {
        filename = g_dir_read_name(fileImageDir);
        while (filename != NULL) {
            // See if a file or a folder?
            tmpstring = g_strconcat(foldername, "/", filename, NULL);
            if (g_file_test(tmpstring, G_FILE_TEST_IS_REGULAR) == TRUE) {
                // We have a regular file. So add it to the list.
                filelist = g_slist_append(filelist, g_strdup(tmpstring));
            } else {
                if (g_file_test(tmpstring, G_FILE_TEST_IS_DIR) == TRUE) {
                    // We have another folder so recursively call ourselves...
                    addFilesinFolder(tmpstring);
                }
            }
            filename = g_dir_read_name(fileImageDir);
            g_free(tmpstring);
        }
    }
    // Set the Playlist ID to be asked if needed.
    if (Preferences.auto_add_track_to_playlist == TRUE) {
        addTrackPlaylistID = GMTP_REQUIRE_PLAYLIST;
    } else {
        addTrackPlaylistID = GMTP_NO_PLAYLIST;
    }
    AlbumErrorIgnore = FALSE;
    // Upload our given files in the current selected folder.
    if (filelist != NULL) {
        g_slist_foreach(filelist, (GFunc) __filesAdd, NULL);
    }
    // Now clear the GList;
    g_slist_foreach(filelist, (GFunc) g_free, NULL);
    g_slist_free(filelist);

    if (fileImageDir != NULL)
        g_dir_close(fileImageDir);
    // Restore our current working folder.
    currentFolderID = oldFolderID;
} // end addFilesinFolder()
