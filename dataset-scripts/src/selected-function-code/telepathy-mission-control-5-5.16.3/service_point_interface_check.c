static void
service_point_interface_check (TpConnection *tp_conn,
    const gchar **interfaces,
    const GError *error,
    gpointer data,
    GObject *connection)
{
  const gchar *interface;
  gboolean found = FALSE;
  gboolean watch = GPOINTER_TO_UINT (data);
  guint i = 0;

  if (interfaces == NULL)
    return;

  for (interface = interfaces[0];
       !found && !tp_str_empty (interface);
       interface = interfaces[++i])
    {
      if (!tp_strdiff (interface, TP_IFACE_CONNECTION_INTERFACE_SERVICE_POINT))
        found = TRUE;
    }

  if (!found)
    return;

  /* so we know if/when the service points change (eg the SIM might not be
   * accessible yet, in which case the call below won't return any entries)
   * check the flag though as we only want to do this once per connection:
   */
  if (watch)
    tp_cli_connection_interface_service_point_connect_to_service_points_changed
      (tp_conn, service_points_changed_cb, NULL, NULL, connection, NULL);

  /* fetch the current list to initialise our state */
  tp_cli_dbus_properties_call_get (tp_conn, -1,
      TP_IFACE_CONNECTION_INTERFACE_SERVICE_POINT,
      "KnownServicePoints", service_points_fetched_cb,
      NULL, NULL, connection);
}