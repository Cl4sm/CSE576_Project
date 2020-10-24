void get_mp3_info(gchar *filename, MP3_Info *mp3_struct) {

    FILE * mp3_file = NULL;
    struct stat sb;
    MP3_header header_info;
    uint32_t initial_bitrate = 0;
    uint32_t new_bitrate = 0;
    uint64_t total_bitrate = 0;
    uint32_t frames_sampled = 0;
    uint64_t filesize = 0;

    // Init our struct that has been passed to us, and return defaults even if
    // things go wrong.
    mp3_struct->VBR = 1;
    mp3_struct->bitrate = 0;
    mp3_struct->channels = 0;
    mp3_struct->duration = 0;

    mp3_file = fopen(filename, "r");
    if (mp3_file == NULL)
        return;

    if (stat(filename, &sb) == -1) {
        perror("stat");
        fclose(mp3_file);
        return;
    }

    filesize = sb.st_size;

    if (get_mp3_header(mp3_file, &header_info) == TRUE) {
        initial_bitrate = mp3_bitrate[header_info.version & 0x1][header_info.layer - 1][header_info.bitrate];
        total_bitrate = initial_bitrate;
        frames_sampled = 1;
        if (header_info.channel_mode == 0x3) {
            mp3_struct->channels = 1;
        } else {
            mp3_struct->channels = 2;
        }
        // Scan the full file for all frames.
        while ((ftell(mp3_file) < (int64_t) (filesize - 128)) && (get_mp3_header(mp3_file, &header_info) == TRUE)) {
            new_bitrate = mp3_bitrate[header_info.version & 0x1][header_info.layer - 1][header_info.bitrate];
            total_bitrate += new_bitrate;
            frames_sampled++;
            if (new_bitrate != initial_bitrate)
                mp3_struct->VBR = 2;
        }
        if (mp3_struct->VBR != 2) {
            mp3_struct->bitrate = initial_bitrate * 1000;
        } else {
            mp3_struct->bitrate = (total_bitrate / frames_sampled) * 1000;
        }
        //mp3_struct->duration = frames_sampled * 26;
        mp3_struct->duration = (double) frames_sampled * 26.00;
        // Each frame lasts for 26ms, so just multiple the number of frames by 26 to get our duration
    }
}
