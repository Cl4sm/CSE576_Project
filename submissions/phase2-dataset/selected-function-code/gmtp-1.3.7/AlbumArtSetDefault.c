    //gtk_image_set_from_stock(GTK_IMAGE(AlbumArtImage), GTK_STOCK_MISSING_IMAGE, GTK_ICON_SIZE_DIALOG );
    GdkPixbuf *gdk_image = NULL;
    GdkPixbuf *gdk_image_scale = NULL;
#if HAVE_GTK3 == 0
    gdk_image = gtk_widget_render_icon(GTK_WIDGET(AlbumArtImage), GTK_STOCK_MISSING_IMAGE,
            GTK_ICON_SIZE_DIALOG, "gtk-missing-image");
#else 
    GtkIconTheme *theme = gtk_icon_theme_get_default();
    gdk_image = gtk_icon_theme_load_icon(theme, "image-missing", ALBUM_SIZE, 0, NULL);
#endif
    gdk_image_scale = gdk_pixbuf_scale_simple(gdk_image, ALBUM_SIZE, ALBUM_SIZE, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(AlbumArtImage), gdk_image_scale);
    g_object_unref(gdk_image);
    g_object_unref(gdk_image_scale);

    // Disable the buttons, since we have a default image.
    gtk_widget_set_sensitive(GTK_WIDGET(buttonAlbumDownload), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(buttonAlbumDelete), FALSE);
}
