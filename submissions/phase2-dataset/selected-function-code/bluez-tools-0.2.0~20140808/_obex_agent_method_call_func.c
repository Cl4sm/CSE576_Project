{
    g_assert(user_data != NULL);
    ObexAgent *self = user_data;
    
    if (g_strcmp0(method_name, "AuthorizePush") == 0)
    {
        const gchar *transfer = g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL);
        if (intf_supported(OBEX_TRANSFER_DBUS_SERVICE, transfer, OBEX_TRANSFER_DBUS_INTERFACE))
        {
            ObexTransfer *transfer_t = obex_transfer_new(transfer);
            g_print("[Transfer Request]\n");
            g_print("  Name: %s\n", obex_transfer_get_name(transfer_t, NULL));
            g_print("  Size: %llu bytes\n", obex_transfer_get_size(transfer_t, NULL));
            // Filename seems to be always NULL
            // g_print("  Filename: %s\n", obex_transfer_get_filename(transfer_t, NULL));
            const gchar *filename = obex_transfer_get_name(transfer_t, NULL);
            const guint64 size = obex_transfer_get_size(transfer_t, NULL);
            g_object_unref(transfer_t);

            gchar yn[4] = {0,};
            g_print("Accept (yes/no)? ");
            errno = 0;
            if (scanf("%3s", yn) == EOF && errno)
            {
                g_warning("%s\n", strerror(errno));
            }
            if (g_strcmp0(yn, "y") == 0 || g_strcmp0(yn, "yes") == 0)
            {
                // IMPORTANT NOTE!
                // OBEX CANNOT WRITE FILES OUTSIDE OF /home/<user>/.cache/obexd
                // GVariant *ret = g_variant_new("(s)", g_build_filename(self->priv->root_folder, filename, NULL));
                // This will store the file in /home/<user>/.cache/obexd
                GVariant *ret = g_variant_new("(s)", filename);
                // g_print("invocation return value: %s\n", g_variant_print(ret, TRUE));
                
                // Call the callback to handle the filename and size
                if(self->priv->agent_approved_callback)
                    (*self->priv->agent_approved_callback)(self, transfer, filename, size, self->priv->approved_user_data);
                
                // Return string
                g_dbus_method_invocation_return_value(invocation, ret);
                return;
            }
            else
            {
                // Return error
                g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.obex.Error.Rejected", "File transfer rejected");
                return;
            }
        }
        else
        {
            g_print("Error: Unknown transfer request\n");
            // Return error
            g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.obex.Error.Rejected", "File transfer rejected");
            return;
        }
    }
    else if (g_strcmp0(method_name, "Cancel") == 0)
    {
        g_print("Request cancelled\n");
        // Return void
        g_dbus_method_invocation_return_value(invocation, NULL);
        return;
    }
    else if (g_strcmp0(method_name, "Release") == 0)
    {
        if (_update_progress)
        {
            g_print("\n");
            _update_progress = FALSE;
        }

        g_print("OBEXAgent released\n");

        (*self->priv->agent_released_callback)(self, self->priv->user_data);
        
        // Return void
        g_dbus_method_invocation_return_value(invocation, NULL);
        return;
    }
}
