static void
gsd_ldsm_get_config (void)
{
        gchar **settings_list;

        free_percent_notify = g_settings_get_double (settings, SETTINGS_FREE_PC_NOTIFY_KEY);
        free_percent_notify_again = g_settings_get_double (settings, SETTINGS_FREE_PC_NOTIFY_AGAIN_KEY);

        free_size_gb_no_notify = g_settings_get_int (settings, SETTINGS_FREE_SIZE_NO_NOTIFY);
        min_notify_period = g_settings_get_int (settings, SETTINGS_MIN_NOTIFY_PERIOD);

        if (ignore_paths != NULL) {
                g_slist_foreach (ignore_paths, (GFunc) g_free, NULL);
                g_clear_pointer (&ignore_paths, g_slist_free);
        }

        settings_list = g_settings_get_strv (settings, SETTINGS_IGNORE_PATHS);
        if (settings_list != NULL) {
                guint i;

                for (i = 0; settings_list[i] != NULL; i++)
                        ignore_paths = g_slist_prepend (ignore_paths, g_strdup (settings_list[i]));

                /* Make sure we dont leave stale entries in ldsm_notified_hash */
                g_hash_table_foreach_remove (ldsm_notified_hash,
                                             ldsm_is_hash_item_in_ignore_paths, NULL);

                g_strfreev (settings_list);
        }

        purge_trash = g_settings_get_boolean (privacy_settings, SETTINGS_PURGE_TRASH);
        purge_temp_files = g_settings_get_boolean (privacy_settings, SETTINGS_PURGE_TEMP_FILES);
        purge_after = g_settings_get_uint (privacy_settings, SETTINGS_PURGE_AFTER);
}