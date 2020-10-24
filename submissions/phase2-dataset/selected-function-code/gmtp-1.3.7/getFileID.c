uint32_t getFileID(gchar* filename, gboolean ignorepath) {
    LIBMTP_file_t* files = deviceFiles;
    uint32_t folderID = 0;

    // Separate into filename and path...
    gchar* basefilename = g_path_get_basename(filename);
    gchar* dirfilename = g_path_get_dirname(filename);

    // If we are ignoring all path information, then simply scan all files for the filename.
    if (ignorepath == TRUE) {
        while (files != NULL) {
            // Ensure we only check if we are on the current storage device...
            if (files->storage_id == DeviceMgr.devicestorage->id) {
                // See if our filename is the same.
                if (g_ascii_strcasecmp(basefilename, files->filename) == 0) {
                    // We found our file...
                    g_free(basefilename);
                    g_free(dirfilename);
                    return files->item_id;
                }
            }
            files = files->next;
        }
    } else {
        // Lets find the folderid of the path we have, so it makes searching a lot easier...
        folderID = getFolderID(deviceFolders, dirfilename);
        if ((int32_t) folderID == -1) {
            // We don't have this path on the device, so no need to continue checking.
            g_free(basefilename);
            g_free(dirfilename);
            return 0;
        }
        while (files != NULL) {
            // Ensure we only check if we are on the current storage device AND the file is in the correct folder...
            if ((files->storage_id == DeviceMgr.devicestorage->id) && (files->parent_id == folderID)) {
                // See if our filename is the same.
                if (g_ascii_strcasecmp(basefilename, files->filename) == 0) {
                    // We found our file...
                    g_free(basefilename);
                    g_free(dirfilename);
                    return files->item_id;
                }
            }
            files = files->next;
        }
    }
    g_free(basefilename);
    g_free(dirfilename);
    return 0;
}
