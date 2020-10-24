static void
set_middle_button (GsdMouseManager *manager,
                   GdkDevice       *device,
                   gboolean         middle_button)
{
        Atom prop;
        XDevice *xdevice;
        Atom type;
        int format;
        unsigned long nitems, bytes_after;
        unsigned char *data;
        int rc;

        prop = XInternAtom (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()),
                            "Evdev Middle Button Emulation", True);

        if (!prop) /* no evdev devices */
                return;

        xdevice = open_gdk_device (device);
        if (xdevice == NULL)
                return;

	g_debug ("setting middle button on %s", gdk_device_get_name (device));

        gdk_error_trap_push ();

        rc = XGetDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()),
                            xdevice, prop, 0, 1, False, XA_INTEGER, &type, &format,
                            &nitems, &bytes_after, &data);

        if (rc == Success && format == 8 && type == XA_INTEGER && nitems == 1) {
                data[0] = middle_button ? 1 : 0;

                XChangeDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()),
                                       xdevice, prop, type, format, PropModeReplace, data, nitems);
        }

        if (gdk_error_trap_pop ())
                g_warning ("Error in setting middle button emulation on \"%s\"", gdk_device_get_name (device));

        if (rc == Success)
                XFree (data);

        xdevice_close (xdevice);
}