static void
update_brightness_cb (GObject             *source_object,
                      GAsyncResult        *res,
                      gpointer             user_data)
{
        GError *error = NULL;
        int percentage;
        GVariant *new_percentage;
        GsdMediaKeysManager *manager = GSD_MEDIA_KEYS_MANAGER (user_data);
        const char *icon, *debug;

        if (G_DBUS_PROXY (source_object) == manager->priv->power_keyboard_proxy) {
                icon = "keyboard-brightness-symbolic";
                debug = "keyboard";
        } else {
                icon = "display-brightness-symbolic";
                debug = "screen";
        }

        new_percentage = g_dbus_proxy_call_finish (G_DBUS_PROXY (source_object),
                                                   res, &error);
        if (new_percentage == NULL) {
                g_warning ("Failed to set new %s percentage: %s",
                           debug, error->message);
                g_error_free (error);
                return;
        }

        /* update the dialog with the new value */
        g_variant_get (new_percentage, "(i)", &percentage);
        show_osd (manager, icon, NULL, percentage);
        g_variant_unref (new_percentage);
}