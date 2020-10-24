    // This function will populate the playlist_PL_List widget with the
    // details of the selected playlist.
    LIBMTP_playlist_t* tmpplaylist = devicePlayLists;
    gint tmpplaylistID = PlayListID;
    guint trackID = 0;
    GtkTreeIter rowIter;
    gchar * tmp_string = NULL;

    playlist_track_count = 0;

    gtk_list_store_clear(GTK_LIST_STORE(playlist_PL_List));

    if (PlayListID > 0) {
        while (tmpplaylistID--)
            if (tmpplaylist->next != NULL)
                tmpplaylist = tmpplaylist->next;
    }
    // tmpplaylist points to our playlist;
    for (trackID = 0; trackID < tmpplaylist->no_tracks; trackID++) {
        LIBMTP_track_t *trackinfo;
        trackinfo = LIBMTP_Get_Trackmetadata(DeviceMgr.device, tmpplaylist->tracks[trackID]);
        if (trackinfo != NULL) {
            playlist_track_count++;
            gtk_list_store_append(GTK_LIST_STORE(playlist_PL_List), &rowIter);
            tmp_string = g_strdup_printf("%d:%.2d", (int) ((trackinfo->duration / 1000) / 60), (int) ((trackinfo->duration / 1000) % 60));
            gtk_list_store_set(GTK_LIST_STORE(playlist_PL_List), &rowIter, COL_PL_ORDER_NUM, playlist_track_count,
                    COL_PL_ARTIST, trackinfo->artist,
                    COL_PL_ALBUM, trackinfo->album, COL_PL_TRACKID, trackinfo->item_id,
                    COL_PL_TRACKNAME, trackinfo->title, COL_PL_TRACKDURATION, tmp_string, -1);
            g_free(tmp_string);
            tmp_string = NULL;

            LIBMTP_destroy_track_t(trackinfo);
        } else {
            LIBMTP_Dump_Errorstack(DeviceMgr.device);
            LIBMTP_Clear_Errorstack(DeviceMgr.device);
        }
    }
}
