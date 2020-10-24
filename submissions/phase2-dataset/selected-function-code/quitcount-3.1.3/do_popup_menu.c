static void do_popup_menu(GtkStatusIcon *i, guint button, guint activate_time, 
			  gpointer user_data)
{
	GtkWidget *menu = gtk_menu_new();
	GtkWidget *item;
	GtkWidget *img;
	gchar *path = g_strconcat(ICONSDIR, G_DIR_SEPARATOR_S, PACKAGE, ".svg", NULL);
	gint w,h;
	GdkPixbuf *pixbuf;
	GtkApplication *app = GTK_APPLICATION(user_data);
	
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES, NULL);

	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item), "activate", 
			 G_CALLBACK(show_config), NULL);

	if (!gtk_icon_size_lookup(GTK_ICON_SIZE_MENU, &w, &h)) {
		w = 16;
		h = 16;
	}

	pixbuf = gdk_pixbuf_new_from_file_at_size(path, w, h, NULL);
	g_free(path);	
	img = gtk_image_new_from_pixbuf(pixbuf);
	g_object_unref(G_OBJECT(pixbuf));

	item = gtk_image_menu_item_new_with_label(_("Statistics"));
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item), img);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item), "activate", 
			 G_CALLBACK(do_activate), NULL);

	item = gtk_separator_menu_item_new();
	gtk_widget_show(item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item), "activate", 
			 G_CALLBACK(do_about), NULL);
	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item), "activate", 
			 G_CALLBACK(do_quit), app);
	gtk_menu_popup(GTK_MENU(menu), NULL, NULL, gtk_status_icon_position_menu, i,
			button, activate_time);
}