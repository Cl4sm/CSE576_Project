static void
update_accelerometer (GsdOrientationManager *manager)
{
        GUdevDevice *dev;

        g_clear_pointer (&manager->priv->sysfs_path, g_free);
        manager->priv->prev_orientation = ORIENTATION_UNDEFINED;

        dev = get_accelerometer (manager->priv->client);
        if (dev == NULL) {
                if (manager->priv->name_id != 0)
                        g_bus_unown_name (manager->priv->name_id);
        } else {
                if (manager->priv->connection) {
                        manager->priv->name_id = g_bus_own_name_on_connection (manager->priv->connection,
                                                                               GSD_ORIENTATION_DBUS_NAME,
                                                                               G_BUS_NAME_OWNER_FLAGS_NONE,
                                                                               NULL,
                                                                               NULL,
                                                                               NULL,
                                                                               NULL);
                }

                manager->priv->sysfs_path = g_strdup (g_udev_device_get_sysfs_path (dev));
                g_debug ("Found accelerometer at sysfs path '%s'", manager->priv->sysfs_path);

                manager->priv->prev_orientation = get_orientation_from_device (dev);
                g_object_unref (dev);
        }
}