static void
do_brightness_action (GsdMediaKeysManager *manager,
                      MediaKeyType type)
{
        const char *cmd;
        GDBusProxy *proxy;

        switch (type) {
        case KEYBOARD_BRIGHTNESS_UP_KEY:
        case KEYBOARD_BRIGHTNESS_DOWN_KEY:
        case KEYBOARD_BRIGHTNESS_TOGGLE_KEY:
                proxy = manager->priv->power_keyboard_proxy;
                break;
        case SCREEN_BRIGHTNESS_UP_KEY:
        case SCREEN_BRIGHTNESS_DOWN_KEY:
                proxy = manager->priv->power_screen_proxy;
                break;
        default:
                g_assert_not_reached ();
        }

        if (manager->priv->connection == NULL ||
            proxy == NULL) {
                g_warning ("No existing D-Bus connection trying to handle power keys");
                return;
        }

        switch (type) {
        case KEYBOARD_BRIGHTNESS_UP_KEY:
        case SCREEN_BRIGHTNESS_UP_KEY:
                cmd = "StepUp";
                break;
        case KEYBOARD_BRIGHTNESS_DOWN_KEY:
        case SCREEN_BRIGHTNESS_DOWN_KEY:
                cmd = "StepDown";
                break;
        case KEYBOARD_BRIGHTNESS_TOGGLE_KEY:
                cmd = "Toggle";
                break;
        default:
                g_assert_not_reached ();
        }

        /* call into the power plugin */
        g_dbus_proxy_call (proxy,
                           cmd,
                           NULL,
                           G_DBUS_CALL_FLAGS_NONE,
                           -1,
                           NULL,
                           update_brightness_cb,
                           manager);
}