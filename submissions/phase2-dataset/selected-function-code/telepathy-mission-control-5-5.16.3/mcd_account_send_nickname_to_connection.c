static void
mcd_account_send_nickname_to_connection (McdAccount *self,
    const gchar *nickname)
{
  if (self->priv->tp_connection == NULL)
    return;

  if (self->priv->self_contact == NULL)
    return;

  DEBUG ("%s: '%s'", self->priv->unique_name, nickname);

  if (tp_proxy_has_interface_by_id (self->priv->tp_connection,
          TP_IFACE_QUARK_CONNECTION_INTERFACE_ALIASING))
    {
      GHashTable *aliases = g_hash_table_new (NULL, NULL);

      g_hash_table_insert (aliases,
          GUINT_TO_POINTER (tp_contact_get_handle (self->priv->self_contact)),
          (gchar *) nickname);
      tp_cli_connection_interface_aliasing_call_set_aliases (
          self->priv->tp_connection, -1, aliases,
          mcd_account_set_self_alias_cb, NULL, NULL, NULL);
      g_hash_table_unref (aliases);
    }
}