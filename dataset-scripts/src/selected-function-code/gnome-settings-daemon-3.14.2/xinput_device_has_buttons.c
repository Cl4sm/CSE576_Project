static gboolean
xinput_device_has_buttons (GdkDevice *device)
{
        int i;
        XAnyClassInfo *class_info;

        /* FIXME can we use the XDevice's classes here instead? */
        XDeviceInfo *device_info, *info;
        gint n_devices;
        int id;

        /* Find the XDeviceInfo for the GdkDevice */
        g_object_get (G_OBJECT (device), "device-id", &id, NULL);

        device_info = XListInputDevices (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()), &n_devices);
        if (device_info == NULL)
                return FALSE;

        info = NULL;
        for (i = 0; i < n_devices; i++) {
                if (device_info[i].id == id) {
                        info = &device_info[i];
                        break;
                }
        }
        if (info == NULL)
                goto bail;

        class_info = info->inputclassinfo;
        for (i = 0; i < info->num_classes; i++) {
                if (class_info->class == ButtonClass) {
                        XButtonInfo *button_info;

                        button_info = (XButtonInfo *) class_info;
                        if (button_info->num_buttons > 0) {
                                XFreeDeviceList (device_info);
                                return TRUE;
                        }
                }

                class_info = (XAnyClassInfo *) (((guchar *) class_info) +
                                                class_info->length);
        }

bail:
        XFreeDeviceList (device_info);

        return FALSE;
}