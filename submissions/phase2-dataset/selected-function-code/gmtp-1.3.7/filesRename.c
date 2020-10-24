    // We must first determine, if this is a file, a folder, playlist or album
    // and use the correct API.
    LIBMTP_file_t *genfile = NULL;
    LIBMTP_album_t *albuminfo = NULL;
    LIBMTP_album_t *albumlist = NULL;
    LIBMTP_playlist_t *playlist = NULL;
    LIBMTP_folder_t *folder = NULL;
    GSList *node;
    FileListStruc *fileptr;

    if (filename == NULL) {
        return;
    }
    if (ObjectID == 0) {
        return;
    }

    // Now remove the item from the searchlist if we are in search mode.
    if (inFindMode == TRUE) {
        // searchList
        node = searchList;
        while (node != NULL) {
            fileptr = node->data;
            if (fileptr->itemid == ObjectID) {
                // update the filename appropriately;
                g_free(fileptr->filename);
                fileptr->filename = g_strdup(filename);
                node = NULL;
            } else {
                node = node->next;
            }
        }
    }

    // Lets scan files first.
    genfile = deviceFiles;
    while (genfile != NULL) {
        if (genfile->item_id == ObjectID) {
            // We have our file, so update it.
            LIBMTP_Set_File_Name(DeviceMgr.device, genfile, filename);
            deviceRescan();
            return;
        }
        genfile = genfile->next;
    }

    // Lets scan our albums.
    albuminfo = LIBMTP_Get_Album_List_For_Storage(DeviceMgr.device, DeviceMgr.devicestorage->id);
    albumlist = albuminfo;
    while (albuminfo != NULL) {
        if (albuminfo->album_id == ObjectID) {
            LIBMTP_Set_Album_Name(DeviceMgr.device, albuminfo, filename);
            deviceRescan();
            clearAlbumStruc(albumlist);
            return;
        }
        albuminfo = albuminfo->next;
    }
    clearAlbumStruc(albumlist);

    // Let's scan our playlists.
    playlist = devicePlayLists;
    while (playlist != NULL) {
        if (playlist->playlist_id == ObjectID) {
            // We have our playlist, so update it.
            LIBMTP_Set_Playlist_Name(DeviceMgr.device, playlist, filename);
            deviceRescan();
            return;
        }
        playlist = playlist->next;
    }

    // Lets scan our folders;
    folder = deviceFolders;
    folder = LIBMTP_Find_Folder(folder, ObjectID);
    if (folder != NULL) {
        LIBMTP_Set_Folder_Name(DeviceMgr.device, folder, filename);
        deviceRescan();
        return;
    }

}
