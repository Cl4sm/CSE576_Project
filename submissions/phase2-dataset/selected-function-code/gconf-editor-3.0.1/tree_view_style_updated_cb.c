static void
tree_view_style_updated_cb (GtkWidget *widget,
                            GObject *cell)
{
        GdkPixbuf *folder_open = NULL, *folder_closed = NULL;
        int w, h;

        if (gtk_icon_size_lookup_for_settings (gtk_widget_get_settings (widget),
                                               GTK_ICON_SIZE_MENU,
                                               &w, &h)) {
                GtkIconTheme *theme;

                theme = gtk_icon_theme_get_for_screen (gtk_widget_get_screen (widget));
                
                folder_open = gtk_icon_theme_load_icon (theme, "folder-open", w, GTK_ICON_LOOKUP_GENERIC_FALLBACK, NULL);
                folder_closed = gtk_icon_theme_load_icon (theme, "folder", w, GTK_ICON_LOOKUP_GENERIC_FALLBACK, NULL);
        }

        g_object_set (cell,
                      "pixbuf-expander-open", folder_open,
                      "pixbuf-expander-closed", folder_closed,
                      "pixbuf", folder_closed,
                      NULL);
        if (folder_open) {
                g_object_unref (folder_open);
        }
        if (folder_closed) {
                g_object_unref (folder_closed);
        }
}