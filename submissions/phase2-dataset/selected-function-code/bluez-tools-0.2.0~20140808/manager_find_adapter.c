{
    g_assert(MANAGER_IS(self));

    GVariant *objects = NULL;
    objects = manager_get_managed_objects(self, error);
    if (objects == NULL)
        return NULL;

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
            {
                const gchar *object_base_name = g_path_get_basename(object_path);
                if (g_strstr_len(g_ascii_strdown(object_base_name, -1), -1, g_ascii_strdown(pattern, -1)))
                {
                    const gchar *retVal = g_strdup(object_path);
                    g_variant_unref(properties);
                    g_variant_unref(ifaces_and_properties);
                    g_variant_unref(objects);
                    return retVal;
                }
                const gchar *address = g_variant_get_string(g_variant_lookup_value(properties, "Address", NULL), NULL);
                if (g_strstr_len(g_ascii_strdown(address, -1), -1, g_ascii_strdown(pattern, -1)))
                {
                    gchar *retVal = g_strdup(object_path);
                    g_variant_unref(properties);
                    g_variant_unref(ifaces_and_properties);
                    g_variant_unref(objects);
                    return retVal;
                }
            }
            g_variant_unref(properties);
        }
        g_variant_unref(ifaces_and_properties);
    }
    g_variant_unref(objects);

    return NULL;
}
