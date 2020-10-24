static void
gconf_editor_window_about_window (GtkAction *action, GtkWidget *callback_data)
{
	const gchar *authors[] = {
		"Anders Carlsson <andersca@gnome.org>",
		"Fernando Herrera <fherrera@onirica.com>",
		NULL
	};
	const gchar *documenters[] = {
		"Sun Microsystems",
		"Shaun McCance <shaunm@gnome.org>",
		NULL
	};

	gtk_show_about_dialog (GTK_WINDOW (callback_data),
#if GTK_CHECK_VERSION (2, 11, 0)
                              "program-name", _("Configuration Editor"),
#else
                              "name", _("Configuration Editor"),
#endif
			      "version", VERSION,
			      "copyright", "Copyright \xc2\xa9 2001-2004 Anders Carlsson",
			      "comments", _("An editor for the GConf configuration system."),
			      "authors", authors,
			      "documenters", documenters,
			      "translator-credits", _("translator-credits"),
			      "logo-icon-name", "gconf-editor",
			      NULL);
}	