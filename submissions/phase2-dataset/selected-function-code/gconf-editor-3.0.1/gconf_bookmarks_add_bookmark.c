void
gconf_bookmarks_add_bookmark (const char *path)
{
	GSList *list, *tmp;
	GConfClient *client;

	client = gconf_client_get_default ();

	/* Get the old list and then set it */
	list = gconf_client_get_list (client,
				     "/apps/gconf-editor/bookmarks", GCONF_VALUE_STRING, NULL);

	/* FIXME: We need error handling here, also this function leaks memory */

	/* Check that the bookmark hasn't been added already */
	for (tmp = list; tmp; tmp = tmp->next) {
		if (strcmp (tmp->data, path) == 0) {
			g_slist_free (list);
			return;
		}
	}

	/* Append the new bookmark */
	list = g_slist_append (list, g_strdup (path));
	
	gconf_client_set_list (client,
			       "/apps/gconf-editor/bookmarks", GCONF_VALUE_STRING, list, NULL);
	g_slist_free (list);
	g_object_unref (client);
}