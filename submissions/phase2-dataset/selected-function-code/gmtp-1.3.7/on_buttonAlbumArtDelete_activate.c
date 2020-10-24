void on_buttonAlbumArtDelete_activate(GtkWidget *button, gpointer user_data) {

    // Send a blank representation.
    gint selected = gtk_combo_box_get_active(GTK_COMBO_BOX(textboxAlbumArt));
    gint count = 0;
    LIBMTP_album_t *albumlist = LIBMTP_Get_Album_List_For_Storage(DeviceMgr.device, DeviceMgr.devicestorage->id);
    LIBMTP_album_t *albuminfo = albumlist;

    while (albuminfo != NULL) {
        if (count == selected) {
            // Found our album, so update the image on the device, then update the display.
            albumDeleteArt(albuminfo->album_id);
            AlbumArtUpdateImage(NULL);
            clearAlbumStruc(albumlist);
            return;
        }
        // Next album_entry
        albuminfo = albuminfo->next;
        count++;
    }
    // Set a default image as we didn't find our album.
    AlbumArtUpdateImage(NULL);
    clearAlbumStruc(albumlist);

} // end on_buttonAlbumArtDelete_activate()
