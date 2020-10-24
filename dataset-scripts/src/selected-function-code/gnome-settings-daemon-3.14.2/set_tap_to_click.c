static void
set_tap_to_click (GdkDevice *device,
                  gboolean   state,
                  gboolean   left_handed)
{
        int format, rc;
        unsigned long nitems, bytes_after;
        XDevice *xdevice;
        unsigned char* data;
        Atom prop, type;

        prop = XInternAtom (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), "Synaptics Tap Action", False);
        if (!prop)
                return;

        xdevice = open_gdk_device (device);
        if (xdevice == NULL)
                return;

        if (!device_is_touchpad (xdevice)) {
                xdevice_close (xdevice);
                return;
        }

	g_debug ("setting tap to click on %s", gdk_device_get_name (device));

        gdk_error_trap_push ();
        rc = XGetDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice, prop, 0, 2,
                                 False, XA_INTEGER, &type, &format, &nitems,
                                 &bytes_after, &data);

        if (rc == Success && type == XA_INTEGER && format == 8 && nitems >= 7) {
                /* Set RLM mapping for 1/2/3 fingers*/
                data[4] = (state) ? ((left_handed) ? 3 : 1) : 0;
                data[5] = (state) ? ((left_handed) ? 1 : 3) : 0;
                data[6] = (state) ? 2 : 0;
                XChangeDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice, prop, XA_INTEGER, 8,
                                       PropModeReplace, data, nitems);
        }

        if (rc == Success)
                XFree (data);

        if (gdk_error_trap_pop ())
                g_warning ("Error in setting tap to click on \"%s\"", gdk_device_get_name (device));

        xdevice_close (xdevice);
}