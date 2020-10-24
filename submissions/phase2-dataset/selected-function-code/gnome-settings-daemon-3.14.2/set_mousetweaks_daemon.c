static void
set_mousetweaks_daemon (GsdMouseManager *manager,
                        gboolean         dwell_click_enabled,
                        gboolean         secondary_click_enabled)
{
        GError *error = NULL;
        gchar *comm;
        gboolean run_daemon = dwell_click_enabled || secondary_click_enabled;

        if (run_daemon || manager->priv->mousetweaks_daemon_running)
                comm = g_strdup_printf ("mousetweaks %s",
                                        run_daemon ? "" : "-s");
        else
                return;

        if (run_daemon)
                manager->priv->mousetweaks_daemon_running = TRUE;

        if (! g_spawn_command_line_async (comm, &error)) {
                if (error->code == G_SPAWN_ERROR_NOENT && run_daemon) {
                        if (dwell_click_enabled) {
                                g_settings_set_boolean (manager->priv->mouse_a11y_settings,
                                                        KEY_DWELL_CLICK_ENABLED, FALSE);
                        } else if (secondary_click_enabled) {
                                g_settings_set_boolean (manager->priv->mouse_a11y_settings,
                                                        KEY_SECONDARY_CLICK_ENABLED, FALSE);
                        }

                        g_warning ("Error enabling mouse accessibility features (mousetweaks is not installed)");
                }
                g_error_free (error);
        }
        g_free (comm);
}