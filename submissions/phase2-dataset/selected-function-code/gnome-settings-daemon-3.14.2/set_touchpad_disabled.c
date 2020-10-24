static void
set_touchpad_disabled (GdkDevice *device)
{
        int id;
        XDevice *xdevice;

        g_object_get (G_OBJECT (device), "device-id", &id, NULL);

        g_debug ("Trying to set device disabled for \"%s\" (%d)", gdk_device_get_name (device), id);

        xdevice = open_gdk_device (device);
        if (xdevice == NULL)
                return;

        if (!device_is_touchpad (xdevice)) {
                xdevice_close (xdevice);
                return;
        }

        if (set_device_enabled (id, FALSE) == FALSE)
                g_warning ("Error disabling device \"%s\" (%d)", gdk_device_get_name (device), id);
        else
                g_debug ("Disabled device \"%s\" (%d)", gdk_device_get_name (device), id);

        xdevice_close (xdevice);
}