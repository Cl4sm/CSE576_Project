static void
_mcd_client_registry_found_name (McdClientRegistry *self,
    const gchar *well_known_name,
    const gchar *unique_name_if_known,
    gboolean activatable)
{
  McdClientProxy *client;

  if (!g_str_has_prefix (well_known_name, TP_CLIENT_BUS_NAME_BASE))
    {
      /* This is not a Telepathy Client */
      return;
    }

  if (!_mcd_client_check_valid_name (
        well_known_name + MC_CLIENT_BUS_NAME_BASE_LEN, NULL))
    {
      /* This is probably meant to be a Telepathy Client, but it's not */
      DEBUG ("Ignoring invalid Client name: %s",
          well_known_name + MC_CLIENT_BUS_NAME_BASE_LEN);
      return;
    }

  client = g_hash_table_lookup (self->priv->clients, well_known_name);

  if (client != NULL)
    {
      if (activatable)
        {
          /* We already knew that it was active, but now we also know that
           * it is activatable */
          _mcd_client_proxy_set_activatable (client);
        }
      else if (unique_name_if_known != NULL)
        {
          /* We already knew that it was activatable, but now we also know
           * that it is active */
          _mcd_client_proxy_set_active (client, unique_name_if_known);
        }

      return;
    }

  DEBUG ("Registering client %s", well_known_name);

  client = _mcd_client_proxy_new (self->priv->dbus_daemon,
      well_known_name, unique_name_if_known, activatable);
  g_hash_table_insert (self->priv->clients, g_strdup (well_known_name),
      client);

  /* paired with one in mcd_client_registry_ready_cb, when the
   * McdClientProxy is ready */
  _mcd_client_registry_inc_startup_lock (self);

  g_signal_connect (client, "ready",
                    G_CALLBACK (mcd_client_registry_ready_cb),
                    self);

  g_signal_connect (client, "gone",
                    G_CALLBACK (mcd_client_registry_gone_cb),
                    self);

  g_signal_emit (self, signals[S_CLIENT_ADDED], 0, client);
}