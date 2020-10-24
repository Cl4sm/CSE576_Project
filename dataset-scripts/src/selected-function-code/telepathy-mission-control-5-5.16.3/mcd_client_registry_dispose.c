static void
mcd_client_registry_dispose (GObject *object)
{
  McdClientRegistry *self = MCD_CLIENT_REGISTRY (object);
  void (*chain_up) (GObject *) =
    G_OBJECT_CLASS (_mcd_client_registry_parent_class)->dispose;

  if (self->priv->dbus_daemon != NULL)
    {
      DBusGConnection *gconn =
        tp_proxy_get_dbus_connection (self->priv->dbus_daemon);
      DBusConnection *dconn = dbus_g_connection_get_connection (gconn);

      dbus_connection_remove_filter (dconn,
          mcd_client_registry_name_owner_filter,
          self);
    }

  tp_clear_object (&self->priv->dbus_daemon);

  if (self->priv->clients != NULL)
    {
      g_hash_table_foreach (self->priv->clients,
          mcd_client_registry_disconnect_client_signals, self);

    }

  tp_clear_pointer (&self->priv->clients, g_hash_table_unref);

  if (chain_up != NULL)
    chain_up (object);
}