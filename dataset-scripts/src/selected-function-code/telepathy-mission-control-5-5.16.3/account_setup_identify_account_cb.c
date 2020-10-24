static void
account_setup_identify_account_cb (TpProxy *protocol,
    const char *account_id,
    const GError *in_error,
    gpointer user_data,
    GObject *self)
{
  McdAccount *account = MCD_ACCOUNT (self);
  TpConnectionManager *cm = mcd_account_get_cm (account);

  if (in_error != NULL)
    {
      DEBUG ("Error identifying account: %s", in_error->message);
      return;
    }

  DEBUG ("Identified account as %s", account_id);

  /* look up the current value of the CM.I.AS.Accounts property
   * and monitor future changes */
  tp_cli_dbus_properties_call_get (cm, -1,
      MC_IFACE_CONNECTION_MANAGER_INTERFACE_ACCOUNT_STORAGE,
      "Accounts",
      account_external_password_storage_get_accounts_cb,
      g_strdup (account_id), g_free, G_OBJECT (account));
}