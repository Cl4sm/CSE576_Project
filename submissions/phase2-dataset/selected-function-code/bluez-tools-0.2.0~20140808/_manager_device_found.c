{
    g_assert(user_data != NULL);
    const gchar *adapter_object_path = user_data;

    GVariant *arg0 = g_variant_get_child_value(parameters, 0);
    const gchar *str_object_path = g_variant_get_string(arg0, NULL);
    g_variant_unref(arg0);

    if (!g_str_has_prefix(str_object_path, adapter_object_path))
        return;

    GVariant *interfaces_and_properties = g_variant_get_child_value(parameters, 1);
    GVariant *properties = NULL;
    if (g_variant_lookup(interfaces_and_properties, DEVICE_DBUS_INTERFACE, "@a{sv}", &properties))
    {
        g_print("[%s]\n", g_variant_get_string(g_variant_lookup_value(properties, "Address", NULL), NULL));
        g_print("  Name: %s\n", g_variant_lookup_value(properties, "Name", NULL) != NULL ? g_variant_get_string(g_variant_lookup_value(properties, "Name", NULL), NULL) : NULL);
        g_print("  Alias: %s\n", g_variant_lookup_value(properties, "Alias", NULL) != NULL ? g_variant_get_string(g_variant_lookup_value(properties, "Alias", NULL), NULL) : NULL);
        g_print("  Address: %s\n", g_variant_lookup_value(properties, "Address", NULL) != NULL ? g_variant_get_string(g_variant_lookup_value(properties, "Address", NULL), NULL) : NULL);
        g_print("  Icon: %s\n", g_variant_lookup_value(properties, "Icon", NULL) != NULL ? g_variant_get_string(g_variant_lookup_value(properties, "Icon", NULL), NULL) : NULL);
        g_print("  Class: 0x%x\n", g_variant_lookup_value(properties, "Class", NULL) != NULL ? g_variant_get_uint32(g_variant_lookup_value(properties, "Class", NULL)) : 0x0);
        g_print("  LegacyPairing: %d\n", g_variant_lookup_value(properties, "LegacyPairing", NULL) != NULL ? g_variant_get_boolean(g_variant_lookup_value(properties, "LegacyPairing", NULL)) : FALSE);
        g_print("  Paired: %d\n", g_variant_lookup_value(properties, "Paired", NULL) != NULL ? g_variant_get_boolean(g_variant_lookup_value(properties, "Paired", NULL)) : FALSE);
        g_print("  RSSI: %d\n", g_variant_lookup_value(properties, "RSSI", NULL) != NULL ? g_variant_get_int16(g_variant_lookup_value(properties, "RSSI", NULL)) : 0x0);
        g_print("\n");

        g_variant_unref(properties);
    }
    g_variant_unref(interfaces_and_properties);
}
