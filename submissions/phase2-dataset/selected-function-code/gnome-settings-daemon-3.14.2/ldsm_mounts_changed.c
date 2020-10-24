static void
ldsm_mounts_changed (GObject  *monitor,
                     gpointer  data)
{
        GList *mounts;

        /* remove the saved data for mounts that got removed */
        mounts = g_unix_mounts_get (time_read);
        g_hash_table_foreach_remove (ldsm_notified_hash,
                                     ldsm_is_hash_item_not_in_mounts, mounts);
        g_list_free_full (mounts, (GDestroyNotify) g_unix_mount_free);

        /* check the status now, for the new mounts */
        ldsm_check_all_mounts (NULL);

        /* and reset the timeout */
        if (ldsm_timeout_id)
                g_source_remove (ldsm_timeout_id);
        ldsm_timeout_id = g_timeout_add_seconds (CHECK_EVERY_X_SECONDS,
                                                 ldsm_check_all_mounts, NULL);
        g_source_set_name_by_id (ldsm_timeout_id, "[gnome-settings-daemon] ldsm_check_all_mounts");
}