    LIBMTP_playlist_t* tmpplaylist = playlist;
    uint32_t *tmp = NULL;

    uint32_t numtracks = tmpplaylist->no_tracks;
    int32_t count;
    int32_t count2;
    // item_id = our track num... so remove to tmpplaylist->tracks
    if ((instances == MTP_PLAYLIST_ALL_INSTANCES) || (instances == MTP_PLAYLIST_FIRST_INSTANCE)) {
        for (count = 0; count < (int32_t) numtracks; count++) {
            if (tmpplaylist->tracks[count] == track->item_id) {
                // move all ones up one.
                for (count2 = count; count2 < (int32_t) numtracks; count2++) {
                    if ((count2 + 1) != (int32_t) numtracks) {
                        tmpplaylist->tracks[count2] = tmpplaylist->tracks[count2 + 1];
                    }
                }
                // exit the for loop if only doing the first instance.
                if (instances == MTP_PLAYLIST_FIRST_INSTANCE) {
                    count = numtracks;
                }
                tmpplaylist->no_tracks--;
            }
        }
    } else {
        for (count = numtracks - 1; count >= 0; count--) {
            if (tmpplaylist->tracks[count] == track->item_id) {
                // move all ones up one.
                for (count2 = count; count2 < (int32_t) numtracks; count2++) {
                    if ((count2 + 1) != (int32_t) numtracks) {
                        tmpplaylist->tracks[count2] = tmpplaylist->tracks[count2 + 1];
                    }
                }
                // exit the for loop if only doing the first instance.
                if (instances == MTP_PLAYLIST_LAST_INSTANCE) {
                    count = numtracks;
                }
                tmpplaylist->no_tracks--;
            }
        }
    }
    // And redo the memory allocation.
    if ((tmp = g_realloc(tmpplaylist->tracks, sizeof (uint32_t) * (tmpplaylist->no_tracks))) == NULL) {
        g_fprintf(stderr, _("realloc in savePlayList failed\n"));
        displayError(_("Updating playlist failed? 'realloc in savePlayList'\n"));
        return;
    }
    tmpplaylist->tracks = tmp;
    playlistUpdate(tmpplaylist);
}
