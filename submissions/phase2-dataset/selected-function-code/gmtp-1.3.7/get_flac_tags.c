void get_flac_tags(gchar *filename, LIBMTP_track_t *trackinformation) {
    FLAC__StreamMetadata *tags = NULL;
    FLAC__StreamMetadata streaminfo;
    gchar * tracknumber = 0;

    // Load in our tag information stream
    if (!FLAC__metadata_get_tags(filename, &tags))
        return;
    if (!FLAC__metadata_get_streaminfo(filename, &streaminfo)) {
        return;
    }
    // We have our tag data, get the individual fields.
    trackinformation->album = g_strdup(FLAC_getFieldText(tags, "ALBUM"));
    trackinformation->title = g_strdup(FLAC_getFieldText(tags, "TITLE"));
    trackinformation->artist = g_strdup(FLAC_getFieldText(tags, "ARTIST"));
    trackinformation->date = g_strdup(FLAC_getFieldText(tags, "DATE"));
    trackinformation->genre = g_strdup(FLAC_getFieldText(tags, "GENRE"));

    tracknumber = FLAC_getFieldText(tags, "TRACKNUMBER");
    if (tracknumber != 0) {
        trackinformation->tracknumber = atoi(tracknumber);
    } else {
        trackinformation->tracknumber = 0;
    }

    // Duration, bitrate and other information
    if((streaminfo.data.stream_info.sample_rate != 0)&&(streaminfo.data.stream_info.total_samples != 0)){
    trackinformation->duration = (streaminfo.data.stream_info.total_samples /
        streaminfo.data.stream_info.sample_rate) * 1000;
    trackinformation->bitrate = 8.0 * (float) (trackinformation->filesize) /
        (1000.0 * (float) streaminfo.data.stream_info.total_samples
        / (float) streaminfo.data.stream_info.sample_rate);
    } else {
        trackinformation->duration = 0;
        trackinformation->bitrate = 0;
    }
    trackinformation->bitratetype = 0; // Not used
    trackinformation->nochannels = streaminfo.data.stream_info.channels;

    //trackinformation->tracknumber = atoi(FLAC_getFieldText(tags, "TRACKNUMBER"));
    FLAC__metadata_object_delete(tags);
    //FLAC__metadata_object_delete(&streaminfo);
    return;
}
