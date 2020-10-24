static void
gsd_updates_firmware_finalize (GObject *object)
{
        GsdUpdatesFirmware *firmware;

        g_return_if_fail (GSD_UPDATES_IS_FIRMWARE (object));

        firmware = GSD_UPDATES_FIRMWARE (object);

        g_return_if_fail (firmware->priv != NULL);
        g_ptr_array_unref (firmware->priv->array_requested);
        g_ptr_array_unref (firmware->priv->packages_found);
        g_object_unref (PK_CLIENT(firmware->priv->task));
        g_object_unref (firmware->priv->settings);
        if (firmware->priv->monitor != NULL)
                g_object_unref (firmware->priv->monitor);
        if (firmware->priv->timeout_id > 0)
                g_source_remove (firmware->priv->timeout_id);

        G_OBJECT_CLASS (gsd_updates_firmware_parent_class)->finalize (object);
}