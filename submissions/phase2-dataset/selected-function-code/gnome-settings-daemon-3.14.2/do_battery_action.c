static void
do_battery_action (GsdMediaKeysManager *manager)
{
        gdouble percentage;
        UpDeviceKind kind;
        gchar *icon_name;

        g_return_if_fail (manager->priv->composite_device != NULL);

        g_object_get (manager->priv->composite_device,
                      "kind", &kind,
                      "icon-name", &icon_name,
                      "percentage", &percentage,
                      NULL);

        if (kind == UP_DEVICE_KIND_UPS || kind == UP_DEVICE_KIND_BATTERY) {
                g_debug ("showing battery level OSD");
                show_osd (manager, icon_name, NULL, percentage);
        }

        g_free (icon_name);
}