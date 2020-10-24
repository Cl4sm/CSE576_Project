GtkTreePath *
gconf_tree_model_get_tree_path_from_gconf_path (GConfTreeModel *tree_model, const char *key)
{
	GtkTreePath *path = NULL;
	GtkTreeIter iter, child_iter, found_iter;
	gchar *tmp_str;
	gchar **key_array;
	int i;
	gboolean found;

	g_assert (key[0] == '/');

	/* special case root node */
	if (strlen (key) == 1 && key[0] == '/')
		return gtk_tree_path_new_from_string ("0");

	key_array = g_strsplit (key + 1, "/", 0);

	if (!gtk_tree_model_get_iter_first (GTK_TREE_MODEL (tree_model), &iter)) {

		/* Ugh, something is terribly wrong */
		g_strfreev (key_array);
		return NULL;
	}

	for (i = 0; key_array[i] != NULL; i++) {
		/* FIXME: this will build the level if it isn't there. But,
		 * the level can also be there, possibly incomplete. This
		 * code isn't handling those incomplete levels yet (that
		 * needs some current level/gconf directory comparing code)
		 */
		if (!gtk_tree_model_iter_children (GTK_TREE_MODEL (tree_model), &child_iter, &iter))
			break;

		found = FALSE;
		do {
			gtk_tree_model_get (GTK_TREE_MODEL (tree_model), &child_iter,
					    GCONF_TREE_MODEL_NAME_COLUMN, &tmp_str,
					    -1);

			if (strcmp (tmp_str, key_array[i]) == 0) {
				found_iter = child_iter;
				found = TRUE;
				g_free (tmp_str);
				break;
			} else
				g_free (tmp_str);
		} while (gtk_tree_model_iter_next (GTK_TREE_MODEL (tree_model), &child_iter));

		if (!found)
			break;
		iter = child_iter;
	}

	path = gtk_tree_model_get_path (GTK_TREE_MODEL (tree_model), &found_iter);

	g_strfreev (key_array);

	return path;
}