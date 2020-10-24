static gboolean
can_edit_source (const char *source)
{
	GConfEngine *engine;
	GConfClient *client;
	GConfEntry  *entry;
	GError      *error;
	gboolean     retval;

	if (!(engine = gconf_engine_get_for_address (source, NULL)))
		return FALSE;

	error = NULL;
	client = gconf_client_get_for_engine (engine);
	entry = gconf_client_get_entry (client,
					"/apps/gconf-editor/can_edit_source",
					NULL,
					FALSE,
					&error);
	if (error != NULL) {
		g_assert (entry == NULL);
		g_error_free (error);
		g_object_unref (client);
		gconf_engine_unref (engine);
		return FALSE;
	}

	g_assert (entry != NULL);

	retval = gconf_entry_get_is_writable (entry);

	gconf_entry_unref (entry);
	g_object_unref (client);
	gconf_engine_unref (engine);

	return retval;
}