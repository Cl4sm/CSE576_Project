void get_id3_tags(gchar *filename, LIBMTP_track_t *trackinformation) {
    gchar * tracknumber = NULL;
    gchar * trackduration = NULL;
    MP3_Info mp3_information;

    struct id3_file * id3_file_id = id3_file_open(filename, ID3_FILE_MODE_READONLY);

    if (id3_file_id != NULL) {
        // We have a valid file, so lets get some data.
        struct id3_tag* id3_tag_id = id3_file_tag(id3_file_id);
        // We have our tag data, so now cycle through the fields.
        trackinformation->album = ID3_getFrameText(id3_tag_id, ID3_FRAME_ALBUM);
        trackinformation->title = ID3_getFrameText(id3_tag_id, ID3_FRAME_TITLE);
        trackinformation->artist = ID3_getFrameText(id3_tag_id, ID3_FRAME_ARTIST);
        trackinformation->date = ID3_getFrameText(id3_tag_id, ID3_FRAME_YEAR);
        trackinformation->genre = ID3_getFrameText(id3_tag_id, ID3_FRAME_GENRE);

        tracknumber = ID3_getFrameText(id3_tag_id, ID3_FRAME_TRACK);
        if (tracknumber != 0) {
            trackinformation->tracknumber = atoi(tracknumber);
        } else {
            trackinformation->tracknumber = 0;
        }

        // Need below if the default artist field is NULL
        if (trackinformation->artist == NULL)
            trackinformation->artist = ID3_getFrameText(id3_tag_id, "TPE2");
        if (trackinformation->artist == NULL)
            trackinformation->artist = ID3_getFrameText(id3_tag_id, "TPE3");
        if (trackinformation->artist == NULL)
            trackinformation->artist = ID3_getFrameText(id3_tag_id, "TPE4");
        if (trackinformation->artist == NULL)
            trackinformation->artist = ID3_getFrameText(id3_tag_id, "TCOM");
        // Need this if using different Year field.
        if (trackinformation->date == NULL)
            trackinformation->date = ID3_getFrameText(id3_tag_id, "TDRC");

        // Get our track duration via ID3 Tag.
        trackduration = ID3_getFrameText(id3_tag_id, "TLEN");
        if (trackduration != 0) {
            trackinformation->duration = atoi(trackduration);
        } else {
            trackinformation->duration = 0;
        }
        // Close our file for reading the fields.
        id3_file_close(id3_file_id);
    }

    // Duration, bitrate and other information
    // This information must be derived by manually decoding the MP3 file.
    get_mp3_info(filename, &mp3_information);
    trackinformation->duration = mp3_information.duration;
    trackinformation->bitrate = mp3_information.bitrate;
    trackinformation->bitratetype = mp3_information.VBR;
    trackinformation->nochannels = mp3_information.channels;
}
