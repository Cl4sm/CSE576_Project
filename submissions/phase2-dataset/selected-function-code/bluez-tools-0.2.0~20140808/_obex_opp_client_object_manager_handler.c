static void _obex_opp_client_object_manager_handler(GDBusConnection *connection, const gchar *sender_name, const gchar *object_path, const gchar *interface_name, const gchar *signal_name, GVariant *parameters, gpointer user_data)
{
    if(g_strcmp0(signal_name, "InterfacesAdded") == 0)
    {
        const gchar *interface_object_path = g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL);
        GVariant *interfaces_and_properties = g_variant_get_child_value(parameters, 1);
        GVariant *properties = NULL;
        
        if(g_variant_lookup(interfaces_and_properties, OBEX_TRANSFER_DBUS_INTERFACE, "@a{sv}", &properties))
        {
            // g_print("[OBEX Client] Transfer started\n");
            ObexTransfer *t = obex_transfer_new(interface_object_path);
            g_hash_table_insert(_transfers, g_strdup(interface_object_path), t);

            ObexTransferInfo *info = g_malloc0(sizeof(ObexTransferInfo));
            info->filesize = g_variant_get_uint64(g_variant_lookup_value(properties, "Size", NULL));
            info->filename = g_strdup(g_variant_get_string(g_variant_lookup_value(properties, "Name", NULL), NULL));
            info->status = g_strdup(g_variant_get_string(g_variant_lookup_value(properties, "Status", NULL), NULL));
            ObexSession *session = obex_session_new(g_variant_get_string(g_variant_lookup_value(properties, "Session", NULL), NULL));
            
            info->obex_root = g_strdup(obex_session_get_root(session, NULL));
            
            g_object_unref(session);
            
            g_hash_table_insert(_transfer_infos, g_strdup(interface_object_path), info);
            if(g_strcmp0(info->status, "queued") == 0)
                g_print("[Transfer#%s] Waiting...\n", info->filename);
        }
        
        if(g_variant_lookup(interfaces_and_properties, OBEX_SESSION_DBUS_INTERFACE, "@a{sv}", &properties))
        {
            // g_print("[OBEX Client] OBEX session opened\n");
        }
        
        g_variant_unref(interfaces_and_properties);
        if(properties)
            g_variant_unref(properties);
    }
    else if(g_strcmp0(signal_name, "InterfacesRemoved") == 0)
    {
        const gchar *interface_object_path = g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL);
        GVariant *interfaces = g_variant_get_child_value(parameters, 1);
        const gchar **inf_array = g_variant_get_strv(interfaces, NULL);
        g_variant_unref(interfaces);
        const gchar **inf = NULL;
        for(inf = inf_array; *inf != NULL; inf++)
        {
            if(g_strcmp0(*inf, OBEX_TRANSFER_DBUS_INTERFACE) == 0)
            {
                // g_print("[OBEX Client] OBEX transfer closed\n");
                ObexTransfer *transfer = g_hash_table_lookup(_transfers, interface_object_path);
                g_hash_table_remove(_transfers, interface_object_path);
                g_object_unref(transfer);
                g_free(g_hash_table_lookup(_transfer_infos, interface_object_path));
                g_hash_table_remove(_transfer_infos, interface_object_path);
                if (g_main_loop_is_running(mainloop))
                    g_main_loop_quit(mainloop);
            }
            
            if(g_strcmp0(*inf, OBEX_SESSION_DBUS_INTERFACE) == 0)
            {
                // g_print("[OBEX Client] OBEX session closed\n");
            }
        }
        g_free(inf_array);
    }
}
