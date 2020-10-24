static void
account_external_password_storage_properties_changed_cb (TpProxy *cm,
    const char *iface,
    GHashTable *changed_properties,
    const char **invalidated_properties,
    gpointer user_data,
    GObject *self)
{
  McdAccount *account = MCD_ACCOUNT (self);
  TpProtocol *protocol = tp_connection_manager_get_protocol_object (
      TP_CONNECTION_MANAGER (cm), account->priv->protocol_name);
  GHashTable *params;

  if (tp_strdiff (iface,
        MC_IFACE_CONNECTION_MANAGER_INTERFACE_ACCOUNT_STORAGE))
    return;

  /* look up account identity so we can look up our value in
   * the Accounts map */
  params = _mcd_account_dup_parameters (account);
  tp_cli_protocol_call_identify_account (protocol, -1, params,
      account_setup_identify_account_cb,
      NULL, NULL, G_OBJECT (account));

  g_hash_table_unref (params);
}