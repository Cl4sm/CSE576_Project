GSList* getFilesListURI(gchar* rawdata) {
    // The data is just the data in string form
    // Files are in the URI form of file:///filename\n so just look for those,
    // and if found see if a folder or not?
    // Then create a slist of those and use filesAdd() to add them in .
    GSList* filelist;
    gchar* tmpstring;
    gchar *fullpath;
    gchar *filepath;
    gchar *token;

    const char delimit[] = "\n\r";

    filelist = NULL;

    fullpath = g_strdup(rawdata);

    token = strtok(fullpath, delimit);
    while ((token != NULL)) {
        // Now test to see if we have it here...
        filepath = g_strdup(token);
        // See if we have a local file URI, otherwise discard.
        if (!g_ascii_strncasecmp(filepath, "file://", 7)) {
            tmpstring = g_filename_from_uri(filepath, NULL, NULL);

            // See if we have a file or a folder?
            if (g_file_test(tmpstring, G_FILE_TEST_IS_REGULAR) == TRUE) {
                // Add the file to the list
                filelist = g_slist_append(filelist, g_strdup(tmpstring));
            } else {
                // Otherwise we have a folder, so add the folder and all it's contents.
                addFilesinFolder(tmpstring);
            }
            g_free(tmpstring);
        }
        token = strtok(NULL, delimit);
        g_free(filepath);
    }
    g_free(fullpath);
    return filelist;
} // end getFilesListURI()
