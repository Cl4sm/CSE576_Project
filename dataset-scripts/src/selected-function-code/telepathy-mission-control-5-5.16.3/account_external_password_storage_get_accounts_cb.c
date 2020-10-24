static void
account_external_password_storage_get_accounts_cb (TpProxy *cm,
    const GValue *value,
    const GError *in_error,
    gpointer user_data,
    GObject *self)
{
  McdAccount *account = MCD_ACCOUNT (self);
  const char *account_id = user_data;
  GHashTable *map, *props;

  if (in_error != NULL)
    {
      DEBUG ("Failed to get Account property: %s", in_error->message);
      return;
    }

  g_return_if_fail (G_VALUE_HOLDS (value, MC_HASH_TYPE_ACCOUNT_FLAGS_MAP));

  map = g_value_get_boxed (value);

  account->priv->password_saved =
    GPOINTER_TO_UINT (g_hash_table_lookup (map, account_id)) &
      MC_ACCOUNT_FLAG_CREDENTIALS_STORED;

  DEBUG ("PasswordSaved = %u", account->priv->password_saved);

  /* emit the changed signal */
  props = tp_asv_new (
      "PasswordSaved", G_TYPE_BOOLEAN, account->priv->password_saved,
      NULL);

  tp_svc_dbus_properties_emit_properties_changed (account,
      MC_IFACE_ACCOUNT_INTERFACE_EXTERNAL_PASSWORD_STORAGE,
      props,
      NULL);

  g_hash_table_unref (props);
}