static void
account_external_password_storage_identify_account_cb (TpProxy *protocol,
    const char *account_id,
    const GError *in_error,
    gpointer user_data,
    GObject *self)
{
  McdAccount *account = MCD_ACCOUNT (self);
  DBusGMethodInvocation *context = user_data;
  TpConnectionManager *cm = mcd_account_get_cm (account);

  if (in_error != NULL)
    {
      dbus_g_method_return_error (context, in_error);
      return;
    }

  DEBUG ("Identified account as %s", account_id);

  mc_cli_connection_manager_interface_account_storage_call_forget_credentials (
      cm, -1, account_id,
      account_external_password_storage_forget_credentials_cb,
      context, NULL, self);
}