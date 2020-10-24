static void
names_listed (GObject      *object,
              GAsyncResult *result,
              gpointer      user_data)
{
  NamespaceWatcher *watcher;
  GError *error = NULL;
  GVariant *reply;
  GVariantIter *iter;
  const gchar *name;

  reply = g_dbus_connection_call_finish (G_DBUS_CONNECTION (object), result, &error);

  if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
    {
      g_error_free (error);
      return;
    }

  watcher = user_data;

  if (reply == NULL)
    {
      g_warning ("bus_watch_namespace: error calling org.freedesktop.DBus.ListNames: %s", error->message);
      g_error_free (error);
      return;
    }

  g_variant_get (reply, "(as)", &iter);
  while (g_variant_iter_next (iter, "&s", &name))
    {
      if (dbus_name_has_namespace (name, watcher->name_space))
        {
          GetNameOwnerData *data = g_slice_new (GetNameOwnerData);
          data->watcher = watcher;
          data->name = g_strdup (name);
          g_dbus_connection_call (watcher->connection, "org.freedesktop.DBus", "/",
                                  "org.freedesktop.DBus", "GetNameOwner",
                                  g_variant_new ("(s)", name), G_VARIANT_TYPE ("(s)"),
                                  G_DBUS_CALL_FLAGS_NONE, -1, watcher->cancellable,
                                  got_name_owner, data);
        }
    }

  g_variant_iter_free (iter);
  g_variant_unref (reply);
}