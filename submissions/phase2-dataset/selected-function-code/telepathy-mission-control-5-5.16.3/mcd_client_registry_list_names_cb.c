static void
mcd_client_registry_list_names_cb (TpDBusDaemon *proxy,
    const gchar **names,
    const GError *error,
    gpointer user_data,
    GObject *weak_object)
{
  McdClientRegistry *self = MCD_CLIENT_REGISTRY (weak_object);

  if (error != NULL)
    {
      DEBUG ("ListNames returned error, assuming none: %s %d: %s",
          g_quark_to_string (error->domain), error->code, error->message);
    }
  else if (names != NULL)
    {
      const gchar **iter = names;

      DEBUG ("ListNames returned");

      while (*iter != NULL)
        {
          _mcd_client_registry_found_name (self, *iter, NULL, FALSE);
          iter++;
        }
    }

  tp_cli_dbus_daemon_call_list_activatable_names (proxy, -1,
      mcd_client_registry_list_activatable_names_cb,
      NULL, NULL, weak_object);
  /* deliberately not calling _mcd_client_registry_dec_startup_lock here -
   * this function is "lock-neutral", similarly to list_names_cb (we would
   * take a lock for ListActivatableNames then release the one used for
   * ReloadConfig), so simplify by doing nothing */
}