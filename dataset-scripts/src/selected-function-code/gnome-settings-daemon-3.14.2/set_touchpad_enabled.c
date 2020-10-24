static void
set_touchpad_enabled (int id)
{
        XDevice *xdevice;

        g_debug ("Trying to set device enabled for %d", id);

        gdk_error_trap_push ();
        xdevice = XOpenDevice (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), id);
        if (gdk_error_trap_pop () != 0)
                return;

        if (!device_is_touchpad (xdevice)) {
                xdevice_close (xdevice);
                return;
        }

        if (set_device_enabled (id, TRUE) == FALSE)
                g_warning ("Error enabling device \"%d\"", id);
        else
                g_debug ("Enabled device %d", id);

        xdevice_close (xdevice);
}