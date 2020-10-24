{
	GtkTreeIter iter;
	char *c = line->backtrace;
	char *c2;
	char buffer[1024];

	while (c && *c) {
		while (*c == ' ')
			c++;
		if (!*c)
			break;
		c2 = strchr(c, ' ');
		if (c2) {
			int l = MIN(c2-c, 1023);
			strncpy(buffer, c, l);
			buffer[l] = 0;
		} else {
			strncpy(buffer, c, 1023);
			buffer[1023] = 0;
		}
		c = c2;
		gtk_list_store_append(GTK_LIST_STORE(backtrace_model), &iter);
		gtk_list_store_set(GTK_LIST_STORE(backtrace_model),
				   &iter,
				   COL_B_FUNC, buffer,
				   -1);
	}
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(backtrace_view));
}
