static void
gconf_tree_model_get_value (GtkTreeModel *tree_model, GtkTreeIter *iter, gint column, GValue *value)
{
	Node *node = iter->user_data;

	switch (column) {
	case GCONF_TREE_MODEL_NAME_COLUMN:
		g_value_init (value, G_TYPE_STRING);

		if (node == NULL || node->parent == NULL)
			g_value_set_string (value, "/");
		else {
			gchar *ptr;

			ptr = node->path + strlen (node->path);

			while (ptr[-1] != '/')
				ptr--;

			g_value_set_string (value, ptr);
		}
		break;
	default:
		break;
	}
}