void on_albumtextbox_activate(GtkComboBox *combobox, gpointer user_data) {
    gint selected = gtk_combo_box_get_active(combobox);
    gint count = 0;
    LIBMTP_album_t *albumlist = LIBMTP_Get_Album_List_For_Storage(DeviceMgr.device, DeviceMgr.devicestorage->id);
    LIBMTP_album_t *albuminfo = albumlist;

    while (albuminfo != NULL) {
        if (count == selected) {
            AlbumArtUpdateImage(albuminfo);
            clearAlbumStruc(albumlist);
            return;
        }
        // Text the album_entry
        albuminfo = albuminfo->next;
        count++;
    }
    // Set a default image
    AlbumArtUpdateImage(NULL);
    clearAlbumStruc(albumlist);
}
