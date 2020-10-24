static void
set_horiz_scroll (GdkDevice *device,
                  gboolean   state)
{
        int rc;
        XDevice *xdevice;
        Atom act_type, prop_edge, prop_twofinger;
        int act_format;
        unsigned long nitems, bytes_after;
        unsigned char *data;

        prop_edge = XInternAtom (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), "Synaptics Edge Scrolling", False);
        prop_twofinger = XInternAtom (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), "Synaptics Two-Finger Scrolling", False);

        if (!prop_edge || !prop_twofinger)
                return;

        xdevice = open_gdk_device (device);
        if (xdevice == NULL)
                return;

        if (!device_is_touchpad (xdevice)) {
                xdevice_close (xdevice);
                return;
        }

	g_debug ("setting horiz scroll on %s", gdk_device_get_name (device));

        gdk_error_trap_push ();
        rc = XGetDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                 prop_edge, 0, 1, False,
                                 XA_INTEGER, &act_type, &act_format, &nitems,
                                 &bytes_after, &data);
        if (rc == Success && act_type == XA_INTEGER &&
            act_format == 8 && nitems >= 2) {
                data[1] = (state && data[0]);
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
                data[1] = (state && data[0]);
                XChangeDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                       prop_twofinger, XA_INTEGER, 8,
                                       PropModeReplace, data, nitems);
        }

        if (gdk_error_trap_pop ())
                g_warning ("Error in setting horiz scroll on \"%s\"", gdk_device_get_name (device));

        if (rc == Success)
                XFree (data);

        xdevice_close (xdevice);

}