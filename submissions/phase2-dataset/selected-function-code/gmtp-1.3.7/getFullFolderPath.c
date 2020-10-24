gchar* getFullFolderPath(uint32_t folderid) {

    gchar* fullfilename = g_strdup("");
    gchar* tmpfilename = NULL;
    uint32_t parent_id = folderid;
    guint stringlength = 0;
    LIBMTP_folder_t* tmpfolder = deviceFolders;
    if(!Preferences.use_alt_access_method){
        // Legacy search since we have a complete folder structure in memory.
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
        
       
    } else {
        // We are using alt access mode, so we need to manually query the device for
        // the parent folder.
        LIBMTP_file_t * f = LIBMTP_Get_Filemetadata(DeviceMgr.device, parent_id);
        while(f != NULL){
            // we have the entry
            tmpfilename = g_strdup_printf("%s/%s", f->filename, fullfilename);
            g_free(fullfilename);
            fullfilename = tmpfilename;
            parent_id = f->parent_id;
            LIBMTP_destroy_file_t(f);
            f = LIBMTP_Get_Filemetadata(DeviceMgr.device, parent_id);
        }
    }
    // Add in leading slash if needed
    if (*fullfilename != '/') {
        tmpfilename = g_strdup_printf("/%s", fullfilename);
        g_free(fullfilename);
        fullfilename = tmpfilename;
    }
    // Remove trailing slash if needed.
    stringlength = strlen(fullfilename);
    if (stringlength > 1) {
        fullfilename[stringlength - 1] = '\0';
    }
    return fullfilename;
}
