    LIBMTP_filesampledata_t *imagedata = NULL;
    GdkPixbufLoader *BufferLoader = NULL;
    GdkPixbuf *gdk_image = NULL;
    GdkPixbuf *gdk_image_scale = NULL;

    // Ensure our widget exists.
    if (AlbumArtImage == NULL)
        return;
    // Ensure we have a selected album.
    if (selectedAlbum == NULL) {
        AlbumArtSetDefault();
        return;
    }
    imagedata = albumGetArt(selectedAlbum);
    if (imagedata != NULL) {
        if (imagedata->size != 0) {
            // We have our image data.
            // Create a GdkPixbuf
            BufferLoader = gdk_pixbuf_loader_new();
            if (gdk_pixbuf_loader_write(BufferLoader, (const guchar*) imagedata->data, imagedata->size, NULL) == TRUE) {
                // Set the GtkImage to use that GdkPixbuf.
                gdk_image = gdk_pixbuf_loader_get_pixbuf(BufferLoader);
                gdk_pixbuf_loader_close(BufferLoader, NULL);
                gdk_image_scale = gdk_pixbuf_scale_simple(gdk_image, ALBUM_SIZE, ALBUM_SIZE, GDK_INTERP_BILINEAR);
                gtk_image_set_from_pixbuf(GTK_IMAGE(AlbumArtImage), gdk_image_scale);
                g_object_unref(gdk_image);
                g_object_unref(gdk_image_scale);

                // Set button states, so we can do stuff on the image.
                gtk_widget_set_sensitive(GTK_WIDGET(buttonAlbumDownload), TRUE);
                gtk_widget_set_sensitive(GTK_WIDGET(buttonAlbumDelete), TRUE);
            } else {
                AlbumArtSetDefault();
            }
        } else {
            AlbumArtSetDefault();
        }
        // Clean up the image buffer.
        LIBMTP_destroy_filesampledata_t(imagedata);
    } else {
        AlbumArtSetDefault();
    }
}
