gboolean
gsd_housekeeping_manager_start (GsdHousekeepingManager *manager,
                                GError                **error)
{
        gchar *dir;

        g_debug ("Starting housekeeping manager");
        gnome_settings_profile_start (NULL);

        /* Create ~/.local/ as early as possible */
        g_mkdir_with_parents(g_get_user_data_dir (), 0700);

        /* Create ~/.local/share/applications/, see
         * https://bugzilla.gnome.org/show_bug.cgi?id=703048 */
        dir = g_build_filename (g_get_user_data_dir (), "applications", NULL);
        g_mkdir (dir, 0700);
        g_free (dir);

        gsd_ldsm_setup (FALSE);

        manager->priv->settings = g_settings_new (THUMB_PREFIX);
        g_signal_connect (G_OBJECT (manager->priv->settings), "changed",
                          G_CALLBACK (settings_changed_callback), manager);

        /* Clean once, a few minutes after start-up */
        do_cleanup_soon (manager);

        /* Clean periodically, on a daily basis. */
        manager->priv->long_term_cb = g_timeout_add_seconds (INTERVAL_ONCE_A_DAY,
                                      (GSourceFunc) do_cleanup,
                                      manager);
        g_source_set_name_by_id (manager->priv->long_term_cb, "[gnome-settings-daemon] do_cleanup");

        gnome_settings_profile_end (NULL);

        return TRUE;
}