    gchar* fullfilename = NULL;
    LIBMTP_folder_t* currentFolder = NULL;
    LIBMTP_file_t* tmpFiles = NULL;

    // Store our current path for safe keeping and generate our new path.
    GString *currentdownload_folder = g_string_new(Preferences.fileSystemDownloadPath->str);
    fullfilename = g_strdup_printf("%s/%s", Preferences.fileSystemDownloadPath->str, foldername);

    // See if folder exists, if not create it.
    if (g_file_test(fullfilename, G_FILE_TEST_IS_DIR) == FALSE) {
        if (mkdir(fullfilename, S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
            g_fprintf(stderr, _("Folder creation failed: %s\n"), fullfilename);
            displayError(g_strconcat(_("Folder creation failed:"), " <b>", fullfilename, "</b>", NULL));
            // Since we can't create that directory, then we simple return from this call...
            return;
        }
    }

    // First we scan for all folders in the current folder, and do those first...
    if (folderID != 0) {
        currentFolder = getCurrentFolderPtr(deviceFolders, folderID);
    } else {
        // If our id = 0 then we are dealing with the root device.
        currentFolder = deviceFolders;
    }

    if (currentFolder == NULL) {
        // This means we don't exist, so bail out.
        return;
    }

    if (currentFolder->child != NULL) {
        // Now we can simple set our download path to the new path, and save all folders/files there.
        Preferences.fileSystemDownloadPath = g_string_assign(Preferences.fileSystemDownloadPath, fullfilename);

        // Call to download all folder/files in that folder...
        folderDownload(currentFolder->child->name, currentFolder->child->folder_id, FALSE);

        // Restore the old download path;
        Preferences.fileSystemDownloadPath = g_string_assign(Preferences.fileSystemDownloadPath, currentdownload_folder->str);
    }

    if ((isParent == FALSE) || (folderID == 0)) {
        if (currentFolder->sibling != NULL) {
            folderDownload(currentFolder->sibling->name, currentFolder->sibling->folder_id, FALSE);
        }
    }
    // Now download all the files whose parentID = folderID;

    // Now we can simple set our download path to the new path, and save all folders/files there.
    Preferences.fileSystemDownloadPath = g_string_assign(Preferences.fileSystemDownloadPath, fullfilename);

    // we no longer need the full folder path, so free it's variable.
    g_free(fullfilename);

    // Start processing all our files.
    tmpFiles = deviceFiles;
    while (tmpFiles != NULL) {
        if ((tmpFiles->parent_id == folderID) && (tmpFiles->storage_id == DeviceMgr.devicestorage->id)) {
            // We have a file in this folder, so download it...
            // But first check to see if it exists, before overwriting it...
            fullfilename = g_strdup_printf("%s/%s", Preferences.fileSystemDownloadPath->str, tmpFiles->filename);
            // Check if file exists?
            if (access(fullfilename, F_OK) != -1) {
                // We have that file already?
                if (Preferences.prompt_overwrite_file_op == TRUE) {
                    if (fileoverwriteop == MTP_ASK) {
                        fileoverwriteop = displayFileOverwriteDialog(tmpFiles->filename);
                    }
                    switch (fileoverwriteop) {
                        case MTP_ASK:
                            break;
                        case MTP_SKIP:
                            fileoverwriteop = MTP_ASK;
                            break;
                        case MTP_SKIP_ALL:
                            break;
                        case MTP_OVERWRITE:
                            filesDownload(tmpFiles->filename, tmpFiles->item_id);
                            fileoverwriteop = MTP_ASK;
                            break;
                        case MTP_OVERWRITE_ALL:
                            filesDownload(tmpFiles->filename, tmpFiles->item_id);
                            break;
                    }
                } else {
                    filesDownload(tmpFiles->filename, tmpFiles->item_id);
                }
            } else {
                filesDownload(tmpFiles->filename, tmpFiles->item_id);
            }
            // Free our file name...
            g_free(fullfilename);
        }
        // Start working on the next file...
        tmpFiles = tmpFiles->next;
    }

    // Restore the old download path;
    Preferences.fileSystemDownloadPath = g_string_assign(Preferences.fileSystemDownloadPath, currentdownload_folder->str);

    // Clean up any tmp items;
    g_string_free(currentdownload_folder, TRUE);
}
