static void _bt_network_property_changed(GDBusConnection *connection, const gchar *sender_name, const gchar *object_path, const gchar *interface_name, const gchar *signal_name, GVariant *parameters, gpointer user_data)
{
        g_assert(user_data != NULL);
	GMainLoop *mainloop = user_data;

        GVariant *changed_properties = g_variant_get_child_value(parameters, 1);
        g_variant_lookup_value(changed_properties, "UUID", NULL);
	if (g_variant_lookup_value(changed_properties, "Connected", NULL))
        {
		if (g_variant_get_boolean(g_variant_lookup_value(changed_properties, "Connected", NULL)) == TRUE)
                {
			g_print("Network service is connected\n");
		}
                else
                {
			g_print("Network service is disconnected\n");
			g_main_loop_quit(mainloop);
		}
	}
        else if (g_variant_lookup_value(changed_properties, "Interface", NULL))
        {
		g_print("Interface: %s\n", g_variant_get_string(g_variant_lookup_value(changed_properties, "Interface", NULL), NULL));
	}
        else if (g_variant_lookup_value(changed_properties, "UUID", NULL))
        {
		g_print("UUID: %s (%s)\n", uuid2name(g_variant_get_string(g_variant_lookup_value(changed_properties, "UUID", NULL), NULL)), g_variant_get_string(g_variant_lookup_value(changed_properties, "UUID", NULL), NULL));
	}
}
