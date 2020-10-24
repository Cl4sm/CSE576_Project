static void
set_left_handed (GsdMouseManager *manager,
                 GdkDevice       *device,
                 gboolean mouse_left_handed,
                 gboolean touchpad_left_handed)
{
        XDevice *xdevice;
        guchar *buttons;
        gsize buttons_capacity = 16;
        gboolean left_handed;
        gint n_buttons;

        if (!xinput_device_has_buttons (device))
                return;

        xdevice = open_gdk_device (device);
        if (xdevice == NULL)
                return;

	g_debug ("setting handedness on %s", gdk_device_get_name (device));

        buttons = g_new (guchar, buttons_capacity);

        /* If the device is a touchpad, swap tap buttons
         * around too, otherwise a tap would be a right-click */
        if (device_is_touchpad (xdevice)) {
                gboolean tap = g_settings_get_boolean (manager->priv->touchpad_settings, KEY_TAP_TO_CLICK);
                gboolean single_button = touchpad_has_single_button (xdevice);

                left_handed = touchpad_left_handed;

                if (tap && !single_button)
                        set_tap_to_click (device, tap, left_handed);

                if (single_button)
                        goto out;
        } else {
                left_handed = mouse_left_handed;
        }

        gdk_error_trap_push ();

        n_buttons = XGetDeviceButtonMapping (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                             buttons,
                                             buttons_capacity);

        while (n_buttons > buttons_capacity) {
                buttons_capacity = n_buttons;
                buttons = (guchar *) g_realloc (buttons,
                                                buttons_capacity * sizeof (guchar));

                n_buttons = XGetDeviceButtonMapping (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                                     buttons,
                                                     buttons_capacity);
        }

        configure_button_layout (buttons, n_buttons, left_handed);

        XSetDeviceButtonMapping (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice, buttons, n_buttons);
        gdk_error_trap_pop_ignored ();

out:
        xdevice_close (xdevice);
        g_free (buttons);
}