static GsdUpdatesFirmwareRequest *
request_new (const gchar *filename, const gchar *sysfs_path)
{
        GsdUpdatesFirmwareRequest *req;
#ifdef HAVE_GUDEV
        GUdevDevice *device;
        GUdevClient *client;
        const gchar *subsystem;
        const gchar *model;
        const gchar *id_vendor;
        const gchar *id_product;
#endif

        req = g_new0 (GsdUpdatesFirmwareRequest, 1);
        req->filename = g_strdup (filename);
        req->sysfs_path = g_strdup (sysfs_path);
        req->subsystem = FIRMWARE_SUBSYSTEM_UNKNOWN;
#ifdef HAVE_GUDEV

        /* get all subsystems */
        client = g_udev_client_new (NULL);
        device = g_udev_client_query_by_sysfs_path (client, sysfs_path);
        if (device == NULL)
                goto out;

        /* find subsystem, which will affect if we have to replug, or reboot */
        subsystem = g_udev_device_get_subsystem (device);
        if (g_strcmp0 (subsystem, "usb") == 0) {
                req->subsystem = FIRMWARE_SUBSYSTEM_USB;
        } else if (g_strcmp0 (subsystem, "pci") == 0) {
                req->subsystem = FIRMWARE_SUBSYSTEM_PCI;
        } else {
                g_warning ("subsystem unrecognised: %s", subsystem);
        }

        /* get model, so we can show something sensible */
        model = g_udev_device_get_property (device, "ID_MODEL");
        if (model != NULL && model[0] != '\0') {
                req->model = g_strdup (model);
                /* replace invalid chars */
                g_strdelimit (req->model, "_", ' ');
        }

        /* create ID so we can ignore the specific device */
        id_vendor = g_udev_device_get_property (device, "ID_VENDOR");
        id_product = g_udev_device_get_property (device, "ID_MODEL_ID");
        req->id = g_strdup_printf ("%s_%s", id_vendor, id_product);
out:
        if (device != NULL)
                g_object_unref (device);
        g_object_unref (client);
#endif
        return req;
}