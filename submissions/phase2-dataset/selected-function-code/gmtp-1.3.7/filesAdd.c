void filesAdd(gchar* filename) {
    uint64_t filesize = 0;
    gchar *filename_stripped;
    struct stat sb;
    LIBMTP_file_t *genfile = NULL;
    LIBMTP_track_t *trackfile = NULL;
    LIBMTP_album_t *albuminfo = NULL;
    LIBMTP_playlist_t* tmpplaylist = NULL;
    gint ret;

    // Maybe something went wrong, so we disconnected. If so, then simple exit....
    if (DeviceMgr.deviceConnected == FALSE){
        return;
    }

    if(progressDialog_killed == TRUE){
	    return;
    }

    if (stat(filename, &sb) == -1) {
        perror("stat");
        return;
    }

    filesize = sb.st_size;
    if (filesize > DeviceMgr.devicestorage->FreeSpaceInBytes) {
        g_fprintf(stderr, _("Unable to add %s due to insufficient space: filesize = %llu, freespace = %llu\n"),
                filename, 
                (unsigned long long int)filesize, 
                (unsigned long long int)DeviceMgr.devicestorage->FreeSpaceInBytes);
        displayError(_("Unable to add file due to insufficient space"));
        return;
    }

    filename_stripped = basename(filename);
    //displayProgressBar(_("File Upload"));
    setProgressFilename(g_strdup(filename_stripped));

    // What we need to do is work what type of file we are sending
    // and either use the general file send, or
    // use the track send function.
    ret = find_filetype(filename_stripped);

    gboolean useTrack = (ret == LIBMTP_FILETYPE_MP3) ||
            (ret == LIBMTP_FILETYPE_OGG) ||
            (ret == LIBMTP_FILETYPE_FLAC) ||
            (ret == LIBMTP_FILETYPE_WMA);
    
    if(Preferences.allmediaasfiles){
        useTrack = FALSE;
    }
    
    if (useTrack) {
        // We have an MP3/Ogg/FLAC/WMA file.
        trackfile = LIBMTP_new_track_t();

        trackfile->filesize = filesize;
        trackfile->filename = g_strdup(filename_stripped);
        trackfile->filetype = find_filetype(filename_stripped);
        trackfile->parent_id = currentFolderID;
        trackfile->storage_id = DeviceMgr.devicestorage->id;
        trackfile->album = NULL;
        trackfile->title = NULL;
        trackfile->artist = NULL;
        trackfile->date = NULL;
        trackfile->genre = NULL;
        trackfile->tracknumber = 0;

        albuminfo = LIBMTP_new_album_t();
        albuminfo->parent_id = currentFolderID;
        albuminfo->storage_id = DeviceMgr.devicestorage->id;
        albuminfo->album_id = 0;
        // Let's collect our metadata from the file, typically id3 tag data.
        switch (ret) {
            case LIBMTP_FILETYPE_MP3:
                // We have an MP3 file, so use id3tag to read the metadata.
                get_id3_tags(filename, trackfile);
                break;
            case LIBMTP_FILETYPE_OGG:
                get_ogg_tags(filename, trackfile);
                break;
            case LIBMTP_FILETYPE_FLAC:
                get_flac_tags(filename, trackfile);
                break;
            case LIBMTP_FILETYPE_WMA:
                get_asf_tags(filename, trackfile);
                break;
                //break;

        }
        // Add some data if it's all blank so we don't freak out some players.
        if (trackfile->album == NULL)
            trackfile->album = NULL;
        if (trackfile->title == NULL)
            trackfile->title = g_strdup(filename_stripped);
        if (trackfile->artist == NULL)
            trackfile->artist = g_strdup(_("<Unknown>"));
        if (trackfile->date == NULL) {
            trackfile->date = g_strdup("");
        } else {
            if (strlen(trackfile->date) == 4) {
                // only have year part, so extend it.
                trackfile->date = g_strconcat(trackfile->date, "0101T000000", NULL);
            }
        }
        if (trackfile->genre == NULL)
            trackfile->genre = g_strdup(_("<Unknown>"));

        // Update our album info, if we actually have an album.
        if (trackfile->album != NULL) {
            albuminfo->name = g_strdup(trackfile->album);
            albuminfo->artist = g_strdup(trackfile->artist);
            albuminfo->composer = NULL;
            albuminfo->genre = g_strdup(trackfile->genre);
        }

        // If we need a playlist, then ask for it.
        if (addTrackPlaylistID == GMTP_REQUIRE_PLAYLIST) {
            addTrackPlaylistID = displayAddTrackPlaylistDialog(TRUE);
        }

        // Now send the track
        ret = LIBMTP_Send_Track_From_File(DeviceMgr.device, filename, trackfile, fileprogress, NULL);
        if (ret != 0) {
            // Report the error in sending the file.
            g_fprintf(stderr, _("Error sending track.\n"));
            displayError(g_strdup_printf(_("Error code %d sending track to device: <b>%s</b>"), ret, filename));
            LIBMTP_Dump_Errorstack(DeviceMgr.device);
            LIBMTP_Clear_Errorstack(DeviceMgr.device);
        } else {
            // Adjust device storage.
            DeviceMgr.devicestorage->FreeSpaceInBytes -= filesize;
            DeviceMgr.devicestorage->FreeSpaceInObjects--;

            // Only update Album data if transfer was successful.
            if (trackfile->album != NULL) {
                albumAddTrackToAlbum(albuminfo, trackfile);
            }

            // Now add to playlist if needed...
            if ((addTrackPlaylistID != GMTP_REQUIRE_PLAYLIST) && (addTrackPlaylistID != GMTP_NO_PLAYLIST)) {
                // addTrackPlaylistID has the ID of the playlist, and trackfile is the track we need to
                // add to that playlist.

                // Find the playlist.
                tmpplaylist = devicePlayLists;
                while (tmpplaylist != NULL) {
                    if (tmpplaylist->playlist_id != (uint32_t) addTrackPlaylistID) {
                        // Don't have it.
                        tmpplaylist = tmpplaylist->next;
                    } else {
                        //We have found it.
                        playlistAddTrack(tmpplaylist, trackfile);
                        tmpplaylist = NULL;
                    }
                }
            }
        }
        LIBMTP_destroy_track_t(trackfile);
        LIBMTP_destroy_album_t(albuminfo);
    } else {
        // Generic file upload.
        genfile = LIBMTP_new_file_t();
        genfile->filesize = filesize;
        genfile->filename = g_strdup(filename_stripped);
        genfile->filetype = find_filetype(filename_stripped);
        genfile->parent_id = currentFolderID;
        genfile->storage_id = DeviceMgr.devicestorage->id;

        // Only import the file if it's not a Playlist or Album. (Bad mojo if this happens).
        if ((genfile->filetype != LIBMTP_FILETYPE_ALBUM) && (genfile->filetype != LIBMTP_FILETYPE_PLAYLIST)) {
            ret = LIBMTP_Send_File_From_File(DeviceMgr.device, filename, genfile, fileprogress, NULL);
            if (ret != 0) {
                g_fprintf(stderr, _("Error sending file %s.\n"), filename);
                displayError(g_strconcat(_("Error sending file:"), " <b>", filename, "</b>", NULL));
                LIBMTP_Dump_Errorstack(DeviceMgr.device);
                LIBMTP_Clear_Errorstack(DeviceMgr.device);
            } else {
                // Adjust device storage.
                DeviceMgr.devicestorage->FreeSpaceInBytes -= filesize;
                DeviceMgr.devicestorage->FreeSpaceInObjects--;
            }
        }
        LIBMTP_destroy_file_t(genfile);
    }


    //destroyProgressBar();
    // Now update the storage...
    if (DeviceMgr.devicestorage == NULL) {
        if (LIBMTP_Get_Storage(DeviceMgr.device, 0) < 0) {
            // We have an error getting our storage, so let the user know and then disconnect the device.
            displayError("Failed to get storage parameters from the device - need to disconnect.");
            on_deviceConnect_activate(NULL, NULL);
            return;
        }
        if (DeviceMgr.storagedeviceID == MTP_DEVICE_SINGLE_STORAGE) {
            DeviceMgr.devicestorage = DeviceMgr.device->storage;
        } else {
            DeviceMgr.devicestorage = getCurrentDeviceStoragePtr(DeviceMgr.storagedeviceID);
        }
    }
}
