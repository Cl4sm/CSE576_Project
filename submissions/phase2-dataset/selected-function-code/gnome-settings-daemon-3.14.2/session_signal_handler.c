static void
session_signal_handler (GDBusConnection  *connection,
                        const gchar      *sender_name,
                        const gchar      *object_path,
                        const gchar      *interface_name,
                        const gchar      *signal_name,
                        GVariant         *parameters,
                        gpointer          user_data)
{
        guint            new_status;

        g_variant_get (parameters, "(u)", &new_status);

        if (new_status == PRESENCE_STATUS_IDLE ||
            new_status == PRESENCE_STATUS_AVAILABLE) {
                unregister_objects ();
                unown_names ();

                if (new_status == PRESENCE_STATUS_AVAILABLE) {
                        npn_owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                                                       SCP_DBUS_NPN_NAME,
                                                       G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
                                                       G_BUS_NAME_OWNER_FLAGS_REPLACE,
                                                       on_npn_bus_acquired,
                                                       on_name_acquired,
                                                       on_name_lost,
                                                       NULL,
                                                       NULL);

                        pdi_owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                                                       SCP_DBUS_PDI_NAME,
                                                       G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
                                                       G_BUS_NAME_OWNER_FLAGS_REPLACE,
                                                       on_pdi_bus_acquired,
                                                       on_name_acquired,
                                                       on_name_lost,
                                                       NULL,
                                                       NULL);
                }
        }
}