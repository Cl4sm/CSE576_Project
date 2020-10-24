    LIBMTP_album_t *album = NULL;
    LIBMTP_album_t *found_album = NULL;
    LIBMTP_album_t *album_orig = NULL;
    gint ret = 0;

    // Quick sanity check.
    if ((albuminfo->name == NULL) || (albuminfo->artist == NULL))
        return;

    // Lets try to find the album.
    album = LIBMTP_Get_Album_List_For_Storage(DeviceMgr.device, DeviceMgr.devicestorage->id);
    album_orig = album;
    while ((album != NULL) && (found_album == NULL)) {
        if ((album->name != NULL) && (album->artist != NULL)) {
            // Lets test it. We attempt to match both album name and artist.
            if ((g_ascii_strcasecmp(album->name, albuminfo->name) == 0) &&
                    (g_ascii_strcasecmp(album->artist, albuminfo->artist) == 0)) {
                found_album = album;
            }
        }
        album = album->next;
    }
    // Some devices ignore all other fields and only retain the ablum name - so test for this as well!
    album = album_orig;
    if (found_album == NULL) {
        while ((album != NULL) && (found_album == NULL)) {
            if (album->name != NULL) {
                // Lets test it. We attempt to match both album name and artist.
                if (g_ascii_strcasecmp(album->name, albuminfo->name) == 0) {
                    found_album = album;
                }
            }
            album = album->next;
        }
    }

    if (found_album != NULL) {
        // The album already exists.
        uint32_t *tracks;
        tracks = (uint32_t *) g_malloc0((found_album->no_tracks + 1) * sizeof (uint32_t));
        if (!tracks) {
            g_fprintf(stderr, _("ERROR: Failed memory allocation in albumAddTrackToAlbum()\n"));
            return;
        }
        found_album->no_tracks++;
        if (found_album->tracks != NULL) {
            memcpy(tracks, found_album->tracks, found_album->no_tracks * sizeof (uint32_t));
            free(found_album->tracks);
        }
        tracks[found_album->no_tracks - 1] = trackinfo->item_id; // This ID is only set once the track is on the device.
        found_album->tracks = tracks;
        ret = LIBMTP_Update_Album(DeviceMgr.device, found_album);
        g_free(tracks);
        found_album->tracks = NULL;
    } else {
        // New album.
        uint32_t *trackid;
        trackid = (uint32_t *) g_malloc0(sizeof (uint32_t));
        *trackid = trackinfo->item_id;
        albuminfo->tracks = trackid;
        albuminfo->no_tracks = 1;
        albuminfo->storage_id = DeviceMgr.devicestorage->id;
        ret = LIBMTP_Create_New_Album(DeviceMgr.device, albuminfo);
        g_free(trackid);
        albuminfo->tracks = NULL;
    }
    if (ret != 0) {

        if (Preferences.suppress_album_errors == FALSE) {
            if (AlbumErrorIgnore == FALSE) {
                displayError(_("Error creating or updating album.\n(This could be due to that your device does not support albums.)\n"));
                g_fprintf(stderr, _("Error creating or updating album.\n(This could be due to that your device does not support albums.)\n"));
            }
        }
        // Displayed the message once already per transfer...
        AlbumErrorIgnore = TRUE;
        LIBMTP_Dump_Errorstack(DeviceMgr.device);
        LIBMTP_Clear_Errorstack(DeviceMgr.device);
    }
    clearAlbumStruc(album_orig);
}
