int write_config(void)
{
	gchar *path = g_strconcat(
		g_get_user_config_dir(),
		G_DIR_SEPARATOR_S, PACKAGE, NULL);
	gchar *file = g_strconcat(
		path,
		G_DIR_SEPARATOR_S, "config", NULL);
	gchar *data;
	gchar price_s[256], nicotine_s[256], tar_s[256];
	
	if (!g_file_test(path, G_FILE_TEST_IS_DIR)) {
		if (g_mkdir_with_parents(path, 0755) < 0) {
			g_free(path);
			g_free(file);
			return FALSE;
		}
	}
	g_free(path);
	
	while(strstr(currency, "="))
		*(strstr(currency, "=")) = '-';
		
	g_ascii_dtostr(price_s, 255, price_per_pack);
	g_ascii_dtostr(nicotine_s, 255, nicotine_per_cig);
	g_ascii_dtostr(tar_s, 255, tar_per_cig);
	data = g_strdup_printf(
		"stop_date=%ld\n"
		"num_per_day=%d\n"
		"price_per_pack=%s\n"
		"nicotine_per_cig=%s\n"
		"tar_per_cig=%s\n"
		"pack_size=%d\n"
		"currency=%s\n",
			stop_date,
			num_per_day,
			price_s,
			nicotine_s,
			tar_s,
			pack_size,
			currency?currency:"€");
	
	if (g_file_set_contents(file, data, -1, NULL) == FALSE) {
		printf("can't write %s\n", file);
		g_free(file);
		g_free(data);
		return FALSE;
	}
	g_free(file);
	g_free(data);

	return TRUE;
}