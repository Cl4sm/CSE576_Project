static void
got_bus (GObject      *object,
         GAsyncResult *result,
         gpointer      user_data)
{
  GDBusConnection *connection;
  NamespaceWatcher *watcher;
  GError *error = NULL;

  connection = g_bus_get_finish (result, &error);

  if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
    {
      g_error_free (error);
      return;
    }

  watcher = user_data;

  if (connection == NULL)
    {
      namespace_watcher_stop (watcher);
      return;
    }

  watcher->connection = connection;
  g_signal_connect (watcher->connection, "closed", G_CALLBACK (connection_closed), watcher);

  watcher->subscription_id =
    g_dbus_connection_signal_subscribe (watcher->connection, "org.freedesktop.DBus",
                                        "org.freedesktop.DBus", "NameOwnerChanged", "/org/freedesktop/DBus",
                                        watcher->name_space, G_DBUS_SIGNAL_FLAGS_MATCH_ARG0_NAMESPACE,
                                        name_owner_changed, watcher, NULL);

  g_dbus_connection_call (watcher->connection, "org.freedesktop.DBus", "/",
                          "org.freedesktop.DBus", "ListNames", NULL, G_VARIANT_TYPE ("(as)"),
                          G_DBUS_CALL_FLAGS_NONE, -1, watcher->cancellable,
                          names_listed, watcher);
}