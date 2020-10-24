int
main (int argc, char *argv[])
{
  GDBusConnection *connection;
  gboolean         client_signal_subscription_set = FALSE;
  GError          *error = NULL;
  guint            client_signal_subscription_id;
  guint            session_signal_subscription_id;
  gchar           *object_path;

  bindtextdomain (GETTEXT_PACKAGE, GNOME_SETTINGS_LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
  setlocale (LC_ALL, "");

  npn_registration_id = 0;
  pdi_registration_id = 0;
  npn_owner_id = 0;
  pdi_owner_id = 0;

  notify_init ("gnome-settings-daemon-printer");

  npn_introspection_data =
          g_dbus_node_info_new_for_xml (npn_introspection_xml, &error);

  if (npn_introspection_data == NULL) {
          g_warning ("Error parsing introspection XML: %s\n", error->message);
          g_error_free (error);
          goto error;
  }

  pdi_introspection_data =
          g_dbus_node_info_new_for_xml (pdi_introspection_xml, &error);

  if (pdi_introspection_data == NULL) {
          g_warning ("Error parsing introspection XML: %s\n", error->message);
          g_error_free (error);
          goto error;
  }

  connection = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &error);

  session_signal_subscription_id =
    g_dbus_connection_signal_subscribe (connection,
                                        NULL,
                                        GNOME_SESSION_PRESENCE_DBUS_IFACE,
                                        "StatusChanged",
                                        GNOME_SESSION_PRESENCE_DBUS_PATH,
                                        NULL,
                                        G_DBUS_SIGNAL_FLAGS_NONE,
                                        session_signal_handler,
                                        NULL,
                                        NULL);

  object_path = register_gnome_session_client ("gsd-printer", "");
  if (object_path) {
          client_signal_subscription_id =
                  g_dbus_connection_signal_subscribe (connection,
                                                      NULL,
                                                      GNOME_SESSION_CLIENT_PRIVATE_DBUS_IFACE,
                                                      NULL,
                                                      object_path,
                                                      NULL,
                                                      G_DBUS_SIGNAL_FLAGS_NONE,
                                                      client_signal_handler,
                                                      NULL,
                                                      NULL);
          client_signal_subscription_set = TRUE;
  }

  if (npn_owner_id == 0)
          npn_owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                                         SCP_DBUS_NPN_NAME,
                                         G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
                                         G_BUS_NAME_OWNER_FLAGS_REPLACE,
                                         on_npn_bus_acquired,
                                         on_name_acquired,
                                         on_name_lost,
                                         NULL,
                                         NULL);

  if (pdi_owner_id == 0)
          pdi_owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                                         SCP_DBUS_PDI_NAME,
                                         G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
                                         G_BUS_NAME_OWNER_FLAGS_REPLACE,
                                         on_pdi_bus_acquired,
                                         on_name_acquired,
                                         on_name_lost,
                                         NULL,
                                         NULL);

  main_loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (main_loop);

  unregister_objects ();
  unown_names ();

  if (client_signal_subscription_set)
          g_dbus_connection_signal_unsubscribe (connection, client_signal_subscription_id);
  g_dbus_connection_signal_unsubscribe (connection, session_signal_subscription_id);

  g_free (object_path);

  g_dbus_node_info_unref (npn_introspection_data);
  g_dbus_node_info_unref (pdi_introspection_data);

  return 0;

error:

  if (npn_introspection_data)
          g_dbus_node_info_unref (npn_introspection_data);

  if (pdi_introspection_data)
          g_dbus_node_info_unref (pdi_introspection_data);

  return 1;
}