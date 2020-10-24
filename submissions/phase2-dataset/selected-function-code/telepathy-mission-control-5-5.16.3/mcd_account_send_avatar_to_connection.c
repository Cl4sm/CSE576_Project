static void
mcd_account_send_avatar_to_connection (McdAccount *self,
    const GArray *avatar,
    const gchar *mime_type)
{
  if (self->priv->tp_connection == NULL)
    return;

  if (self->priv->self_contact == NULL)
    return;

  DEBUG ("%s: %u bytes", self->priv->unique_name, avatar->len);

  if (tp_proxy_has_interface_by_id (self->priv->tp_connection,
          TP_IFACE_QUARK_CONNECTION_INTERFACE_AVATARS))
    {
      self->priv->setting_avatar = TRUE;

      if (avatar->len > 0 && avatar->len < G_MAXUINT)
        {
          tp_cli_connection_interface_avatars_call_set_avatar (
            self->priv->tp_connection, -1, avatar, mime_type,
            avatars_set_avatar_cb, NULL, NULL, (GObject *) self);
        }
      else
        {
          tp_cli_connection_interface_avatars_call_clear_avatar (
              self->priv->tp_connection, -1, avatars_clear_avatar_cb,
              NULL, NULL, (GObject *) self);
        }
    }
  else
    {
      DEBUG ("unsupported, ignoring");
    }
}