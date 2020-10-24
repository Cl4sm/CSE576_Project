static void
proxy_new_cb (GObject *source,
    GAsyncResult *result,
    gpointer user_data)
{
  McdSlacker *self = user_data;
  GVariant *prop;
  GError *error = NULL;

  self->priv->proxy = g_dbus_proxy_new_finish (result, &error);
  if (self->priv->proxy == NULL)
    {
      DEBUG ("Error while creating slacker proxy: %s", error->message);
      goto out;
    }

  g_signal_connect (self->priv->proxy, "g-signal",
      G_CALLBACK (signal_cb), self);

  prop = g_dbus_proxy_get_cached_property (self->priv->proxy, SERVICE_PROP_NAME);

  if (g_dbus_proxy_get_name_owner (self->priv->proxy) == NULL)
    {
      DEBUG ("%s service not found", SERVICE_NAME);
    }
  else if (prop == NULL)
    {
      DEBUG ("%s.%s property is missing", SERVICE_INTERFACE, SERVICE_PROP_NAME);
    }
  else
    {
      status_changed (self, prop);
      g_variant_unref (prop);
    }

out:
  g_object_unref (self);
}