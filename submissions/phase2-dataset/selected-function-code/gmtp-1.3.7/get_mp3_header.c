gboolean get_mp3_header(FILE * mp3_file, MP3_header * header_info) {
    uint8_t raw_header[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t framesize = 0;
    uint32_t id3_footer = 0;
    uint32_t id3_size = 0;

resync:
    if (fread(&raw_header, sizeof (uint8_t)*10, 1, mp3_file) < 1) {
        header_info->header_sync = 0;
        return FALSE;
    }
    /*
        // Print header info for debugging
        printf("Offset: %llx : ", ftell(mp3_file) - 10);
        for (int i = 0; i < 10; i++) {
            printf("0x%x ", raw_header[i]);
        }
        printf("\n");
     */

    header_info->header_sync = ((raw_header[0] << 4) | ((raw_header[1]&0xE0) >> 4));

    // Check for ID3 Tags
    while (header_info->header_sync != 0xFFE) {
        // We may have ID3 Tag.
        if (raw_header[0] == 'I' && raw_header[1] == 'D' && raw_header[2] == '3') {
            // We have a ID3 header...
            // Now skip it.
            id3_footer = (raw_header[5] >> 4) & 0x1;
            id3_size = ((uint32_t) (raw_header[6] << 21) +
                (uint32_t) (raw_header[7] << 14) +
                (uint32_t) (raw_header[8] << 7) +
                (uint32_t) (raw_header[9]));
            if (id3_footer == 1) {
                fseek(mp3_file, (id3_size + 10), SEEK_CUR);
            } else {
                fseek(mp3_file, (id3_size), SEEK_CUR);
            }
            if (fread(&raw_header, sizeof (uint8_t)*10, 1, mp3_file) < 1) {
                header_info->header_sync = 0;
                return FALSE;
            }
            /*
                        // Print header info for debugging
                        printf("Offset: %llx : ", ftell(mp3_file) - 10);
                        for (int i = 0; i < 10; i++) {
                            printf("0x%x ", raw_header[i]);
                        }
                        printf("\n");
             */
            header_info->header_sync = ((raw_header[0] << 4) | ((raw_header[1]&0xE0) >> 4));
        } else {
            // Not a valid frame, nor an ID3 frame?
            // Attempt resync;
            header_info->header_sync = 0;
            fseek(mp3_file, -9, SEEK_CUR);
            goto resync;
            //return FALSE;
        }
    }

    header_info->version = (raw_header[1] >> 3) & 0x3;
    header_info->layer = (raw_header[1] >> 1) & 0x3;
    header_info->crc = raw_header[1] & 0x1;
    header_info->bitrate = (raw_header[2] >> 4) & 0xF;
    header_info->samplerate = (raw_header[2] >> 2) & 0x3;
    header_info->padding = (raw_header[2] >> 1) & 0x1;
    header_info->private_bit = (raw_header[2]) & 0x1;
    header_info->channel_mode = (raw_header[3] >> 6) & 0x3;
    header_info->mode_extension = (raw_header[3] >> 4) & 0x3;
    header_info->copyright = (raw_header[3] >> 3) & 0x1;
    header_info->original = (raw_header[3] >> 2) & 0x1;
    header_info->emphasis = (raw_header[3]) & 0x3;

    // Sanity checks --
    if ((header_info->header_sync != 0xFFE)
        || (header_info->version == 0x1) // Reserved value
        //|| (header_info->layer == 0x1)    // We only care about layer 3 data
        || (header_info->bitrate == 0xF) // Bad value
        || (header_info->samplerate == 0x3)) { // Reserved value
        header_info->header_sync = 0;
        // Attempt resync;
        header_info->header_sync = 0;
        fseek(mp3_file, -9, SEEK_CUR);
        goto resync;
        //return FALSE;
    }
    // We have a valid header, so forward to next possible frame.
    // FrameSize = (samples per sec / 8)  * BitRate / (SampleRate + Padding).

    // Layer I
    if (header_info->layer == 3) {
        framesize = ((12 * mp3_bitrate[header_info->version & 0x1][header_info->layer - 1][header_info->bitrate] * 1000
            / mp3_samplerate[header_info->version & 0x1][header_info->samplerate]
            ) + header_info->padding) * 4;
    } else {
        // Layer 2 and Layer 3
        framesize = (mp3_sampleperframe[header_info->layer - 1][header_info->version] / 8 )     // Samples per frame.
            * mp3_bitrate[header_info->version & 0x1][header_info->layer - 1][header_info->bitrate] * 1000 // Bitrate
            / (mp3_samplerate[header_info->version & 0x1][header_info->samplerate] // Sample Rate
            ) + header_info->padding; // Padding bit
    }
    fseek(mp3_file, (framesize - 10), SEEK_CUR);
    return TRUE;
}
