void
gsd_xrandr_manager_stop (GsdXrandrManager *manager)
{
        g_debug ("Stopping xrandr manager");

        manager->priv->running = FALSE;

        if (manager->priv->bus_cancellable != NULL) {
                g_cancellable_cancel (manager->priv->bus_cancellable);
                g_object_unref (manager->priv->bus_cancellable);
                manager->priv->bus_cancellable = NULL;
        }

        if (manager->priv->settings != NULL) {
                g_object_unref (manager->priv->settings);
                manager->priv->settings = NULL;
        }

        if (manager->priv->rw_screen != NULL) {
                g_object_unref (manager->priv->rw_screen);
                manager->priv->rw_screen = NULL;
        }

        if (manager->priv->upower_client != NULL) {
                g_signal_handlers_disconnect_by_data (manager->priv->upower_client, manager);
                g_object_unref (manager->priv->upower_client);
                manager->priv->upower_client = NULL;
        }

        if (manager->priv->name_id != 0)
                g_bus_unown_name (manager->priv->name_id);

        if (manager->priv->introspection_data) {
                g_dbus_node_info_unref (manager->priv->introspection_data);
                manager->priv->introspection_data = NULL;
        }

        if (manager->priv->connection != NULL) {
                g_object_unref (manager->priv->connection);
                manager->priv->connection = NULL;
        }

        if (manager->priv->device_manager != NULL) {
                g_signal_handler_disconnect (manager->priv->device_manager,
                                             manager->priv->device_added_id);
                g_signal_handler_disconnect (manager->priv->device_manager,
                                             manager->priv->device_removed_id);
                manager->priv->device_manager = NULL;
        }

        log_open ();
        log_msg ("STOPPING XRANDR PLUGIN\n------------------------------------------------------------\n");
        log_close ();
}