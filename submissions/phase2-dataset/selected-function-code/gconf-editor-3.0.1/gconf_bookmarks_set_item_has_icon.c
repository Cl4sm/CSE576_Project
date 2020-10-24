static void
gconf_bookmarks_set_item_has_icon (GtkWidget *item,
				   gboolean   have_icons)
{
	GtkWidget *image;

	image = gtk_image_menu_item_get_image (GTK_IMAGE_MENU_ITEM (item));
	if (image && !g_object_get_data (G_OBJECT (item), "gconf-editor-icon"))
		g_object_set_data_full (G_OBJECT (item), "gconf-editor-icon",
					g_object_ref (image), g_object_unref);

	if (!image)
		image = g_object_get_data (G_OBJECT (item), "gconf-editor-icon");

	if (!image)
		return;

	if (have_icons)
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (item), image);
	else
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (item), NULL);
}