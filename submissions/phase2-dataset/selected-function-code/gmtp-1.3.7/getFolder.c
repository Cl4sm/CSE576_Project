    if(Preferences.use_alt_access_method){
        LIBMTP_file_t* tmpfile = LIBMTP_Get_Files_And_Folders(DeviceMgr.device, DeviceMgr.devicestorage->id, folderID);
        while (tmpfile != NULL) {
            // Check for matching folder ID and storage ID.
            if ((tmpfile->parent_id == currentFolderID) && (tmpfile->storage_id == DeviceMgr.devicestorage->id)) {
                // Now test for the file name (do case insensitive cmp for those odd devices);
                if (g_ascii_strcasecmp(foldername, tmpfile->filename) == 0)
                    return tmpfile->item_id;
            }
            tmpfile = tmpfile->next;
        }
    } else {
        LIBMTP_folder_t* folder = getCurrentFolderPtr(deviceFolders, folderID);
        if(folder == NULL){
            return FALSE;
        }
        // Scan for child folders with the same name.
        LIBMTP_folder_t* child = folder->child;
        while(child != NULL){
            if (g_ascii_strcasecmp(foldername, child->name) == 0)
                return child->folder_id;
            child = child->sibling;
        }
    }
    return -1;
}
