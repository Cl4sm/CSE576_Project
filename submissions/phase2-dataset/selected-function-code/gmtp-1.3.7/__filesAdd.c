void __filesAdd(gchar* filename) {
    gchar* filename_stripped = NULL;

    filename_stripped = basename(filename);
    if (Preferences.prompt_overwrite_file_op == FALSE) {
        if(fileExists(filename_stripped) == TRUE){
            filesDelete(filename_stripped, getFile(filename_stripped, currentFolderID));
        }
        filesAdd(filename);
        return;
    }
    // I guess we want to know if we should replace the file, but first
    if (deviceoverwriteop == MTP_ASK) {
        if (fileExists(filename_stripped) == TRUE) {
            deviceoverwriteop = displayFileOverwriteDialog(filename_stripped);
            switch (deviceoverwriteop) {
                case MTP_ASK:
                    break;
                case MTP_SKIP:
                    deviceoverwriteop = MTP_ASK;
                    break;
                case MTP_SKIP_ALL:
                    break;
                case MTP_OVERWRITE:
                    if(fileExists(filename_stripped) == TRUE){
                        filesDelete(filename_stripped, getFile(filename_stripped, currentFolderID));
                    }
                    filesAdd(filename);
                    deviceoverwriteop = MTP_ASK;
                    break;
                case MTP_OVERWRITE_ALL:
                    if(fileExists(filename_stripped) == TRUE){
                        filesDelete(filename_stripped, getFile(filename_stripped, currentFolderID));
                    }
                    filesAdd(filename);
                    break;
            }
        } else {
            if(fileExists(filename_stripped) == TRUE){
                filesDelete(filename_stripped, getFile(filename_stripped, currentFolderID));
            }
            filesAdd(filename);
        }
    } else {
        if (deviceoverwriteop == MTP_OVERWRITE_ALL){
            if(fileExists(filename_stripped) == TRUE){
                filesDelete(filename_stripped, getFile(filename_stripped, currentFolderID));
            }
            filesAdd(filename);
        } else if (deviceoverwriteop == MTP_SKIP_ALL) {
            // Ensure we add files that don't exist.
            if (fileExists(filename_stripped) != TRUE) {
                filesAdd(filename);
            }
        }
    }
}
