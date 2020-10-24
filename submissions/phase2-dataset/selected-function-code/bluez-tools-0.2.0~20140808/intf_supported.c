{
    g_assert(dbus_service_name != NULL && strlen(dbus_service_name) > 0);
    g_assert(dbus_object_path != NULL && strlen(dbus_object_path) > 0);
    g_assert(intf_name != NULL && strlen(intf_name) > 0);

    gboolean supported = FALSE;
    GDBusConnection *conn = NULL;

    if (g_strcmp0(dbus_service_name, BLUEZ_DBUS_SERVICE_NAME) == 0)
    {
        conn = system_conn;
    }
    else if (g_strcmp0(dbus_service_name, BLUEZ_OBEX_DBUS_SERVICE_NAME) == 0)
    {
        conn = session_conn;
    }
    else
    {
        return FALSE;
    }
    g_assert(conn != NULL);

    gchar *check_intf_regex_str = g_strconcat("<interface name=\"", intf_name, "\">", NULL);

    /* Getting introspection XML */
    GError *error = NULL;
    GDBusProxy *introspection_proxy = g_dbus_proxy_new_sync(conn, G_DBUS_PROXY_FLAGS_NONE, NULL, dbus_service_name, dbus_object_path, "org.freedesktop.DBus.Introspectable", NULL, &error);
    g_assert(introspection_proxy != NULL);
    GVariant *introspection_ret = g_dbus_proxy_call_sync(introspection_proxy, "Introspect", NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
    gchar *introspection_xml = NULL;
    if (!introspection_ret)
    {
#if 0
        g_critical("%s", error->message);
#else
        g_error_free(error);
        error = NULL;
        introspection_xml = g_strdup("null");
#endif
    }
    else
    {
        introspection_xml = (gchar *) g_variant_get_string(g_variant_get_child_value(introspection_ret, 0), NULL);
    }
    g_assert(error == NULL);
    
    if (g_regex_match_simple(check_intf_regex_str, introspection_xml, 0, 0))
    {
        supported = TRUE;
    }

    g_free(check_intf_regex_str);
    g_free(introspection_xml);
    if(introspection_ret)
        g_variant_unref(introspection_ret);
    g_object_unref(introspection_proxy);

    return supported;
}
