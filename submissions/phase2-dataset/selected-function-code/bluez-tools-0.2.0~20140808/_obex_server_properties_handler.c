{
    const gchar *arg0 = g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL);
    GVariant *changed_properties = g_variant_get_child_value(parameters, 1);
    
    if(g_strcmp0(arg0, OBEX_TRANSFER_DBUS_INTERFACE) == 0)
    {
        ObexTransfer *transfer = g_hash_table_lookup(_transfers, object_path);
        
        guint64 size = 0x0;
        guint64 transferred = 0x0;
        obex_transfer_get_size(transfer, NULL);
        g_variant_lookup(changed_properties, "Size", "t", &size);
        if(!size)
            size = obex_transfer_get_size(transfer, NULL);
        g_variant_lookup(changed_properties, "Transferred", "t", &transferred);
        
        if(size && transferred)
        {
            guint pp = (transferred / (gfloat) size)*100;

            if (!_update_progress)
            {
                g_print("[OBEXTransfer] Progress: %3u%%", pp);
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
            if(g_strcmp0(status, "active") == 0)
            {
                // g_print("[OBEX Server] Transfer active\n");
            }
            else if(g_strcmp0(status, "complete") == 0)
            {
                g_print("[OBEX Server] Transfer succeeded\n");
                ObexTransferInfo *info = g_hash_table_lookup(_transfer_infos, object_path);
                g_rename(g_build_filename(info->obex_root, info->filename, NULL), g_build_filename(_root_path, info->filename, NULL));
            }
            else if(g_strcmp0(status, "error") == 0)
            {
                g_print("[OBEX Server] Transfer failed\n");
            }
            else if(g_strcmp0(status, "queued") == 0)
            {
                g_print("[OBEX Server] Transfer queued\n");
            }
            else if(g_strcmp0(status, "suspended") == 0)
            {
                g_print("[OBEX Server] Transfer halted\n");
            }
            g_free(status);
        }
    }
    
    g_variant_unref(changed_properties);
}
