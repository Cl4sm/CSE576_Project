void folderDelete(LIBMTP_folder_t* folderptr, guint level) {

    GSList *node;
    FileListStruc *fileptr;

    if (folderptr == NULL) {
        // Sanity check for rogue data or exit here operation, that is no child/sibling to work on.
        return;
    }
    // This is fun, as we have to find all child folders, delete those, as well as all files contained within...
    // First iteratate through all child folders and remove those files in those folders.
    // But first we need to get the folder structure pointer based on the objectID, so we know where to start.
    // So now we have our structure to the current select folder, so we need to cycle through all children and remove any files contained within.
    folderDeleteChildrenFiles(folderptr->folder_id);
    // Now cycle through folders contained in this folder and delete those;
    folderDelete(folderptr->child, level + 1);
    if (level != 0)
        folderDelete(folderptr->sibling, level + 1);
    // That should clear all the children.

    // Now remove the item from the searchlist if we are in search mode.
    if (inFindMode == TRUE) {
        // searchList
        node = searchList;
        while (node != NULL) {
            fileptr = node->data;
            if (fileptr->itemid == folderptr->folder_id) {
                // remove this node from the main list;
                searchList = g_slist_delete_link(searchList, node);
                g_free_search(fileptr);
                node = NULL;
            } else {
                node = node->next;
            }
        }
    }
    // Now do self.
    guint res = LIBMTP_Delete_Object(DeviceMgr.device, folderptr->folder_id);
    if (res != 0) {
        g_fprintf(stderr, _("Couldn't delete folder %s (%x)\n"), folderptr->name, folderptr->folder_id);
        LIBMTP_Dump_Errorstack(DeviceMgr.device);
        LIBMTP_Clear_Errorstack(DeviceMgr.device);
    } else {
        // Adjust device storage.
        DeviceMgr.devicestorage->FreeSpaceInObjects++;
    }
}
