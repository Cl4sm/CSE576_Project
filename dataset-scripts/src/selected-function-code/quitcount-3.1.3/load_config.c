gboolean load_config(void)
{
	gchar *path = g_strconcat(
		g_get_user_config_dir(),
		G_DIR_SEPARATOR_S, PACKAGE, NULL);
	gchar *file = g_strconcat(
		path,
		G_DIR_SEPARATOR_S, "config", NULL);
	gchar *data;
	gchar **vars, **cur_var;

	if (!g_file_test(path, G_FILE_TEST_IS_DIR)) {
		if (g_mkdir_with_parents(path, 0755) < 0) {
			g_free(path);
			g_free(file);
			return FALSE;
		}
	}
	g_free(path);
	
	if (g_file_get_contents(file, &data, NULL, NULL) == FALSE) {
		printf("can't read %s\n", file);
		g_free(file);
		return FALSE;
	}
	g_free(file);
	
	vars = g_strsplit_set(data, "\n=", -1);
	g_free(data);

	cur_var = vars;
	while (cur_var && *cur_var && *(cur_var+1)) {
		if (!strcmp(*cur_var, "stop_date"))
			stop_date = (time_t)atoi(*(++cur_var));
		else if (!strcmp(*cur_var, "num_per_day"))
			num_per_day = atoi(*(++cur_var));
		else if (!strcmp(*cur_var, "price_per_pack"))
			price_per_pack = g_ascii_strtod(*(++cur_var), NULL);
		else if (!strcmp(*cur_var, "nicotine_per_cig"))
			nicotine_per_cig = g_ascii_strtod(*(++cur_var), NULL);
		else if (!strcmp(*cur_var, "tar_per_cig"))
			tar_per_cig = g_ascii_strtod(*(++cur_var), NULL);
		else if (!strcmp(*cur_var, "pack_size"))
			pack_size = atoi(*(++cur_var));
		else if (!strcmp(*cur_var, "currency"))
			currency = g_strdup(*(++cur_var));

		cur_var++;
	}
	g_strfreev(vars);

	if (pack_size <= 0)
		pack_size = 1;
	unit_price = price_per_pack/pack_size;

	return (stop_date != (time_t)0);
}