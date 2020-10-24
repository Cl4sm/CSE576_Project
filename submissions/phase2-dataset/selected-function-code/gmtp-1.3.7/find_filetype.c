    LIBMTP_filetype_t filetype = LIBMTP_FILETYPE_UNKNOWN;
    gchar *fileext;
    gint i;
    gint j = sizeof (file_ext) / sizeof (MTP_file_ext_struct);

    fileext = rindex(filename, '.');
    // This accounts for the case with a filename without any "." (period).
    if (!fileext) {
        fileext = "";
    } else {
        ++fileext;
    }
    // Now cycle through the array of extensions, and get the associated
    // libmtp filetype.
    for (i = 0; i < j; i++) {
        if (g_ascii_strcasecmp(fileext, file_ext[i].file_extension) == 0) {
            filetype = file_ext[i].file_type;
            break;
        }
    }
    //if (filetype == 0) {
    //    filetype = LIBMTP_FILETYPE_UNKNOWN;
    //}
    return filetype;
}
