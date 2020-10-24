    OggVorbis_File *mov_file = NULL;
    vorbis_info * mov_info = NULL;
    FILE *mfile;
    vorbis_comment *mov_file_comment = NULL;
    gchar * tracknumber = NULL;

    // Attempt to open the file, and init the OggVorbis_File struct for our file.
    // Yes I know about ov_fopen(), but Solaris 10 ships with vorbis 1.0.1 which
    // doesn't have this function.
    mfile = fopen(filename, "r");
    if (mfile == NULL)
        return;

    // Allocate memory to hold the OV file information.
    mov_file = g_malloc0(sizeof (OggVorbis_File));

    if (ov_open(mfile, mov_file, NULL, 0) != 0) {
        fclose(mfile);
        return;
    }

    // Get or comment data;
    mov_file_comment = ov_comment(mov_file, -1);
    mov_info = ov_info(mov_file, -1);

    trackinformation->album = OGG_getFieldText(mov_file_comment, "ALBUM");
    trackinformation->title = OGG_getFieldText(mov_file_comment, "TITLE");
    trackinformation->artist = OGG_getFieldText(mov_file_comment, "ARTIST");
    trackinformation->date = OGG_getFieldText(mov_file_comment, "DATE");
    trackinformation->genre = OGG_getFieldText(mov_file_comment, "GENRE");
    tracknumber = OGG_getFieldText(mov_file_comment, "TRACKNUMBER");
    if (tracknumber != NULL) {
        trackinformation->tracknumber = atoi(tracknumber);
    } else {
        trackinformation->tracknumber = 0;
    }
    // Duration, bitrate and other information
    trackinformation->duration = (int) ov_time_total(mov_file, -1) * 1000;
    trackinformation->bitrate = ov_bitrate(mov_file, -1);
    trackinformation->bitratetype = 2; // VBR
    trackinformation->nochannels = mov_info->channels;
    // Clean up our data structures.
    ov_clear(mov_file);
    g_free(mov_file);
    return;
}
