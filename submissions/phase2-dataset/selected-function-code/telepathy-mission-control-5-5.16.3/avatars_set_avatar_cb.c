static void
avatars_set_avatar_cb (TpConnection *tp_connection,
    const gchar *token,
    const GError *error,
    gpointer user_data,
    GObject *weak_object)
{
  McdAccount *self = MCD_ACCOUNT (weak_object);

  self->priv->setting_avatar = FALSE;

  if (error != NULL)
    {
      DEBUG ("%s: %s", self->priv->unique_name, error->message);
    }
  else
    {
      DEBUG ("%s: new token %s", self->priv->unique_name, token);
      _mcd_account_set_avatar_token (self, token);
    }
}