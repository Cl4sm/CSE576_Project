    const id3_ucs4_t *id3_string;
    struct id3_frame *id3_frame;
    union id3_field *id3_field;
    gchar *rtn_string = NULL;
    enum id3_field_textencoding id3_field_encoding = ID3_FIELD_TEXTENCODING_ISO_8859_1;

    id3_frame = id3_tag_findframe(tag, frame_name, 0);
    if (id3_frame == NULL)
        return NULL;

    id3_field = id3_frame_field(id3_frame, 0);
    if (id3_field && (id3_field_type(id3_field) == ID3_FIELD_TYPE_TEXTENCODING)) {
        id3_field_encoding = id3_field->number.value;
    }
    //if (frame_name == ID3_FRAME_COMMENT) {
    if(g_ascii_strcasecmp(frame_name, ID3_FRAME_COMMENT) == 0){
        id3_field = id3_frame_field(id3_frame, 3);
    } else {
        id3_field = id3_frame_field(id3_frame, 1);
    }
    if (id3_field == NULL)
        return NULL;
        
    if (g_ascii_strcasecmp(frame_name, ID3_FRAME_COMMENT) == 0) {
        id3_string = id3_field_getfullstring(id3_field);
    } else {
        id3_string = id3_field_getstrings(id3_field, 0);
    }
    if (id3_string == NULL)
        return NULL;
        
    if (g_ascii_strcasecmp(frame_name, ID3_FRAME_GENRE) == 0)
        id3_string = id3_genre_name(id3_string);
        
    if (id3_field_encoding == ID3_FIELD_TEXTENCODING_ISO_8859_1) {
        rtn_string = (gchar *) id3_ucs4_latin1duplicate(id3_string);
    } else {
        rtn_string = (gchar *) id3_ucs4_utf8duplicate(id3_string);
    }
    return rtn_string;
}
