void on_buttonAlbumArtAdd_activate(GtkWidget *button, gpointer user_data) {
    // What we do here is display a find folder dialog, and save the resulting folder into the text wigdet and preferences item.
    //gchar *filename;
    gchar *filename = NULL;
    GtkWidget *FileDialog;
    FileDialog = gtk_file_chooser_dialog_new(_("Select Album Art File"),
            GTK_WINDOW(AlbumArtDialog), GTK_FILE_CHOOSER_ACTION_OPEN,
            _("_Cancel"), GTK_RESPONSE_CANCEL,
            _("_Open"), GTK_RESPONSE_ACCEPT,
            NULL);

    gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(FileDialog), TRUE);

    // Set the default path to be the normal upload folder.
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(FileDialog), Preferences.fileSystemUploadPath->str);

    if (gtk_dialog_run(GTK_DIALOG(FileDialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(FileDialog));

        if (filename != NULL) {
            // Upload the file to the selected album.
            gint selected = gtk_combo_box_get_active(GTK_COMBO_BOX(textboxAlbumArt));
            gint count = 0;
            LIBMTP_album_t *albumlist = LIBMTP_Get_Album_List_For_Storage(DeviceMgr.device, DeviceMgr.devicestorage->id);
            LIBMTP_album_t *albuminfo = albumlist;

            while (albuminfo != NULL) {
                if (count == selected) {
                    // Found our album, so update the image on the device, then update the display.
                    albumAddArt(albuminfo->album_id, filename);
                    AlbumArtUpdateImage(albuminfo);
                    clearAlbumStruc(albumlist);
                    g_free(filename);
                    gtk_widget_destroy(FileDialog);
                    return;
                }
                // Next album_entry
                albuminfo = albuminfo->next;
                count++;
            }
            // Set a default image as we didn't find our album.
            AlbumArtUpdateImage(NULL);
            clearAlbumStruc(albumlist);

            g_free(filename);
        }
    }
    gtk_widget_destroy(FileDialog);
} // end on_buttonAlbumArtAdd_activate()
