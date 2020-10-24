GSList *filesSearch(gchar *searchstring, gboolean searchfiles, gboolean searchmeta) {
    GSList *list = NULL;
    GPatternSpec *pspec = g_pattern_spec_new(searchstring);
    LIBMTP_file_t *files = deviceFiles;
    GSList *folderIDs = NULL;
    LIBMTP_track_t *tracks = deviceTracks;
    FileListStruc *filestruc = NULL;
    LIBMTP_track_t *trackinfo;
    gchar *tmpstring1 = NULL;
    gchar *tmpstring2 = NULL;
    gchar *tmpstring3 = NULL;
    gchar *tmpstring4 = NULL;

    uint32_t tmpFolderID = currentFolderID;

    // if using alt method, we cycle through all folders on the current storage device...
    if (Preferences.use_alt_access_method) {
        currentFolderID = 0;
        filesUpateFileList();
        files = deviceFiles;
        //buildFolderIDs(&folderIDs, deviceFolders);
    }

    if (searchfiles == TRUE) {
        // Search folders
        // ignore folder search in alt access mode...
        if (!Preferences.use_alt_access_method) {
            folderSearch(pspec, &list, deviceFolders);
        }
        // Search files.
        while (files != NULL) {
            if (files->storage_id == DeviceMgr.devicestorage->id) {
                if (files->filetype == LIBMTP_FILETYPE_FOLDER) {
                    // Add this folder to the list to be searched.
                    folderIDs = g_slist_append(folderIDs, &files->item_id);
                }
                // Make search case insensitive.
                tmpstring1 = g_utf8_strup(files->filename, -1);
                if (g_pattern_match_string(pspec, tmpstring1) == TRUE) {
                    g_free(tmpstring1);
                    tmpstring1 = NULL;
                    // We have found a matching string...
                    filestruc = g_malloc(sizeof (FileListStruc));
                    if (filestruc == NULL) {
                        g_fprintf(stderr, _("malloc in filesSearch failed\n"));
                        displayError(_("Failed searching? 'malloc in filesSearch'\n"));
                        return list;
                    }
                    filestruc->filename = g_strdup(files->filename);
                    filestruc->filesize = files->filesize;
                    if (files->filetype == LIBMTP_FILETYPE_FOLDER) {
                        filestruc->isFolder = TRUE;
                    } else {
                        filestruc->isFolder = FALSE;
                    }
                    filestruc->itemid = files->item_id;
                    filestruc->filetype = files->filetype;
                    filestruc->location = getFullFolderPath(files->parent_id);
                    list = g_slist_append(list, filestruc);
                } else if (searchmeta == TRUE) {
                    // Now if it's a track type file, eg OGG, WMA, MP3 or FLAC, get it's metadata.
                    // search case insensitive.
                    if ((files->filetype == LIBMTP_FILETYPE_MP3) ||
                            (files->filetype == LIBMTP_FILETYPE_OGG) ||
                            (files->filetype == LIBMTP_FILETYPE_FLAC) ||
                            (files->filetype == LIBMTP_FILETYPE_WMA)) {
                        trackinfo = LIBMTP_Get_Trackmetadata(DeviceMgr.device, files->item_id);
                        if (trackinfo != NULL) {
                            // search case insensitive.
                            if (tmpstring1 != NULL) {
                                g_free(tmpstring1);
                                tmpstring1 = NULL;
                            }
                            tmpstring1 = g_utf8_strup(trackinfo->album, -1);
                            tmpstring2 = g_utf8_strup(trackinfo->artist, -1);
                            tmpstring3 = g_utf8_strup(trackinfo->genre, -1);
                            tmpstring4 = g_utf8_strup(trackinfo->title, -1);
                            if ((g_pattern_match_string(pspec, tmpstring1) == TRUE) ||
                                    (g_pattern_match_string(pspec, tmpstring2) == TRUE) ||
                                    (g_pattern_match_string(pspec, tmpstring3) == TRUE) ||
                                    (g_pattern_match_string(pspec, tmpstring4) == TRUE)) {
                                // We have found a matching string...
                                filestruc = g_malloc(sizeof (FileListStruc));
                                if (filestruc == NULL) {
                                    if (tmpstring1 != NULL) {
                                        g_free(tmpstring1);
                                        tmpstring1 = NULL;
                                    }
                                    if (tmpstring2 != NULL) {
                                        g_free(tmpstring2);
                                        tmpstring2 = NULL;
                                    }
                                    if (tmpstring3 != NULL) {
                                        g_free(tmpstring3);
                                        tmpstring3 = NULL;
                                    }
                                    if (tmpstring4 != NULL) {
                                        g_free(tmpstring4);
                                        tmpstring4 = NULL;
                                    }
                                    g_fprintf(stderr, _("malloc in filesSearch failed\n"));
                                    displayError(_("Failed searching? 'malloc in filesSearch'\n"));
                                    return list;
                                }
                                filestruc->filename = g_strdup(files->filename);
                                filestruc->filesize = files->filesize;
                                filestruc->isFolder = FALSE;
                                filestruc->itemid = files->item_id;
                                filestruc->filetype = files->filetype;
                                filestruc->location = getFullFolderPath(files->parent_id);
                                list = g_slist_append(list, filestruc);
                            }
                            if (tmpstring1 != NULL) {
                                g_free(tmpstring1);
                                tmpstring1 = NULL;
                            }
                            if (tmpstring2 != NULL) {
                                g_free(tmpstring2);
                                tmpstring2 = NULL;
                            }
                            if (tmpstring3 != NULL) {
                                g_free(tmpstring3);
                                tmpstring3 = NULL;
                            }
                            if (tmpstring4 != NULL) {
                                g_free(tmpstring4);
                                tmpstring4 = NULL;
                            }
                            LIBMTP_destroy_track_t(trackinfo);
                        }
                    }
                }
                if (tmpstring1 != NULL) {
                    g_free(tmpstring1);
                    tmpstring1 = NULL;
                }
            }
            files = files->next;

            // Update the file list IFF we are using the alt access method
            if ((files == NULL) && (Preferences.use_alt_access_method)) {
                // reached the end of the current folder, so lets move onto the next folder.
                while (files == NULL) {
                    // exit searching for file if we run out of folders.
                    if (folderIDs == NULL) {
                        break;
                    }
                    currentFolderID = *((uint32_t*) folderIDs->data);
                    //printf("Searching folder: %d\n", currentFolderID);
                    filesUpateFileList();
                    folderIDs = folderIDs->next;
                    files = deviceFiles;
                }
            }
        }
    } else if (searchmeta == TRUE) {
        // Search using the Track information only.
        while (tracks != NULL) {
            // Make search case insensitive.
            tmpstring1 = g_utf8_strup(tracks->album, -1);
            tmpstring2 = g_utf8_strup(tracks->artist, -1);
            tmpstring3 = g_utf8_strup(tracks->genre, -1);
            tmpstring4 = g_utf8_strup(tracks->title, -1);
            if ((g_pattern_match_string(pspec, tmpstring1) == TRUE) ||
                    (g_pattern_match_string(pspec, tmpstring2) == TRUE) ||
                    (g_pattern_match_string(pspec, tmpstring3) == TRUE) ||
                    (g_pattern_match_string(pspec, tmpstring4) == TRUE)) {
                // We have found a matching string...
                filestruc = g_malloc(sizeof (FileListStruc));
                if (filestruc == NULL) {
                    if (tmpstring1 != NULL) {
                        g_free(tmpstring1);
                        tmpstring1 = NULL;
                    }
                    if (tmpstring2 != NULL) {
                        g_free(tmpstring2);
                        tmpstring2 = NULL;
                    }
                    if (tmpstring3 != NULL) {
                        g_free(tmpstring3);
                        tmpstring3 = NULL;
                    }
                    if (tmpstring4 != NULL) {
                        g_free(tmpstring4);
                        tmpstring4 = NULL;
                    }
                    g_fprintf(stderr, _("malloc in filesSearch failed\n"));
                    displayError(_("Failed searching? 'malloc in filesSearch'\n"));
                    return list;
                }
                filestruc->filename = g_strdup(tracks->filename);
                filestruc->filesize = tracks->filesize;
                filestruc->isFolder = FALSE;
                filestruc->itemid = tracks->item_id;
                filestruc->filetype = tracks->filetype;
                filestruc->location = getFullFolderPath(tracks->parent_id);
                list = g_slist_append(list, filestruc);
            }
            if (tmpstring1 != NULL) {
                g_free(tmpstring1);
                tmpstring1 = NULL;
            }
            if (tmpstring2 != NULL) {
                g_free(tmpstring2);
                tmpstring2 = NULL;
            }
            if (tmpstring3 != NULL) {
                g_free(tmpstring3);
                tmpstring3 = NULL;
            }
            if (tmpstring4 != NULL) {
                g_free(tmpstring4);
                tmpstring4 = NULL;
            }
            tracks = tracks->next;
        }
    }
    // restore the current folder ID.
    if (Preferences.use_alt_access_method) {
        currentFolderID = tmpFolderID;
    }
    return list;
}
