static void _bt_agent_method_call_func(GDBusConnection *connection, const gchar *sender, const gchar *object_path, const gchar *interface_name, const gchar *method_name, GVariant *parameters, GDBusMethodInvocation *invocation, gpointer user_data)
{
    // g_print("%s%s\n", method_name, g_variant_print(parameters, FALSE));
    
    if (g_strcmp0(method_name, "AuthorizeService") == 0)
    {
        // Return void
        g_dbus_method_invocation_return_value(invocation, NULL);
    }
    else if (g_strcmp0(method_name, "Cancel") == 0)
    {
        if (_interactive)
            g_print("Request canceled\n");
        // Return void
        g_dbus_method_invocation_return_value(invocation, NULL);
    }
    else if (g_strcmp0(method_name, "DisplayPasskey") == 0)
    {
        GError *error = NULL;
        Device *device_obj = device_new(g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL));
        const gchar *pin = _find_device_pin(device_get_dbus_object_path(device_obj));
        
        if (_interactive)
            g_print("Device: %s (%s)\n", device_get_alias(device_obj, &error), device_get_address(device_obj, &error));
        
        if (error)
        {
            g_critical("Failed to get remote device's MAC address: %s", error->message);
            g_error_free(error);
        }
        
        g_object_unref(device_obj);
        
        if (_interactive)
        {
            g_print("Passkey: %u, entered: %u\n", g_variant_get_uint32(g_variant_get_child_value(parameters, 1)), g_variant_get_uint16(g_variant_get_child_value(parameters, 2)));
            g_dbus_method_invocation_return_value(invocation, NULL);
            return;
        }
        else if (pin != NULL)
        {
            /* OK, device found */
            g_dbus_method_invocation_return_value(invocation, NULL);
            return;
        }
        
        g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "Pairing rejected");
    }
    else if (g_strcmp0(method_name, "DisplayPinCode") == 0)
    {
        GError *error = NULL;
        Device *device_obj = device_new(g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL));
        const gchar *pin = _find_device_pin(device_get_dbus_object_path(device_obj));
        const gchar *pincode = g_variant_get_string(g_variant_get_child_value(parameters, 1), NULL);
        
        if (_interactive)
            g_print("Device: %s (%s)\n", device_get_alias(device_obj, &error), device_get_address(device_obj, &error));
        
        if (error)
        {
            g_critical("Failed to get remote device's MAC address: %s", error->message);
            g_error_free(error);
        }
        
        g_object_unref(device_obj);
        
        /* Try to use found PIN */
	if (pin != NULL)
        {
            if (g_strcmp0(pin, "*") == 0 || g_strcmp0(pin, pincode) == 0)
            {
                if (_interactive)
                    g_print("Pin code confirmed\n");
                g_dbus_method_invocation_return_value(invocation, NULL);
            }
            else
                g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "Passkey does not match");
            
            return;
        }
        else if (_interactive)
        {
            g_print("Confirm pin code: %s (yes/no)? ", pincode);
            
            gchar yn[4] = {0,};
            errno = 0;
            if (scanf("%3s", yn) == EOF && errno)
                g_warning("%s\n", strerror(errno));
            if(g_ascii_strcasecmp(yn, "yes") == 0 || g_ascii_strcasecmp(yn, "y") == 0)
                g_dbus_method_invocation_return_value(invocation, NULL);
            else
                g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "Passkey does not match");
            return;
        }
        
        g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "Pairing rejected");
    }
    else if (g_strcmp0(method_name, "Release") == 0)
    {
        agent_need_unregister = FALSE;
        
        if(_mainloop)
            if (g_main_loop_is_running(_mainloop))
                g_main_loop_quit(_mainloop);
        
	g_print("Agent released\n");
        
        // Return void
        g_dbus_method_invocation_return_value(invocation, NULL);
    }
    else if (g_strcmp0(method_name, "RequestAuthorization") == 0)
    {
        GError *error = NULL;
        Device *device_obj = device_new(g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL));
        
        if (_interactive)
            g_print("Device: %s (%s)\n", device_get_alias(device_obj, &error), device_get_address(device_obj, &error));
        
        if(error)
        {
            g_critical("Failed to get remote device's MAC address: %s", error->message);
            g_error_free(error);
        }
        
        g_object_unref(device_obj);
        
        if (_interactive)
        {
            g_print("Authorize this device pairing (yes/no)? ");
            gchar yn[4] = {0,};
            errno = 0;
            if (scanf("%3s", yn) == EOF && errno)
                g_warning("%s\n", strerror(errno));
            if(g_ascii_strcasecmp(yn, "yes") == 0 || g_ascii_strcasecmp(yn, "y") == 0)
                g_dbus_method_invocation_return_value(invocation, NULL);
            else
                g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "Pairing rejected");
            return;
        }
        
        g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "Pairing rejected");
    }
    else if (g_strcmp0(method_name, "RequestConfirmation") == 0)
    {
        GError *error = NULL;
        Device *device_obj = device_new(g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL));
        guint32 passkey = g_variant_get_uint32(g_variant_get_child_value(parameters, 1));
        const gchar *pin = _find_device_pin(device_get_dbus_object_path(device_obj));
        
        if (_interactive)
            g_print("Device: %s (%s)\n", device_get_alias(device_obj, &error), device_get_address(device_obj, &error));
        
        if(error)
        {
            g_critical("Failed to get remote device's MAC address: %s", error->message);
            g_error_free(error);
        }
        
        g_object_unref(device_obj);
        
        /* Try to use found PIN */
	if (pin != NULL)
        {
            guint32 passkey_t;
            sscanf(pin, "%u", &passkey_t);
            
            if (g_strcmp0(pin, "*") == 0 || passkey_t == passkey)
            {
                if (_interactive)
                    g_print("Passkey confirmed\n");
                g_dbus_method_invocation_return_value(invocation, NULL);
            }
            else
                g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "Passkey does not match");
            
            return;
        }
        else if (_interactive)
        {
            g_print("Confirm passkey: %u (yes/no)? ", passkey);
            gchar yn[4] = {0,};
            errno = 0;
            if (scanf("%3s", yn) == EOF && errno)
                g_warning("%s\n", strerror(errno));
            if(g_ascii_strcasecmp(yn, "yes") == 0 || g_ascii_strcasecmp(yn, "y") == 0)
                g_dbus_method_invocation_return_value(invocation, NULL);
            else
                g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "Passkey does not match");
            return;
        }
        
        g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "Passkey does not match");
    }
    else if (g_strcmp0(method_name, "RequestPasskey") == 0)
    {
        GError *error = NULL;
        Device *device_obj = device_new(g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL));
        const gchar *pin = _find_device_pin(device_get_dbus_object_path(device_obj));
        guint32 ret = 0;
        
        if (_interactive)
            g_print("Device: %s (%s)\n", device_get_alias(device_obj, &error), device_get_address(device_obj, &error));
        
        if(error)
        {
            g_critical("Failed to get remote device's MAC address: %s", error->message);
            g_error_free(error);
        }
        
        g_object_unref(device_obj);
        
        /* Try to use found PIN */
	if (pin != NULL)
        {
            if (_interactive)
                g_print("Passkey found\n");
            sscanf(pin, "%u", &ret);
            g_dbus_method_invocation_return_value(invocation, g_variant_new_uint32(ret));
            return;
	}
        else if (_interactive)
        {
            g_print("Enter passkey: ");
            errno = 0;
            if (scanf("%u", &ret) == EOF && errno)
                g_warning("%s\n", strerror(errno));
            g_dbus_method_invocation_return_value(invocation, g_variant_new_uint32(ret));
            return;
	}
        
        g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "No passkey inputted");
    }
    else if (g_strcmp0(method_name, "RequestPinCode") == 0)
    {
        GError *error = NULL;
        Device *device_obj = device_new(g_variant_get_string(g_variant_get_child_value(parameters, 0), NULL));
        const gchar *pin = _find_device_pin(device_get_dbus_object_path(device_obj));
        const gchar *ret = NULL;
        
        if (_interactive)
            g_print("Device: %s (%s)\n", device_get_alias(device_obj, &error), device_get_address(device_obj, &error));
        
        if(error)
        {
            g_critical("Failed to get remote device's MAC address: %s", error->message);
            g_error_free(error);
        }
        
        g_object_unref(device_obj);
        
        /* Try to use found PIN */
	if (pin != NULL)
        {
            if (_interactive)
                g_print("Passkey found\n");
            sscanf(pin, "%s", &ret);
            g_dbus_method_invocation_return_value(invocation, g_variant_new_string(ret));
            return;
	}
        else if (_interactive)
        {
            g_print("Enter passkey: ");
            errno = 0;
            if (scanf("%s", &ret) == EOF && errno)
                g_warning("%s\n", strerror(errno));
            g_dbus_method_invocation_return_value(invocation, g_variant_new_string(ret));
            return;
	}
        
        g_dbus_method_invocation_return_dbus_error(invocation, "org.bluez.Error.Rejected", "No passkey inputted");
    }
}
