static void
mouse_callback (GSettings       *settings,
                const gchar     *key,
                GsdMouseManager *manager)
{
        GList *devices, *l;

        if (g_str_equal (key, KEY_DWELL_CLICK_ENABLED) ||
            g_str_equal (key, KEY_SECONDARY_CLICK_ENABLED)) {
                set_mousetweaks_daemon (manager,
                                        g_settings_get_boolean (settings, KEY_DWELL_CLICK_ENABLED),
                                        g_settings_get_boolean (settings, KEY_SECONDARY_CLICK_ENABLED));
                return;
        } else if (g_str_equal (key, KEY_LOCATE_POINTER)) {
                set_locate_pointer (manager, g_settings_get_boolean (settings, KEY_LOCATE_POINTER));
                return;
        }

        devices = gdk_device_manager_list_devices (manager->priv->device_manager, GDK_DEVICE_TYPE_SLAVE);

        for (l = devices; l != NULL; l = l->next) {
                GdkDevice *device = l->data;

                if (device_is_ignored (manager, device))
                        continue;

                if (g_str_equal (key, KEY_LEFT_HANDED)) {
                        gboolean mouse_left_handed;
                        mouse_left_handed = g_settings_get_boolean (settings, KEY_LEFT_HANDED);
                        set_left_handed (manager, device, mouse_left_handed, get_touchpad_handedness (manager, mouse_left_handed));
                } else if (g_str_equal (key, KEY_MOTION_ACCELERATION) ||
                           g_str_equal (key, KEY_MOTION_THRESHOLD)) {
                        set_motion (manager, device);
                } else if (g_str_equal (key, KEY_MIDDLE_BUTTON_EMULATION)) {
                        set_middle_button (manager, device, g_settings_get_boolean (settings, KEY_MIDDLE_BUTTON_EMULATION));
                }
        }
        g_list_free (devices);
}