static const gchar *_find_device_pin(const gchar *device_path)
{
    if (_pin_hash_table)
    {
        GError *error = NULL;
        Device *device = device_new(device_path);
        const gchar *pin_by_addr = g_hash_table_lookup(_pin_hash_table, device_get_address(device, &error));
        const gchar *pin_by_alias = g_hash_table_lookup(_pin_hash_table, device_get_alias(device, &error));
        if(error)
        {
            g_critical("Failed to get remote device's MAC address: %s", error->message);
            g_error_free(error);
        }
        g_object_unref(device);
        const gchar *pin_all = g_hash_table_lookup(_pin_hash_table, "*");
        if (pin_by_addr)
            return pin_by_addr;
        else if (pin_by_alias)
            return pin_by_alias;
        else if (pin_all)
            return pin_all;
    }
    return NULL;
}
