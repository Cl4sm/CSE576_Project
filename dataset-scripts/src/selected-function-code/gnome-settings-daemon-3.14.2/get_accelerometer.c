static GUdevDevice *
get_accelerometer (GUdevClient *client)
{
        GList *list, *l;
        GUdevDevice *ret, *parent;

        /* Look for a device with the ID_INPUT_ACCELEROMETER=1 property */
        ret = NULL;
        list = g_udev_client_query_by_subsystem (client, "input");
        for (l = list; l != NULL; l = l->next) {
                GUdevDevice *dev;

                dev = l->data;
                if (g_udev_device_get_property_as_boolean (dev, "ID_INPUT_ACCELEROMETER")) {
                        ret = dev;
                        continue;
                }
                g_object_unref (dev);
        }
        g_list_free (list);

        if (ret == NULL)
                return NULL;

        /* Now walk up to the parent */
        parent = g_udev_device_get_parent (ret);
        if (parent == NULL)
                return ret;

        if (g_udev_device_get_property_as_boolean (parent, "ID_INPUT_ACCELEROMETER")) {
                g_object_unref (ret);
                ret = parent;
        } else {
                g_object_unref (parent);
        }

        return ret;
}