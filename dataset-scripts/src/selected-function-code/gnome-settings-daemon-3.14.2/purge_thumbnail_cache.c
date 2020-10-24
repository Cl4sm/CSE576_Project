static void
purge_thumbnail_cache (GsdHousekeepingManager *manager)
{

        char     **paths;
        GList     *files;
        PurgeData  purge_data;
        GTimeVal   current_time;
        guint      i;

        g_debug ("housekeeping: checking thumbnail cache size and freshness");

        purge_data.max_age = g_settings_get_int (manager->priv->settings, THUMB_AGE_KEY) * 24 * 60 * 60;
        purge_data.max_size = g_settings_get_int (manager->priv->settings, THUMB_SIZE_KEY) * 1024 * 1024;

        /* if both are set to -1, we don't need to read anything */
        if ((purge_data.max_age < 0) && (purge_data.max_size < 0))
                return;

        paths = get_thumbnail_dirs ();
        files = NULL;
        for (i = 0; paths[i] != NULL; i++)
                files = read_dir_for_purge (paths[i], files);
        g_strfreev (paths);

        g_get_current_time (&current_time);

        purge_data.now = current_time.tv_sec;
        purge_data.total_size = 0;

        if (purge_data.max_age >= 0)
                g_list_foreach (files, (GFunc) purge_old_thumbnails, &purge_data);

        if ((purge_data.total_size > purge_data.max_size) && (purge_data.max_size >= 0)) {
                GList *scan;
                files = g_list_sort (files, (GCompareFunc) sort_file_mtime);
                for (scan = files; scan && (purge_data.total_size > purge_data.max_size); scan = scan->next) {
                        ThumbData *info = scan->data;
                        g_unlink (info->path);
                        purge_data.total_size -= info->size;
                }
        }

        g_list_foreach (files, (GFunc) thumb_data_free, NULL);
        g_list_free (files);
}