static GtkWidget *wrap(GtkWidget *inner, gchar *label)
{
	GtkWidget *grid = gtk_grid_new();
	GtkWidget *i;
	GtkWidget *title;
	GdkPixbuf *pixbuf;
	gchar *path = g_strconcat(ICONSDIR, G_DIR_SEPARATOR_S, PACKAGE, ".svg", NULL);

	pixbuf = gdk_pixbuf_new_from_file_at_size(path, 48, 48, NULL);
	i = gtk_image_new_from_pixbuf(pixbuf);
	g_object_unref(G_OBJECT(pixbuf));
	g_free(path);	
	title = gtk_label_new(label);
	gtk_label_set_use_markup(GTK_LABEL(title), TRUE);
	gtk_misc_set_alignment(GTK_MISC(title), 0, 0.5);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
	gtk_grid_attach(GTK_GRID(grid), i, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), title, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), 0, 1 ,2, 1);
	gtk_grid_attach(GTK_GRID(grid), inner, 0, 2, 2, 1);
	gtk_widget_set_hexpand (grid, TRUE);
	gtk_widget_set_hexpand (title, TRUE);
	gtk_widget_set_halign (inner, GTK_ALIGN_CENTER);
	gtk_widget_set_margin_left(i, 10);
	gtk_widget_show_all(grid);
	return grid;
}