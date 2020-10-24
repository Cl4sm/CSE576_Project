gint
main (gint argc, gchar **argv)
{
	GOptionContext *context;
	GtkWidget *window;
	GError *error = NULL;

	static gchar **remaining_args = NULL;
	gchar *initial_key = NULL;
		
	const GOptionEntry entries[] = 
	{
	  { G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_STRING_ARRAY, &remaining_args, NULL, N_("[KEY]") },
	  { NULL }
	};

	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	context = g_option_context_new (N_("- Directly edit your entire configuration database"));

	g_option_context_set_translation_domain (context, GETTEXT_PACKAGE);
	g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);
	g_option_context_add_group (context, gtk_get_option_group (TRUE));

	if (!g_option_context_parse (context, &argc, &argv, &error)) {
		g_critical ("Failed to parse arguments: %s", error->message);
		g_error_free (error);
		g_option_context_free (context);
		exit (1);
	}

	g_option_context_free (context);

	/* Register our stock icons */
        gtk_icon_theme_append_search_path (gtk_icon_theme_get_default (), ICONDIR);
	gconf_stock_icons_register ();
	load_accel_map ();

        gtk_window_set_default_icon_name ("gconf-editor");

	window = gconf_editor_application_create_editor_window (GCONF_EDITOR_WINDOW_TYPE_NORMAL);
	gtk_widget_show_now (window);

	/* get the key specified on the command line if any. Ignore the rest */
	initial_key = remaining_args != NULL ? remaining_args[0] : NULL;

	if (initial_key != NULL)
		gconf_editor_window_go_to (GCONF_EDITOR_WINDOW (window),initial_key);
	
	gtk_main ();

	save_accel_map ();
	g_strfreev (remaining_args);

	return 0;
}