Adapter *find_adapter(const gchar *name, GError **error)
{
    gchar *adapter_path = NULL;
    Adapter *adapter = NULL;

    Manager *manager = g_object_new(MANAGER_TYPE, NULL);

    // If name is null or empty - return default adapter
    if (name == NULL || strlen(name) == 0)
    {
        adapter_path = (gchar *) manager_default_adapter(manager, error);
        if (adapter_path)
        {
            // adapter = g_object_new(ADAPTER_TYPE, "DBusObjectPath", adapter_path, NULL);
            adapter = adapter_new(adapter_path);
        }
    }
    else
    {
        // Try to find by id
        adapter_path = (gchar *) manager_find_adapter(manager, name, error);

        // Found
        if (adapter_path)
        {
            // adapter = g_object_new(ADAPTER_TYPE, "DBusObjectPath", adapter_path, NULL);
            adapter = adapter_new(adapter_path);
        }
        else
        {
            // Try to find by name
            const GPtrArray *adapters_list = manager_get_adapters(manager);
            g_assert(adapters_list != NULL);
            for (int i = 0; i < adapters_list->len; i++)
            {
                adapter_path = g_ptr_array_index(adapters_list, i);
                // adapter = g_object_new(ADAPTER_TYPE, "DBusObjectPath", adapter_path, NULL);
                adapter = adapter_new(adapter_path);
                adapter_path = NULL;

                if (g_strcmp0(name, adapter_get_name(adapter, error)) == 0)
                {
                    if (error)
                    {
                        g_error_free(*error);
                        *error = NULL;
                    }
                    break;
                }

                g_object_unref(adapter);
                adapter = NULL;
            }
        }
    }

    g_object_unref(manager);
    if (adapter_path) g_free(adapter_path);

    return adapter;
}
