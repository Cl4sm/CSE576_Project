static void
touchpad_callback (GSettings       *settings,
                   const gchar     *key,
                   GsdMouseManager *manager)
{
        GList *devices, *l;

        if (g_str_equal (key, KEY_TOUCHPAD_DISABLE_W_TYPING)) {
                set_disable_w_typing (manager, g_settings_get_boolean (manager->priv->touchpad_settings, key));
                return;
        }

        devices = gdk_device_manager_list_devices (manager->priv->device_manager, GDK_DEVICE_TYPE_SLAVE);

        for (l = devices; l != NULL; l = l->next) {
                GdkDevice *device = l->data;

                if (device_is_ignored (manager, device))
                        continue;

                if (g_str_equal (key, KEY_TAP_TO_CLICK)) {
                        gboolean mouse_left_handed;
                        mouse_left_handed = g_settings_get_boolean (manager->priv->mouse_settings, KEY_LEFT_HANDED);
                        set_tap_to_click (device, g_settings_get_boolean (settings, key),
                                          get_touchpad_handedness (manager, mouse_left_handed));
                } else if (g_str_equal (key, KEY_SCROLL_METHOD)) {
                        set_scroll_method (manager, device, g_settings_get_enum (settings, key));
                        set_horiz_scroll (device, g_settings_get_boolean (settings, KEY_PAD_HORIZ_SCROLL));
                } else if (g_str_equal (key, KEY_PAD_HORIZ_SCROLL)) {
                        set_horiz_scroll (device, g_settings_get_boolean (settings, key));
                } else if (g_str_equal (key, KEY_TOUCHPAD_ENABLED)) {
                        if (g_settings_get_boolean (settings, key) == FALSE)
                                set_touchpad_disabled (device);
                        else
                                set_touchpad_enabled (gdk_x11_device_get_id (device));
                } else if (g_str_equal (key, KEY_MOTION_ACCELERATION) ||
                           g_str_equal (key, KEY_MOTION_THRESHOLD)) {
                        set_motion (manager, device);
                } else if (g_str_equal (key, KEY_LEFT_HANDED)) {
                        gboolean mouse_left_handed;
                        mouse_left_handed = g_settings_get_boolean (manager->priv->mouse_settings, KEY_LEFT_HANDED);
                        set_left_handed (manager, device, mouse_left_handed, get_touchpad_handedness (manager, mouse_left_handed));
                } else if (g_str_equal (key, KEY_NATURAL_SCROLL_ENABLED)) {
                        set_natural_scroll (manager, device, g_settings_get_boolean (settings, key));
                }
        }
        g_list_free (devices);

        if (g_str_equal (key, KEY_TOUCHPAD_ENABLED) &&
            g_settings_get_boolean (settings, key)) {
                devices = get_disabled_devices (manager->priv->device_manager);
                for (l = devices; l != NULL; l = l->next) {
                        int device_id;

                        device_id = GPOINTER_TO_INT (l->data);
                        set_touchpad_enabled (device_id);
                }
                g_list_free (devices);
        }
}