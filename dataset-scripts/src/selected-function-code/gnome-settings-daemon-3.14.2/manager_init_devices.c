static void
manager_init_devices (GsdXrandrManager *manager)
{
        GdkDisplay *display;
        GList *devices, *d;
        GdkScreen *screen;

        screen = gdk_screen_get_default ();
        display = gdk_screen_get_display (screen);

        manager->priv->device_mapper = gsd_device_mapper_get ();
        manager->priv->device_manager = gdk_display_get_device_manager (display);
        manager->priv->device_added_id =
                g_signal_connect_swapped (manager->priv->device_manager, "device-added",
                                          G_CALLBACK (manager_device_added), manager);
        manager->priv->device_removed_id =
                g_signal_connect_swapped (manager->priv->device_manager, "device-removed",
                                  G_CALLBACK (manager_device_removed), manager);

        devices = gdk_device_manager_list_devices (manager->priv->device_manager,
                                                   GDK_DEVICE_TYPE_SLAVE);

        for (d = devices; d; d = d->next)
                manager_device_added (manager, d->data);

        g_list_free (devices);
}