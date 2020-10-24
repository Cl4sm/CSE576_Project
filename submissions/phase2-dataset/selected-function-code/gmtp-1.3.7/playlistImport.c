    FILE* fd;
    gchar* playlistname = NULL;
    gchar* fileString = NULL;
    gchar* needle = NULL;
    uint32_t *tracktmp = NULL;
    uint32_t fileobject = 0;
    gboolean ignorepath = Preferences.ignore_path_in_playlist_import;

    // Build basic playlist object
    LIBMTP_playlist_t *playlist = LIBMTP_new_playlist_t();
    playlist->name = NULL;
    playlist->no_tracks = 0;
    playlist->tracks = NULL;
    playlist->parent_id = DeviceMgr.device->default_playlist_folder;
    playlist->storage_id = DeviceMgr.devicestorage->id;

    // Load the file, and parse.
    fd = fopen(filename, "r");
    if (fd == NULL) {
        g_fprintf(stderr, _("Couldn't open playlist file %s\n"), filename);
        displayError(_("Couldn't open playlist file\n"));
        LIBMTP_destroy_playlist_t(playlist);
        return NULL;
    } else {
        fileString = g_malloc0(GMTP_MAX_STRING);
        // Read file until EOF
        while (fgets(fileString, GMTP_MAX_STRING, fd) != NULL) {
            // Strip any trailing '\n' from the string...
            fileString = g_strchomp(fileString);
            if (g_ascii_strncasecmp(fileString, "#GMTPPLA: ", 10) == 0) {
                // We have a playlist name marker...
                playlistname = g_strdup((fileString + 10));
                playlist->name = g_strdup(playlistname);
            } else {
                // We should have a file?
                // But ignore ANY line starting with a # as this is a comment line.
                if ((*fileString != '#') && (*fileString != '\0')) {
                    fileobject = getFileID(fileString, ignorepath);
                    if (fileobject != 0) {
                        // We have a file within our device!
                        playlist->no_tracks++;
                        if ((tracktmp = g_realloc(playlist->tracks, sizeof (uint32_t) * (playlist->no_tracks))) == NULL) {
                            g_fprintf(stderr, _("realloc in playlistImport failed\n"));
                            displayError(_("Updating playlist failed? 'realloc in playlistImport'\n"));
                            return NULL;
                        }
                        playlist->tracks = tracktmp;
                        playlist->tracks[(playlist->no_tracks - 1)] = fileobject;
                    }
                }
            }
        }
        g_free(fileString);
        fclose(fd);
    }
    if ((playlistname == NULL) && (playlist->no_tracks > 0)) {
        // We have some tracks, but no playlist name.
        // So derive the playlist name from the filename...
        playlistname = g_path_get_basename(filename);
        // Now chop off the file extension?
        needle = g_strrstr(playlistname, ".");
        if (needle != NULL) {
            *needle = '\0';
        }
        // And set the name...
        playlist->name = g_strdup(playlistname);
    }

    // If we have something?
    if ((playlistname != NULL) && (playlist->no_tracks > 0)) {
        // Store the playlist on the device...
        gint ret = LIBMTP_Create_New_Playlist(DeviceMgr.device, playlist);
        if (ret != 0) {
            displayError(_("Couldn't create playlist object\n"));
            LIBMTP_Dump_Errorstack(DeviceMgr.device);
            LIBMTP_Clear_Errorstack(DeviceMgr.device);
            g_free(playlistname);
            playlistname = NULL;
        } else {
            displayInformation(_("Playlist imported.\n"));
        }
    } else {
        // Let the user know we found zero tracks, so didn't bother to import it.
        displayInformation(_("Found no tracks within the playlist that exist on this device. Did not import the playlist.\n"));
        g_fprintf(stderr, _("Found no tracks within the playlist that exist on this device. Did not import the playlist.\n"));
        // Clean up the playlist name, since we don't need it.
        if (playlistname != NULL) {
            g_free(playlistname);
            playlistname = NULL;
        }
    }

    // Clean up our playlist data structure.
    LIBMTP_destroy_playlist_t(playlist);

    // Return to caller.
    return playlistname;
}
