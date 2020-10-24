static void _obex_opp_client_properties_handler(GDBusConnection *connection, const gchar *sender_name, const gchar *object_path, const gchar *interface_name, const gchar *signal_name, GVariant *parameters, gpointer user_data)
{
    const gchar *arg0 = g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL);
    GVariant *changed_properties = g_variant_get_child_value(parameters, 1);
    
    if(g_strcmp0(arg0, OBEX_TRANSFER_DBUS_INTERFACE) == 0)
    {
        ObexTransfer *transfer = g_hash_table_lookup(_transfers, object_path);
        ObexTransferInfo *info = g_hash_table_lookup(_transfer_infos, object_path);
        
        guint64 size = 0x0;
        guint64 transferred = 0x0;
        obex_transfer_get_size(transfer, NULL);
        g_variant_lookup(changed_properties, "Size", "t", &size);
        if(!size)
            size = obex_transfer_get_size(transfer, NULL);
        g_variant_lookup(changed_properties, "Transferred", "t", &transferred);
        
        if(size && transferred && g_strcmp0(info->status, "active") == 0)
        {
            guint pp = (transferred / (gfloat) size)*100;

            if (!_update_progress)
            {
                g_print("[Transfer#%s] Progress: %3u%%", obex_transfer_get_name(transfer, NULL), pp);
                _update_progress = TRUE;
            }
            else
            {
                g_print("\b\b\b\b%3u%%", pp);
            }

            if (pp == 100)
            {
                g_print("\n");
                _update_progress = FALSE;
            }
        }
        
        gchar *status = NULL;
        g_variant_lookup(changed_properties, "Status", "s", &status);
        
        if(status)
        {
            g_free(info->status);
            info->status = g_strdup(status);
            
            if(g_strcmp0(status, "active") == 0)
            {
                // g_print("[Client Server] Transfer active\n");
            }
            else if(g_strcmp0(status, "complete") == 0)
            {
                if(_update_progress)
                {
                    _update_progress = FALSE;
                    g_print("\b\b\b\b%3u%%", 100);
                    g_print("\n");
                }
                    
                g_print("[Transfer#%s] Completed\n", info->filename);
            }
            else if(g_strcmp0(status, "error") == 0)
            {
                if(_update_progress)
                {
                    _update_progress = FALSE;
                    g_print("\n");
                }

                g_print("[Transfer#%s] Failed\n", info->filename);
            }
            else if(g_strcmp0(status, "queued") == 0)
            {
                // g_print("[OBEX Client] Transfer queued\n");
            }
            else if(g_strcmp0(status, "suspended") == 0)
            {
                if(_update_progress)
                {
                    _update_progress = FALSE;
                    g_print("\n");
                }
                    
                g_print("[Transfer#%s] Suspended\n", info->filename);
            }
            g_free(status);
        }
    }
    
    g_variant_unref(changed_properties);
}
