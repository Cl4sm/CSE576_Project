void
gsd_ldsm_clean (void)
{
        if (purge_trash_id)
                g_source_remove (purge_trash_id);
        purge_trash_id = 0;

        if (purge_temp_id)
                g_source_remove (purge_temp_id);
        purge_temp_id = 0;

        if (ldsm_timeout_id)
                g_source_remove (ldsm_timeout_id);
        ldsm_timeout_id = 0;

        g_clear_pointer (&ldsm_notified_hash, g_hash_table_destroy);
        g_clear_object (&ldsm_monitor);
        g_clear_object (&settings);
        g_clear_object (&privacy_settings);
        g_clear_object (&dialog);
        g_clear_pointer (&notification, notify_notification_close);
        g_slist_free_full (ignore_paths, g_free);
        ignore_paths = NULL;
}