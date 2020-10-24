    FILE* fd;
    gint selected = gtk_combo_box_get_active(GTK_COMBO_BOX(textboxAlbumArt));
    gint count = 0;
    GtkWidget *FileDialog = NULL;
    gchar *filename = NULL;
    LIBMTP_filesampledata_t *imagedata = NULL;
    LIBMTP_album_t *albumlist = LIBMTP_Get_Album_List_For_Storage(DeviceMgr.device, DeviceMgr.devicestorage->id);
    LIBMTP_album_t *albuminfo = albumlist;

    // Scan our albums, looking for the correct one.
    while (albuminfo != NULL) {
        if (count == selected) {
            // Found our album, let's get our data..
            imagedata = albumGetArt(albuminfo);
            if (imagedata != NULL) {

                FileDialog = gtk_file_chooser_dialog_new(_("Save Album Art File"),
                        GTK_WINDOW(AlbumArtDialog), GTK_FILE_CHOOSER_ACTION_SAVE,
                        _("_Cancel"), GTK_RESPONSE_CANCEL,
                        _("_Save"), GTK_RESPONSE_ACCEPT,
                        NULL);

                gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(FileDialog), TRUE);

                // Set the default path to be the normal download folder.
                gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(FileDialog),
                        Preferences.fileSystemDownloadPath->str);

                // Set a default name to be the album.JPG
                gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(FileDialog),
                        g_strdup_printf("%s.jpg", albuminfo->name));

                if (gtk_dialog_run(GTK_DIALOG(FileDialog)) == GTK_RESPONSE_ACCEPT) {
                    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(FileDialog));
                    if (filename != NULL) {
                        // The user has selected a file to save as, so do the deed.
                        fd = fopen(filename, "w");
                        if (fd == NULL) {
                            g_fprintf(stderr, _("Couldn't save image file %s\n"), filename);
                            displayError(_("Couldn't save image file\n"));
                        } else {
                            fwrite(imagedata->data, imagedata->size, 1, fd);
                            fclose(fd);
                        }
                        g_free(filename);
                    }
                }
                // Clean up our image data and dialog.
                LIBMTP_destroy_filesampledata_t(imagedata);
                gtk_widget_destroy(FileDialog);
            }
            clearAlbumStruc(albumlist);
            return;
        }
        // Next album_entry
        albuminfo = albuminfo->next;
        count++;
    }
    // Set a default image as we didn't find our album.
    clearAlbumStruc(albumlist);
    gtk_widget_destroy(FileDialog);
} // end on_buttonAlbumArtDownload_activate()
