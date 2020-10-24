static void
set_natural_scroll (GsdMouseManager *manager,
                    GdkDevice       *device,
                    gboolean         natural_scroll)
{
        XDevice *xdevice;
        Atom scrolling_distance, act_type;
        int rc, act_format;
        unsigned long nitems, bytes_after;
        unsigned char *data;
        glong *ptr;

        xdevice = open_gdk_device (device);
        if (xdevice == NULL)
                return;

        if (!device_is_touchpad (xdevice)) {
                xdevice_close (xdevice);
                return;
        }

        g_debug ("Trying to set %s for \"%s\"",
                 natural_scroll ? "natural (reverse) scroll" : "normal scroll",
                 gdk_device_get_name (device));

        scrolling_distance = XInternAtom (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()),
                                          "Synaptics Scrolling Distance", False);

        gdk_error_trap_push ();
        rc = XGetDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                 scrolling_distance, 0, 2, False,
                                 XA_INTEGER, &act_type, &act_format, &nitems,
                                 &bytes_after, &data);

        if (rc == Success && act_type == XA_INTEGER && act_format == 32 && nitems >= 2) {
                ptr = (glong *) data;

                if (natural_scroll) {
                        ptr[0] = -abs(ptr[0]);
                        ptr[1] = -abs(ptr[1]);
                } else {
                        ptr[0] = abs(ptr[0]);
                        ptr[1] = abs(ptr[1]);
                }

                XChangeDeviceProperty (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), xdevice,
                                       scrolling_distance, XA_INTEGER, act_format,
                                       PropModeReplace, data, nitems);
        }

        if (gdk_error_trap_pop ())
                g_warning ("Error setting %s for \"%s\"",
                           natural_scroll ? "natural (reverse) scroll" : "normal scroll",
                           gdk_device_get_name (device));

        if (rc == Success)
                XFree (data);

        xdevice_close (xdevice);
}