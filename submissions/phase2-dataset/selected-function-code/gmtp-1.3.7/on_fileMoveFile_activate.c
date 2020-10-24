    GList *List = NULL;
    int64_t targetfolder = 0;

    // If using alternate connection mode, this is disabled.
    if (Preferences.use_alt_access_method) {
        displayInformation(_("The move function is disabled when using the alternate access method for your device."));
        return;
    }

    // Let's check to see if we have anything selected in our treeview?
    if ((List = fileListGetSelection()) == NULL) {
        if (folderListGetSelection() != -1) {
            on_folderRemoveFolder_activate(menuitem, user_data);
        } else {
            displayInformation(_("No files/folders selected?"));
        }
        return;
    }

    // Prompt for the target folder location.
    targetfolder = getTargetFolderLocation();
    if ((targetfolder == -1) || (targetfolder == currentFolderID)) {
        // If the user didn't select a folder, or the target folder is the current selected folder
        // then do nothing.
        return;
    }
    fileMoveTargetFolder = targetfolder;
    fileListClearSelection();
    // List is a list of Iter's to be moved
    g_list_foreach(List, (GFunc) __fileMove, NULL);
    // We have 2 options, manually scan the file structure for that file and manually fix up...
    // or do a rescan...
    // I'll be cheap, and do a full rescan of the device.
    deviceRescan();
}
