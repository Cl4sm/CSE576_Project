gboolean
gsd_print_notifications_manager_start (GsdPrintNotificationsManager *manager,
                                       GError                      **error)
{
        g_debug ("Starting print-notifications manager");

        gnome_settings_profile_start (NULL);

        manager->priv->subscription_id = -1;
        manager->priv->dests = NULL;
        manager->priv->num_dests = 0;
        manager->priv->scp_handler_spawned = FALSE;
        manager->priv->timeouts = NULL;
        manager->priv->printing_printers = NULL;
        manager->priv->active_notifications = NULL;
        manager->priv->cups_bus_connection = NULL;
        manager->priv->cups_connection_timeout_id = 0;
        manager->priv->last_notify_sequence_number = -1;

        manager->priv->start_idle_id = g_idle_add (gsd_print_notifications_manager_start_idle, manager);
        g_source_set_name_by_id (manager->priv->start_idle_id, "[gnome-settings-daemon] gsd_print_notifications_manager_start_idle");

        gnome_settings_profile_end (NULL);

        return TRUE;
}