void get_asf_tags(gchar *filename, LIBMTP_track_t *trackinformation) {
    FILE *ASF_File;
    GUID Header_GUID;
    GUID Stream_GUID;
    uint32_t Header_Blocks;
    uint64_t Object_Size;
    long ASF_File_Position;

    // Content Object
    uint16_t Title_Length = 0;
    uint16_t Author_Length = 0;
    uint16_t Copyright_Length = 0;
    uint16_t Description_Length = 0;
    uint16_t Rating_Length = 0;

    gchar *Title = NULL;
    gchar *Author = NULL;

    // Extended Content Object
    uint16_t Content_Descriptors_Count = 0;
    uint16_t Descriptor_Name_Length = 0;
    gchar *Descriptor_Name = NULL;
    gchar *Descriptor_Name_UTF16 = NULL;
    uint16_t Descriptor_Value_Type = 0;
    uint16_t Descriptor_Value_Length = 0;
    uint64_t Descriptor_Value = 0;
    gchar *Descriptor_Value_Str = NULL;
    gchar *Descriptor_Value_Str_UTF16 = NULL;

    // Audio Object
    uint16_t Stream_Channels;
    uint32_t Stream_Bitrate;

    // File Object
    uint64_t Stream_Duration;


    ASF_File = fopen(filename, "r");
    if (ASF_File == NULL)
        return;

    // Get our header GUID and make sure this is it.
    size_t i = fread(&Header_GUID, sizeof (GUID), 1, ASF_File);
    if (!memcmp(&Header_GUID, &ASF_header, sizeof (GUID))) {
        // If not exit.
        fclose(ASF_File);
        return;
    }
    if(i != 1){
        // If not exit.
        fclose(ASF_File);
        return;
    }
    // Skip the rest of the header area;
    fseek(ASF_File, 8, SEEK_CUR);
    i = fread(&Header_Blocks, sizeof (uint32_t), 1, ASF_File);
    fseek(ASF_File, 2, SEEK_CUR);

    // We should be at the start of the header blocks;
    // Header_blocks has the number of header objects that we can test.
    while (Header_Blocks--) {
        i = fread(&Header_GUID, sizeof (GUID), 1, ASF_File);
        if (memcmp(&Header_GUID, &ASF_comment_header, sizeof (GUID)) == 0) {
            // We have our standard comment header block;

            // Get the size of the object, and the current file position.
            i = fread(&Object_Size, sizeof (uint64_t), 1, ASF_File);
            ASF_File_Position = ftell(ASF_File);
            // Get our field lengths.
            i = fread(&Title_Length, sizeof (uint16_t), 1, ASF_File);
            i = fread(&Author_Length, sizeof (uint16_t), 1, ASF_File);
            i = fread(&Copyright_Length, sizeof (uint16_t), 1, ASF_File);
            i = fread(&Description_Length, sizeof (uint16_t), 1, ASF_File);
            i = fread(&Rating_Length, sizeof (uint16_t), 1, ASF_File);
            // Since we only need Title and Author, we only need to alloc memory for those two.
            Title = g_malloc0(Title_Length + 0x10);
            Author = g_malloc0(Author_Length + 0x10);
            i = fread(Title, Title_Length, 1, ASF_File);
            i = fread(Author, Author_Length, 1, ASF_File);
            // Set our track information
            trackinformation->title = g_utf16_to_utf8((const gunichar2 *) Title, Title_Length, NULL, NULL, NULL);
            trackinformation->artist = g_utf16_to_utf8((const gunichar2 *) Author, Author_Length, NULL, NULL, NULL);
            // Free our memory that we used to load in the fields.
            g_free(Title);
            g_free(Author);
            Title = NULL;
            Author = NULL;
            // Set our file position so it's ready to read in the next GUID Header.
            fseek(ASF_File, ASF_File_Position, SEEK_SET);
            fseek(ASF_File, (Object_Size - sizeof (uint64_t) - sizeof (GUID)), SEEK_CUR);
        } else {
            if (memcmp(&Header_GUID, &ASF_extended_content_header, sizeof (GUID)) == 0) {
                // We have our standard comment header block;
                //g_printf("WMA: Found our extended comment block\n");
                // Get the size of the object, and the current file position.
                i = fread(&Object_Size, sizeof (uint64_t), 1, ASF_File);
                ASF_File_Position = ftell(ASF_File);
                // Get the number of Descripions field we have, as we will need to cycle through them all.
                i = fread(&Content_Descriptors_Count, sizeof (uint16_t), 1, ASF_File);
                while (Content_Descriptors_Count--) {
                    // These themselves are Objects within the main extended content header, which we need to handle.
                    // Format is:
                    // Descriptor Name Length (word)
                    // Descriptor Name (varies)
                    // Descriptor Value Type (word)
                    // Descriptor Value Length (word)
                    // Descriptor Value (varies - depend on Value Type).
                    Descriptor_Name_Length = 0;
                    Descriptor_Name = NULL;
                    Descriptor_Name_UTF16 = NULL;
                    Descriptor_Value_Type = 0;
                    Descriptor_Value_Length = 0;
                    Descriptor_Value = 0;
                    Descriptor_Value_Str = NULL;
                    Descriptor_Value_Str_UTF16 = NULL;
                    // Get our Descriptor Name.
                    i = fread(&Descriptor_Name_Length, sizeof (uint16_t), 1, ASF_File);
                    Descriptor_Name_UTF16 = g_malloc0(Descriptor_Name_Length + 0x10);
                    i = fread(Descriptor_Name_UTF16, Descriptor_Name_Length, 1, ASF_File);
                    Descriptor_Name = g_utf16_to_utf8((const gunichar2 *) Descriptor_Name_UTF16,
                        Descriptor_Name_Length, NULL, NULL, NULL);
                    // Get our Value Type and Value Length
                    i = fread(&Descriptor_Value_Type, sizeof (uint16_t), 1, ASF_File);
                    i = fread(&Descriptor_Value_Length, sizeof (uint16_t), 1, ASF_File);
                    switch (Descriptor_Value_Type) {
                        case 0: // String;
                        case 1: // Binary;
                            Descriptor_Value_Str_UTF16 = g_malloc0(Descriptor_Value_Length + 0x10);
                            i = fread(Descriptor_Value_Str_UTF16, Descriptor_Value_Length, 1, ASF_File);
                            Descriptor_Value_Str = g_utf16_to_utf8((const gunichar2 *) Descriptor_Value_Str_UTF16,
                                Descriptor_Value_Length, NULL, NULL, NULL);
                            // We have out key=value pair so lets look for our desired  keys 'WM/AlbumTitle', 'WM/Genre' and 'WM/Year'
                            if (g_ascii_strcasecmp(Descriptor_Name, "WM/AlbumTitle\0") == 0) {
                                // We have the album Title;
                                trackinformation->album = g_strdup(Descriptor_Value_Str);
                            } else {
                                if (g_ascii_strcasecmp(Descriptor_Name, "WM/Genre\0") == 0) {
                                    // We have the album Genre;
                                    trackinformation->genre = g_strdup(Descriptor_Value_Str);
                                } else {
                                    if (g_ascii_strcasecmp(Descriptor_Name, "WM/Year\0") == 0) {
                                        // We have the album Year;
                                        trackinformation->date = g_strdup(Descriptor_Value_Str);
                                    }
                                }
                            }
                            break;
                        case 2: // Boolean (DWORD)
                        case 3: // DWORD
                        case 4: // QWORD
                        case 5: // WORD
                            if (Descriptor_Value_Length > sizeof (Descriptor_Value))
                                Descriptor_Value_Length = sizeof (Descriptor_Value);
                            i = fread(&Descriptor_Value, Descriptor_Value_Length, 1, ASF_File);
                            if ((g_ascii_strcasecmp(Descriptor_Name, "WM/Track\0") == 0)) {
                                trackinformation->tracknumber = Descriptor_Value + 1;
                            } else {
                                if (g_ascii_strcasecmp(Descriptor_Name, "WM/TrackNumber\0") == 0)
                                    trackinformation->tracknumber = Descriptor_Value;
                            }
                            break;
                        default: // Unknown so skip it.
                            fseek(ASF_File, Descriptor_Value_Length, SEEK_CUR);
                            break;
                    }

                    // Free up our allocated memory;
                    g_free(Descriptor_Name);
                    g_free(Descriptor_Name_UTF16);
                    g_free(Descriptor_Value_Str);
                    g_free(Descriptor_Value_Str_UTF16);
                }

                // Set our file position so it's ready to read in the next GUID Header.
                fseek(ASF_File, ASF_File_Position, SEEK_SET);
                fseek(ASF_File, (Object_Size - sizeof (uint64_t) - sizeof (GUID)), SEEK_CUR);
            } else {
                if (memcmp(&Header_GUID, &ASF_Stream_header, sizeof (GUID)) == 0) {
                    // We have an audio header for the track information.
                    i = fread(&Object_Size, sizeof (uint64_t), 1, ASF_File);
                    ASF_File_Position = ftell(ASF_File);

                    // Read in the stream type GUID
                    i = fread(&Stream_GUID, sizeof (GUID), 1, ASF_File);
                    if (memcmp(&Stream_GUID, &ASF_Audio_Media_header, sizeof (GUID)) == 0) {
                        // We have an audio header.
                        fseek(ASF_File, 38, SEEK_CUR);
                        // We should be pointing at our audio stream data block.
                        fseek(ASF_File, sizeof (uint16_t), SEEK_CUR); // Skip CODEC ID
                        i = fread(&Stream_Channels, sizeof (uint16_t), 1, ASF_File);
                        fseek(ASF_File, 4, SEEK_CUR); // Skip Samples per second
                        i = fread(&Stream_Bitrate, sizeof (uint32_t), 1, ASF_File);

                        trackinformation->nochannels = Stream_Channels;
                        trackinformation->bitrate = Stream_Bitrate * 8; // This value is in BYTES
                        trackinformation->bitratetype = 0; // Not used
                    }
                    // Set our file position so it's ready to read in the next GUID Header.
                    fseek(ASF_File, ASF_File_Position, SEEK_SET);
                    fseek(ASF_File, (Object_Size - sizeof (uint64_t) - sizeof (GUID)), SEEK_CUR);
                } else {
                    if (memcmp(&Header_GUID, &ASF_File_Properties_header, sizeof (GUID)) == 0) {
                        // We have a file header for the track information.
                        i = fread(&Object_Size, sizeof (uint64_t), 1, ASF_File);
                        ASF_File_Position = ftell(ASF_File);
                        // Skip File ID, Filesize, Creation Date and Data Packets Count
                        fseek(ASF_File, (sizeof (GUID) + (sizeof (uint64_t) * 3)), SEEK_CUR);
                        i = fread(&Stream_Duration, sizeof (uint64_t), 1, ASF_File);
                        // Convert from 1/100ths nano sec to millisec.
                        trackinformation->duration = Stream_Duration / 10000;

                        fseek(ASF_File, ASF_File_Position, SEEK_SET);
                        fseek(ASF_File, (Object_Size - sizeof (uint64_t) - sizeof (GUID)), SEEK_CUR);
                    } else {
                        // Skip this header;
                        i = fread(&Object_Size, sizeof (uint64_t), 1, ASF_File);
                        fseek(ASF_File, (Object_Size - sizeof (uint64_t) - sizeof (GUID)), SEEK_CUR);
                    }
                }
            }
        }
    }
    fclose(ASF_File);
    return;
}
