static void
got_name_owner (GObject      *object,
                GAsyncResult *result,
                gpointer      user_data)
{
  GetNameOwnerData *data = user_data;
  GError *error = NULL;
  GVariant *reply;
  const gchar *owner;

  reply = g_dbus_connection_call_finish (G_DBUS_CONNECTION (object), result, &error);

  if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
    {
      g_error_free (error);
      goto out;
    }

  if (reply == NULL)
    {
      if (!g_error_matches (error, G_DBUS_ERROR, G_DBUS_ERROR_NAME_HAS_NO_OWNER))
        g_warning ("bus_watch_namespace: error calling org.freedesktop.DBus.GetNameOwner: %s", error->message);
      g_error_free (error);
      goto out;
    }

  g_variant_get (reply, "(&s)", &owner);
  namespace_watcher_name_appeared (data->watcher, data->name, owner);

  g_variant_unref (reply);

out:
  g_free (data->name);
  g_slice_free (GetNameOwnerData, data);
}