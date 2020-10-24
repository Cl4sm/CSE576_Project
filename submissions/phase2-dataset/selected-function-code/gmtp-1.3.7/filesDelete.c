void filesDelete(gchar* filename, uint32_t objectID) {
    gint ret = 1;
    GSList *node;
    FileListStruc *fileptr;
    uint64_t filesize = 0;
    // Maybe something went wrong, so we disconnected. If so, then simple exit....
    if (DeviceMgr.deviceConnected == FALSE)
        return;

    // Now remove the item from the searchlist if we are in search mode.
    if (inFindMode == TRUE) {
        // searchList
        node = searchList;
        while (node != NULL) {
            fileptr = node->data;
            if (fileptr->itemid == objectID) {
                // remove this node from the main list;
                searchList = g_slist_delete_link(searchList, node);
                g_free_search(fileptr);
                node = NULL;
            } else {
                node = node->next;
            }
        }
    }
    // Get the filesize of the object.
    LIBMTP_file_t * files = deviceFiles;
    while (files != NULL) {
        if (files->item_id == objectID) {
            filesize = files->filesize;
            files = NULL;
        } else {
            files = files->next;
        }
    }

    // Delete the file based on the object ID.
    ret = LIBMTP_Delete_Object(DeviceMgr.device, objectID);
    if (ret != 0) {
        LIBMTP_Dump_Errorstack(DeviceMgr.device);
        LIBMTP_Clear_Errorstack(DeviceMgr.device);
        g_fprintf(stderr, _("\nFailed to delete file %s\n"), filename);
        displayError(g_strconcat(_("Failed to delete file"), " <b>", filename, "</b>", NULL));
    } else {
        // Adjust device storage.
        DeviceMgr.devicestorage->FreeSpaceInBytes += filesize;
        DeviceMgr.devicestorage->FreeSpaceInObjects++;
    }
}
