static void
account_external_password_storage_forget_password (
    McSvcAccountInterfaceExternalPasswordStorage *self,
    DBusGMethodInvocation *context)
{
  McdAccount *account = MCD_ACCOUNT (self);
  TpConnectionManager *cm = mcd_account_get_cm (account);
  TpProtocol *protocol;
  GHashTable *params;

  /* do we support the interface */
  if (!tp_proxy_has_interface_by_id (cm,
          MC_IFACE_QUARK_CONNECTION_MANAGER_INTERFACE_ACCOUNT_STORAGE))
    {
      GError *error = g_error_new (TP_ERROR, TP_ERROR_NOT_IMPLEMENTED,
          "CM for this Account does not implement AccountStorage iface");

      dbus_g_method_return_error (context, error);
      g_error_free (error);

      return;
    }

  /* identify the account */
  protocol = tp_connection_manager_get_protocol_object (cm,
      account->priv->protocol_name);
  params = _mcd_account_dup_parameters (account);

  tp_cli_protocol_call_identify_account (protocol, -1, params,
      account_external_password_storage_identify_account_cb,
      context, NULL, G_OBJECT (self));

  g_hash_table_unref (params);
}