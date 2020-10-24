    // What we have to go is scan the entire file tree looking for
    // entries in the same folder as the current and the same
    // storage pool, then we do a string compare (since doing a string
    // compare is so much slower than comparing a few numbers).
    LIBMTP_file_t* tmpfile;
    if(Preferences.use_alt_access_method){
        tmpfile = LIBMTP_Get_Files_And_Folders(DeviceMgr.device, DeviceMgr.devicestorage->id, folderID);
    } else {
        tmpfile = deviceFiles;
    }
    while (tmpfile != NULL) {
        // Check for matching folder ID and storage ID.
        if ((tmpfile->parent_id == currentFolderID) && (tmpfile->storage_id == DeviceMgr.devicestorage->id)) {
            // Now test for the file name (do case insensitive cmp for those odd devices);
            if (g_ascii_strcasecmp(filename, tmpfile->filename) == 0)
                return tmpfile->item_id;
        }
        tmpfile = tmpfile->next;
    }
    return -1;
}
