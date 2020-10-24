void
gconf_stock_icons_register (void)
{
	GtkIconFactory *icon_factory;
	GtkIconSet *set;
	GtkIconSource *icon_source;
	int i;
	static gboolean initialized = FALSE;

	if (initialized == TRUE) {
		return;
	}

	icon_factory = gtk_icon_factory_new ();
	gtk_icon_factory_add_default (icon_factory);

	for (i = 0; i < (int) G_N_ELEMENTS (icon_theme_items); i++)
	{
		set = gtk_icon_set_new ();
		icon_source = gtk_icon_source_new ();
		gtk_icon_source_set_icon_name (icon_source, icon_theme_items[i]);
		gtk_icon_set_add_source (set, icon_source);
		gtk_icon_factory_add (icon_factory, icon_theme_items[i], set);
		gtk_icon_set_unref (set);
		gtk_icon_source_free (icon_source);
	}

	g_object_unref (G_OBJECT (icon_factory));

	initialized = TRUE;
}