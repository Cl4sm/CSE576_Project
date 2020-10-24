{
    g_assert(adapter != NULL && ADAPTER_IS(adapter));
    g_assert(name != NULL && strlen(name) > 0);

    Device *device = NULL;
    
    Manager *manager = g_object_new(MANAGER_TYPE, NULL);
    
    GVariant *objects = manager_get_managed_objects(manager, error);
    
    g_clear_object(&manager);
    
    if(!objects)
    {
        return NULL;
    }
    
    const gchar *object_path;
    GVariant *ifaces_and_properties;
    GVariantIter i;

    g_variant_iter_init(&i, objects);
    while (g_variant_iter_next(&i, "{&o@a{sa{sv}}}", &object_path, &ifaces_and_properties) && device == NULL)
    {
        GVariant *properties = NULL;
        
        if(g_variant_lookup(ifaces_and_properties, DEVICE_DBUS_INTERFACE, "@a{sv}", &properties))
        {
            gchar *adapter_path = NULL;
            if(g_variant_lookup(properties, "Adapter", "o", &adapter_path))
            {
                if(g_strcmp0(adapter_path, adapter_get_dbus_object_path(adapter)) == 0)
                {
                    // Now check if this is the device we are looking for.
                    
                    // Try to find by MAC
                    gchar *address = NULL;
                    
                    if(g_variant_lookup(properties, "Address", "s", &address))
                    {
                        if(g_strcmp0(g_ascii_strdown(address, -1), g_ascii_strdown(name, -1)) == 0)
                        {
                            device = device_new(object_path);
                        }
                        g_free(address);
                    }
                    
                    // Try to find by name or alias
                    if(!device)
                    {
                        gchar *device_name = NULL;
                        gchar *device_alias = NULL;
                        g_variant_lookup(properties, "Name", "s", &device_name);
                        g_variant_lookup(properties, "Alias", "s", &device_alias);
                        
                        if (g_strcmp0(name, device_name) == 0 || g_strcmp0(name, device_alias) == 0) {
                            device = device_new(object_path);
                        }
                        
                        g_free(device_alias);
                        g_free(device_name);
                    }
                }
                g_free(adapter_path);
            }
            g_variant_unref(properties);
        }
        g_variant_unref(ifaces_and_properties);
    }
    g_variant_unref(objects);
    
    return device;
}
