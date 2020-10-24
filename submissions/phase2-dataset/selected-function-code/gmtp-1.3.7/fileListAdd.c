    GtkTreeIter rowIter;
    //gchar *filename = NULL;
    gchar *filename_hid = NULL;
    gchar *filesize = NULL;
    gchar *filetype = NULL;
    gchar *trackduration = NULL;
    gchar *tracknumber = NULL;
    gchar *fileext = NULL;
    LIBMTP_folder_t *tmpfolder;
    LIBMTP_file_t *tmpfile;
    guint parentID;
    GdkPixbuf *image = NULL;

    // Confirm our currentFolder exists otherwise goto the root folder.
    if (!Preferences.use_alt_access_method) {
        tmpfolder = getCurrentFolderPtr(deviceFolders, currentFolderID);
        if (tmpfolder == NULL)
            currentFolderID = 0;
    } else {
        // deviceFolders is NULL or has garbage. So assume that we actually do exist!
        filesUpateFileList();
    }
    // This ensure that if the current folder or a parent of is deleted in the find mode,
    // the current folder is reset to something sane.

    // Start our file listing.
    if (inFindMode == FALSE) {
        // Since we are in normal mode, hide the location Column.
        gtk_tree_view_column_set_visible(column_Location, FALSE);
        // We start with the folder list...
        if (currentFolderID != 0) {
            // If we are not folderID = 0; then...
            image = gdk_pixbuf_new_from_file(file_folder_png, NULL);
            // Scan the folder list for the current folderID, and set the parent ID,
            if (!Preferences.use_alt_access_method) {
                tmpfolder = deviceFolders;
                parentID = getParentFolderID(tmpfolder, currentFolderID);
            } else {
                parentID = *((guint*) g_queue_peek_tail(stackFolderIDs));
            }
            // Now add in the row information.
            gtk_list_store_append(GTK_LIST_STORE(fileList), &rowIter);
            gtk_list_store_set(GTK_LIST_STORE(fileList), &rowIter,
                    //COL_FILENAME, "< .. >",
                    COL_FILENAME_HIDDEN, "     < .. >",
                    COL_FILENAME_ACTUAL, "..",
                    COL_FILESIZE, "",
                    COL_FILEID, parentID,
                    COL_ISFOLDER, TRUE,
                    COL_FILESIZE_HID, (guint64) 0,
                    COL_ICON, image,
                    -1);

            // Indicate we are done with this image.
            g_object_unref(image);
        }

        // Only use device folders if using normal display mode.
        if (!Preferences.use_alt_access_method) {
            // What we scan for is the folder's details where 'parent_id' == currentFolderID and display those.
            tmpfolder = getParentFolderPtr(deviceFolders, currentFolderID);
            while (tmpfolder != NULL) {
                if ((tmpfolder->parent_id == currentFolderID) && (tmpfolder->storage_id == DeviceMgr.devicestorage->id)) {
                    image = gdk_pixbuf_new_from_file(file_folder_png, NULL);
                    gtk_list_store_append(GTK_LIST_STORE(fileList), &rowIter);
                    //filename = g_strdup_printf("< %s >", tmpfolder->name);
                    filename_hid = g_strdup_printf("     < %s >", tmpfolder->name);
                    gtk_list_store_set(GTK_LIST_STORE(fileList), &rowIter,
                            //COL_FILENAME, filename,
                            COL_FILENAME_HIDDEN, filename_hid,
                            COL_FILENAME_ACTUAL, tmpfolder->name,
                            COL_FILESIZE, "",
                            COL_FILEID, tmpfolder->folder_id,
                            COL_ISFOLDER, TRUE,
                            COL_FILESIZE_HID, (guint64) 0,
                            COL_ICON, image,
                            -1);
                    //g_free(filename);
                    g_free(filename_hid);
                    // Indicate we are done with this image.
                    g_object_unref(image);
                }
                tmpfolder = tmpfolder->sibling;
            }
        }
        // We don't destroy the structure, only on a rescan operation.

        // We scan for files in the file details we 'parent_id' == currentFolderID and display those.
        tmpfile = deviceFiles;
        while (tmpfile != NULL) {

            if ((tmpfile->parent_id == currentFolderID) && (tmpfile->storage_id == DeviceMgr.devicestorage->id)) {
                gtk_list_store_append(GTK_LIST_STORE(fileList), &rowIter);
                filesize = calculateFriendlySize(tmpfile->filesize);
                fileext = rindex(tmpfile->filename, '.');
                // This accounts for the case with a filename without any "." (period).
                if (!fileext) {
                    filetype = g_strconcat(g_ascii_strup(tmpfile->filename, -1), " File", NULL);
                } else {
                    filetype = g_strconcat(g_ascii_strup(++fileext, -1), " File", NULL);
                }
                
                // Get the type, and if it's one of the supported types, get the metadata for it.
                gboolean useTrack = (tmpfile->filetype == LIBMTP_FILETYPE_MP3) ||
                        (tmpfile->filetype == LIBMTP_FILETYPE_OGG) ||
                        (tmpfile->filetype == LIBMTP_FILETYPE_FLAC) ||
                        (tmpfile->filetype == LIBMTP_FILETYPE_WMA);
                // Ignore track metadata if we are treating all media as regular files.
                if(Preferences.allmediaasfiles){
                    useTrack = FALSE;
                }
                
                // Now if it's a track type file, eg OGG, WMA, MP3 or FLAC, get it's metadata.
                if (useTrack) {
                    LIBMTP_track_t *trackinfo;
                    trackinfo = LIBMTP_Get_Trackmetadata(DeviceMgr.device, tmpfile->item_id);
                    if (trackinfo != NULL) {
                        trackduration = g_strdup_printf("%d:%.2d", (int) ((trackinfo->duration / 1000) / 60),
                                (int) ((trackinfo->duration / 1000) % 60));
                        if (trackinfo->tracknumber != 0) {
                            tracknumber = g_strdup_printf("%d", trackinfo->tracknumber);
                        } else {
                            tracknumber = g_strdup(" ");
                        }
                        // Some basic sanitisation.
                        if (trackinfo->title == NULL) trackinfo->title = g_strdup("");
                        if (trackinfo->artist == NULL) trackinfo->artist = g_strdup("");
                        if (trackinfo->album == NULL) trackinfo->album = g_strdup("");
                        if (trackinfo->date == NULL) {
                            trackinfo->date = g_strdup("");
                        } else {
                            if (strlen(trackinfo->date) > 4)
                                trackinfo->date[4] = '\0'; // Shorten the string to year only, yes this is nasty...
                        }
                        if (trackinfo->genre == NULL) trackinfo->genre = g_strdup("");

                        // Icon
                        image = gdk_pixbuf_new_from_file(file_audio_png, NULL);

                        gtk_list_store_set(GTK_LIST_STORE(fileList), &rowIter,
                                //COL_FILENAME, tmpfile->filename,
                                COL_FILENAME_HIDDEN, tmpfile->filename,
                                COL_FILENAME_ACTUAL, tmpfile->filename,
                                COL_FILESIZE, filesize,
                                COL_FILEID, tmpfile->item_id,
                                COL_ISFOLDER, FALSE,
                                COL_FILESIZE_HID, tmpfile->filesize,
                                COL_TYPE, filetype,
                                COL_TRACK_NUMBER, tracknumber,
                                COL_TRACK_NUMBER_HIDDEN, trackinfo->tracknumber,
                                COL_TITLE, trackinfo->title,
                                COL_FL_ARTIST, trackinfo->artist,
                                COL_FL_ALBUM, trackinfo->album,
                                COL_YEAR, trackinfo->date,
                                COL_GENRE, trackinfo->genre,
                                COL_DURATION, trackduration,
                                COL_DURATION_HIDDEN, trackinfo->duration,
                                COL_ICON, image,
                                -1);
                        g_free(trackduration);
                        g_free(tracknumber);
                        trackduration = NULL;
                        tracknumber = NULL;
                        // Indicate we are done with this image.
                        g_object_unref(image);
                        LIBMTP_destroy_track_t(trackinfo);
                    } else {
                        LIBMTP_Dump_Errorstack(DeviceMgr.device);
                        LIBMTP_Clear_Errorstack(DeviceMgr.device);
                    }
                } else {
                    // Determine the file type.
                    if (LIBMTP_FILETYPE_IS_AUDIO(tmpfile->filetype)) {
                        image = gdk_pixbuf_new_from_file(file_audio_png, NULL);
                    } else if (LIBMTP_FILETYPE_IS_AUDIOVIDEO(tmpfile->filetype)) {
                        image = gdk_pixbuf_new_from_file(file_video_png, NULL);
                    } else if (LIBMTP_FILETYPE_IS_VIDEO(tmpfile->filetype)) {
                        image = gdk_pixbuf_new_from_file(file_video_png, NULL);
                    } else if (LIBMTP_FILETYPE_IS_IMAGE(tmpfile->filetype)) {
                        image = gdk_pixbuf_new_from_file(file_image_png, NULL);
                    } else if (tmpfile->filetype == LIBMTP_FILETYPE_ALBUM) {
                        image = gdk_pixbuf_new_from_file(file_album_png, NULL);
                    } else if (tmpfile->filetype == LIBMTP_FILETYPE_PLAYLIST) {
                        image = gdk_pixbuf_new_from_file(file_playlist_png, NULL);
                    } else if (tmpfile->filetype == LIBMTP_FILETYPE_TEXT) {
                        image = gdk_pixbuf_new_from_file(file_textfile_png, NULL);
                    } else if (tmpfile->filetype == LIBMTP_FILETYPE_FOLDER) {
                        image = gdk_pixbuf_new_from_file(file_folder_png, NULL);
                    } else {
                        image = gdk_pixbuf_new_from_file(file_generic_png, NULL);
                    }

                    // let folders through IFF we are in alt connection mode.
                    if (!Preferences.use_alt_access_method && tmpfile->filetype == LIBMTP_FILETYPE_FOLDER) {
                        goto skipAdd;
                    }

                    // Set folder type.
                    int isFolder = FALSE;
                    if (tmpfile->filetype == LIBMTP_FILETYPE_FOLDER) {
                        isFolder = TRUE;
                        filesize = g_strdup("");
                        filetype = g_strdup("");
                        filename_hid = g_strdup_printf("     < %s >", tmpfile->filename);
                    } else {
                        filename_hid = g_strdup(tmpfile->filename);
                    }

                    // Otherwise just show the file information
                    gtk_list_store_set(GTK_LIST_STORE(fileList), &rowIter,
                            //COL_FILENAME, tmpfile->filename,
                            COL_FILENAME_HIDDEN, filename_hid,
                            COL_FILENAME_ACTUAL, tmpfile->filename,
                            COL_FILESIZE, filesize,
                            COL_FILEID, tmpfile->item_id,
                            COL_ISFOLDER, isFolder,
                            COL_FILESIZE_HID, tmpfile->filesize,
                            COL_TYPE, filetype,
                            COL_ICON, image,
                            -1);
                    g_free(filename_hid);
skipAdd:

                    // Indicate we are done with this image.
                    g_object_unref(image);
                }

                if (filetype != NULL)
                    g_free(filetype);
                filetype = NULL;

                if (filesize != NULL)
                    g_free(filesize);
                filesize = NULL;
            }
            tmpfile = tmpfile->next;
        }

        // Now update the title bar with our folder name.
        if (!Preferences.use_alt_access_method) {
            setWindowTitle(getFullFolderPath(currentFolderID));
        } else {
            // Construct the place string based on the contents of the stackFolderNameQueue.
            gchar* fullfilename = g_strdup("");
            gchar* tmpfilename = NULL;
            guint stringlength = 0;
            int items = g_queue_get_length(stackFolderNames);
            
            // Add in our names;
            while(items-- > 0){
                tmpfilename = g_strdup_printf("%s/%s", (gchar *)g_queue_peek_nth(stackFolderNames, items), fullfilename);
                g_free(fullfilename);
                fullfilename = tmpfilename;
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
            setWindowTitle(fullfilename);
        }
    } else {
        // We are in search mode, so use the searchList instead as our source!
        gint item_count = 0;
        GSList *tmpsearchList = searchList;
        FileListStruc *itemdata = NULL;
        while (tmpsearchList != NULL) {
            // Add our files/folders.
            itemdata = tmpsearchList->data;
            item_count++;
            // If a folder...
            if (itemdata->isFolder == TRUE) {
                image = gdk_pixbuf_new_from_file(file_folder_png, NULL);
                gtk_list_store_append(GTK_LIST_STORE(fileList), &rowIter);
                //filename = g_strdup_printf("< %s >", tmpfolder->name);
                filename_hid = g_strdup_printf("     < %s >", itemdata->filename);
                gtk_list_store_set(GTK_LIST_STORE(fileList), &rowIter,
                        //COL_FILENAME, filename,
                        COL_FILENAME_HIDDEN, filename_hid,
                        COL_FILENAME_ACTUAL, itemdata->filename,
                        COL_FILESIZE, "",
                        COL_FILEID, itemdata->itemid,
                        COL_ISFOLDER, TRUE,
                        COL_FILESIZE_HID, (guint64) 0,
                        COL_ICON, image,
                        COL_LOCATION, itemdata->location,
                        -1);
                //g_free(filename);
                g_free(filename_hid);
                // Indicate we are done with this image.
                g_object_unref(image);

            } else {
                // else if a file...
                gtk_list_store_append(GTK_LIST_STORE(fileList), &rowIter);
                filesize = calculateFriendlySize(itemdata->filesize);
                fileext = rindex(itemdata->filename, '.');
                // This accounts for the case with a filename without any "." (period).
                if (!fileext) {
                    filetype = g_strconcat(g_ascii_strup(itemdata->filename, -1), " File", NULL);
                } else {
                    filetype = g_strconcat(g_ascii_strup(++fileext, -1), " File", NULL);
                }
                // Get the type, and if it's one of the supported types, get the metadata for it.
                gboolean useTrack = (itemdata->filetype == LIBMTP_FILETYPE_MP3) ||
                        (itemdata->filetype == LIBMTP_FILETYPE_OGG) ||
                        (itemdata->filetype == LIBMTP_FILETYPE_FLAC) ||
                        (itemdata->filetype == LIBMTP_FILETYPE_WMA);
                // Ignore track metadata if we are treating all media as regular files.
                if(Preferences.allmediaasfiles){
                    useTrack = FALSE;
                }
                
                // Now if it's a track type file, eg OGG, WMA, MP3 or FLAC, get it's metadata.
                if (useTrack) {
                    LIBMTP_track_t *trackinfo;
                    trackinfo = LIBMTP_Get_Trackmetadata(DeviceMgr.device, itemdata->itemid);
                    if (trackinfo != NULL) {
                        trackduration = g_strdup_printf("%d:%.2d", (int) ((trackinfo->duration / 1000) / 60),
                                (int) ((trackinfo->duration / 1000) % 60));
                        if (trackinfo->tracknumber != 0) {
                            tracknumber = g_strdup_printf("%d", trackinfo->tracknumber);
                        } else {
                            tracknumber = g_strdup(" ");
                        }
                        // Some basic sanitisation.
                        if (trackinfo->title == NULL) trackinfo->title = g_strdup("");
                        if (trackinfo->artist == NULL) trackinfo->artist = g_strdup("");
                        if (trackinfo->album == NULL) trackinfo->album = g_strdup("");
                        if (trackinfo->date == NULL) trackinfo->date = g_strdup("");
                        if (trackinfo->genre == NULL) trackinfo->genre = g_strdup("");

                        // Icon
                        image = gdk_pixbuf_new_from_file(file_audio_png, NULL);

                        gtk_list_store_set(GTK_LIST_STORE(fileList), &rowIter,
                                //COL_FILENAME, tmpfile->filename,
                                COL_FILENAME_HIDDEN, itemdata->filename,
                                COL_FILENAME_ACTUAL, itemdata->filename,
                                COL_FILESIZE, filesize,
                                COL_FILEID, itemdata->itemid,
                                COL_ISFOLDER, FALSE,
                                COL_FILESIZE_HID, itemdata->filesize,
                                COL_TYPE, filetype,
                                COL_TRACK_NUMBER, tracknumber,
                                COL_TRACK_NUMBER_HIDDEN, trackinfo->tracknumber,
                                COL_TITLE, trackinfo->title,
                                COL_FL_ARTIST, trackinfo->artist,
                                COL_FL_ALBUM, trackinfo->album,
                                COL_YEAR, trackinfo->date,
                                COL_GENRE, trackinfo->genre,
                                COL_DURATION, trackduration,
                                COL_DURATION_HIDDEN, trackinfo->duration,
                                COL_ICON, image,
                                COL_LOCATION, itemdata->location,
                                -1);
                        g_free(trackduration);
                        g_free(tracknumber);
                        trackduration = NULL;
                        tracknumber = NULL;
                        // Indicate we are done with this image.
                        g_object_unref(image);
                        LIBMTP_destroy_track_t(trackinfo);
                    } else {
                        LIBMTP_Dump_Errorstack(DeviceMgr.device);
                        LIBMTP_Clear_Errorstack(DeviceMgr.device);
                    }
                } else {
                    // Determine the file type.
                    if (LIBMTP_FILETYPE_IS_AUDIO(itemdata->filetype)) {
                        image = gdk_pixbuf_new_from_file(file_audio_png, NULL);
                    } else if (LIBMTP_FILETYPE_IS_AUDIOVIDEO(itemdata->filetype)) {
                        image = gdk_pixbuf_new_from_file(file_video_png, NULL);
                    } else if (LIBMTP_FILETYPE_IS_VIDEO(itemdata->filetype)) {
                        image = gdk_pixbuf_new_from_file(file_video_png, NULL);
                    } else if (LIBMTP_FILETYPE_IS_IMAGE(itemdata->filetype)) {
                        image = gdk_pixbuf_new_from_file(file_image_png, NULL);
                    } else if (itemdata->filetype == LIBMTP_FILETYPE_ALBUM) {
                        image = gdk_pixbuf_new_from_file(file_album_png, NULL);
                    } else if (itemdata->filetype == LIBMTP_FILETYPE_PLAYLIST) {
                        image = gdk_pixbuf_new_from_file(file_playlist_png, NULL);
                    } else if (itemdata->filetype == LIBMTP_FILETYPE_TEXT) {
                        image = gdk_pixbuf_new_from_file(file_textfile_png, NULL);
                    } else if (itemdata->filetype == LIBMTP_FILETYPE_FOLDER) {
                        image = gdk_pixbuf_new_from_file(file_folder_png, NULL);
                    } else {
                        image = gdk_pixbuf_new_from_file(file_generic_png, NULL);
                    }

                    // let folders through IFF we are in alt connection mode.
                    if (!Preferences.use_alt_access_method && itemdata->filetype == LIBMTP_FILETYPE_FOLDER) {
                        goto skipAdd2;
                    }

                    // Set folder type.
                    //int isFolder = FALSE;
                    if (itemdata->filetype == LIBMTP_FILETYPE_FOLDER) {
                        //isFolder = TRUE;
                        filesize = g_strdup("");
                        filetype = g_strdup("");
                        filename_hid = g_strdup_printf("     < %s >", itemdata->filename);
                    } else {
                        filename_hid = g_strdup(itemdata->filename);
                    }

                    // Otherwise just show the file information
                    gtk_list_store_set(GTK_LIST_STORE(fileList), &rowIter,
                            //COL_FILENAME, tmpfile->filename,
                            COL_FILENAME_HIDDEN, itemdata->filename,
                            COL_FILENAME_ACTUAL, itemdata->filename,
                            COL_FILESIZE, filesize,
                            COL_FILEID, itemdata->itemid,
                            COL_ISFOLDER, FALSE,
                            COL_FILESIZE_HID, itemdata->filesize,
                            COL_TYPE, filetype,
                            COL_ICON, image,
                            COL_LOCATION, itemdata->location,
                            -1);
                    // Indicate we are done with this image.
                    g_free(filename_hid);
skipAdd2:
                    g_object_unref(image);
                }

                if (filetype != NULL)
                    g_free(filetype);
                filetype = NULL;

                if (filesize != NULL)
                    g_free(filesize);
                filesize = NULL;
            }
            tmpsearchList = tmpsearchList->next;
        }
        gchar *tmp_string;
        if (item_count != 1) {
            tmp_string = g_strdup_printf(_("Found %d items"), item_count);
        } else {
            tmp_string = g_strdup_printf(_("Found %d item"), item_count);
        }
        statusBarSet(tmp_string);
        g_free(tmp_string);
    }
    return TRUE;
}
