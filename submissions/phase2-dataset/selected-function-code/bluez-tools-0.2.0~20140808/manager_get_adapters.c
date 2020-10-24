GPtrArray *manager_get_adapters(Manager *self)
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

    GPtrArray *adapter_array = g_ptr_array_new();

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
            if (g_strstr_len(g_ascii_strdown(interface_name, -1), -1, "adapter"))
                g_ptr_array_add(adapter_array, (gpointer) g_strdup(object_path));
            g_variant_unref(properties);
        }
        g_variant_unref(ifaces_and_properties);
    }
    g_variant_unref(objects);

    return adapter_array;
}
