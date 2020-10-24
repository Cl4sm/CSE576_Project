const gchar **manager_get_devices(Manager *self, const gchar *adapter_pattern)
{
    g_assert(MANAGER_IS(self));

    GVariant *objects = NULL;
    GError *error = NULL;
    objects = manager_get_managed_objects(self, &error);
    if (objects == NULL)
    {
        g_critical("%s", error->message);
        g_error_free(error);
        return NULL;
    }
    
    GRegex *adapter_regex = g_regex_new(adapter_pattern, 0, 0, &error);
    if (adapter_regex == NULL)
    {
        g_critical("%s", error->message);
        g_error_free(error);
    }
    
    GPtrArray *device_array = g_ptr_array_new();
    
    const gchar *object_path;
    GVariant *ifaces_and_properties;
    GVariantIter i;
    
    g_variant_iter_init(&i, objects);
    while (g_variant_iter_next(&i, "{&o@a{sa{sv}}}", &object_path, &ifaces_and_properties))
    {
        const gchar *interface_name;
        GVariant *properties;
        GVariantIter ii;
        g_variant_iter_init(&ii, ifaces_and_properties);
        while (g_variant_iter_next(&ii, "{&s@a{sv}}", &interface_name, &properties))
        {
            if (g_strcmp0(interface_name, "org.bluez.Device1") == 0)
            {
                const gchar *adapter_prop = g_variant_get_string(g_variant_lookup_value(properties, "Adapter", G_VARIANT_TYPE_OBJECT_PATH), NULL);
                if(g_regex_match(adapter_regex, adapter_prop, 0, NULL))
                    g_ptr_array_add(device_array, (gpointer) g_strdup(object_path));
            }
            g_variant_unref(properties);
        }
        g_variant_unref(ifaces_and_properties);
    }
    g_variant_unref(objects);

    g_regex_unref(adapter_regex);
    
    if(device_array->len > 0)
    {
        // Top it off with a NULL pointer
        g_ptr_array_add(device_array, (gpointer) NULL);
        return (const gchar**) g_ptr_array_free(device_array, FALSE);
    }
    else
        return NULL;
}