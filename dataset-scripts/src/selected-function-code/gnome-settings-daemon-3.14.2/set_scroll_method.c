static void
set_scroll_method (GsdMouseManager         *manager,
                   GdkDevice               *device,
                   GsdTouchpadScrollMethod  method)
{
        int rc;
        XDevice *xdevice;
        Atom act_type, prop, prop_edge, prop_twofinger;
        int act_format;
        unsigned long nitems, bytes_after;
        unsigned char *data;

        prop = XInternAtom (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), "Synaptics Capabilities", True);
        prop_edge = XInternAtom (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), "Synaptics Edge Scrolling", False);
        prop_twofinger = XInternAtom (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), "Synaptics Two-Finger Scrolling", False);

        if (!prop_edge || !prop_twofinger || !prop)
                return;

        xdevice = open_gdk_device (device);
        if (xdevice == NULL)
                return;

        if (!device_is_touchpad (xdevice)) {
                xdevice_close (xdevice);
                return;
        }

	g_debug ("setting edge scroll on %s", gdk_device_get_name (device));

        gdk_error_trap_push ();
        rc = XGetDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                 prop, 0, 2, False,
                                 XA_INTEGER, &act_type, &act_format, &nitems,
                                 &bytes_after, &data);
        if (rc == Success && act_type != None) {
                if (!(data[3]) && method == GSD_TOUCHPAD_SCROLL_METHOD_TWO_FINGER_SCROLLING) {
                        g_warning ("Two finger scroll is not supported by %s", gdk_device_get_name (device));
                        method = GSD_TOUCHPAD_SCROLL_METHOD_EDGE_SCROLLING;
                        g_settings_set_enum (manager->priv->touchpad_settings, KEY_SCROLL_METHOD, method);
                }

                XFree (data);
        }

        rc = XGetDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                 prop_edge, 0, 1, False,
                                 XA_INTEGER, &act_type, &act_format, &nitems,
                                 &bytes_after, &data);
        if (rc == Success && act_type == XA_INTEGER &&
            act_format == 8 && nitems >= 2) {
                data[0] = (method == GSD_TOUCHPAD_SCROLL_METHOD_EDGE_SCROLLING) ? 1 : 0;
                XChangeDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                       prop_edge, XA_INTEGER, 8,
                                       PropModeReplace, data, nitems);
        }

        XFree (data);

        rc = XGetDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                 prop_twofinger, 0, 1, False,
                                 XA_INTEGER, &act_type, &act_format, &nitems,
                                 &bytes_after, &data);
        if (rc == Success && act_type == XA_INTEGER &&
            act_format == 8 && nitems >= 2) {
                data[0] = (method == GSD_TOUCHPAD_SCROLL_METHOD_TWO_FINGER_SCROLLING) ? 1 : 0;
                XChangeDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                       prop_twofinger, XA_INTEGER, 8,
                                       PropModeReplace, data, nitems);
        }

        if (gdk_error_trap_pop ())
                g_warning ("Error in setting edge scroll on \"%s\"", gdk_device_get_name (device));

        if (rc == Success)
                XFree (data);

        xdevice_close (xdevice);
}