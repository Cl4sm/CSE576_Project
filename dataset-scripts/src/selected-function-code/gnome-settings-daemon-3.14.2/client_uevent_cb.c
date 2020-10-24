static void
client_uevent_cb (GUdevClient           *client,
                  gchar                 *action,
                  GUdevDevice           *device,
                  GsdOrientationManager *manager)
{
        const char *sysfs_path;
        OrientationUp orientation;

        sysfs_path = g_udev_device_get_sysfs_path (device);
        g_debug ("Received uevent '%s' from '%s'", action, sysfs_path);

        if (manager->priv->orientation_lock)
                return;

        if (g_str_equal (action, "change") == FALSE) {
                /* "add", "remove", or "move" */
                update_accelerometer (manager);
                return;
        }

        if (g_strcmp0 (manager->priv->sysfs_path, sysfs_path) != 0)
                return;

        g_debug ("Received an event from the accelerometer");

        orientation = get_orientation_from_device (device);
        if (orientation != manager->priv->prev_orientation) {
                manager->priv->prev_orientation = orientation;
                g_debug ("Orientation changed to '%s', switching screen rotation",
                         orientation_to_string (manager->priv->prev_orientation));

                do_rotation (manager);
        }
}