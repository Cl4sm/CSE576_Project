    gchar* fullfilename = NULL;
    gchar* tmpfilename = NULL;
    uint32_t parent_id = 0;
    LIBMTP_file_t* tmpfile = deviceFiles;
    LIBMTP_folder_t* tmpfolder = deviceFolders;

    // Find our file...
    while (tmpfile != NULL) {
        if (tmpfile->item_id == item_id) {
            fullfilename = g_strdup(tmpfile->filename);
            parent_id = tmpfile->parent_id;
            tmpfile = NULL;
        } else {
            tmpfile = tmpfile->next;
        }
    }
    // Let's see if we have a filename?
    if (fullfilename != NULL) {
        // Now let's prepend the parent folder names to it...
        while (tmpfolder != NULL) {
            tmpfolder = getCurrentFolderPtr(deviceFolders, parent_id);
            if (tmpfolder != NULL) {
                // We have something.
                tmpfilename = g_strdup_printf("%s/%s", tmpfolder->name, fullfilename);
                g_free(fullfilename);
                fullfilename = tmpfilename;
                parent_id = tmpfolder->parent_id;
            }
        }
    }
    return fullfilename;
}
