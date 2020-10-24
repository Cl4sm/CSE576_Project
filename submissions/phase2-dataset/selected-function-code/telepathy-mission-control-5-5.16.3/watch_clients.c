static void
watch_clients (McdClientRegistry *self)
{
  TpDBusDaemon *dbus_daemon = self->priv->dbus_daemon;
  DBusGConnection *gconn = tp_proxy_get_dbus_connection (dbus_daemon);
  DBusConnection *dconn = dbus_g_connection_get_connection (gconn);
  DBusError error = { 0 };

#define MATCH_ITEM(t,x) #t "='" x "'"

#define NAME_OWNER_RULE \
    MATCH_ITEM (type,      "signal")            "," \
    MATCH_ITEM (sender,    DBUS_SERVICE_DBUS)   "," \
    MATCH_ITEM (interface, DBUS_INTERFACE_DBUS) "," \
    MATCH_ITEM (member,    "NameOwnerChanged")

#define CLIENT_MATCH_RULE \
    NAME_OWNER_RULE "," \
    MATCH_ITEM (arg0namespace, "org.freedesktop.Telepathy.Client")

  if (!dbus_connection_add_filter (dconn, mcd_client_registry_name_owner_filter,
        self, NULL))
    g_critical ("Could not add filter for NameOwnerChanged (out of memory?)");

  dbus_error_init (&error);

  dbus_bus_add_match (dconn, CLIENT_MATCH_RULE, &error);
  if (dbus_error_is_set (&error))
    {
      DEBUG ("Could not add client names match rule (D-Bus 1.6 required): %s",
          error.message);

      dbus_error_free (&error);

      dbus_bus_add_match (dconn, NAME_OWNER_RULE, &error);
      if (dbus_error_is_set (&error))
        {
          g_critical ("Could not add all dbus names match rule: %s",
              error.message);
          dbus_error_free (&error);
        }
    }
}