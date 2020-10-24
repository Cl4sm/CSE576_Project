static void
mcd_client_registry_list_activatable_names_cb (TpDBusDaemon *proxy,
    const gchar **names,
    const GError *error,
    gpointer user_data,
    GObject *weak_object)
{
  McdClientRegistry *self = MCD_CLIENT_REGISTRY (weak_object);

  if (error != NULL)
    {
      DEBUG ("ListActivatableNames returned error, assuming none: %s %d: %s",
          g_quark_to_string (error->domain), error->code, error->message);
    }
  else if (names != NULL)
    {
      const gchar **iter = names;

      DEBUG ("ListActivatableNames returned");

      while (*iter != NULL)
        {
          _mcd_client_registry_found_name (self, *iter, NULL, TRUE);
          iter++;
        }
    }

  /* paired with the lock taken when the McdClientRegistry was constructed */
  _mcd_client_registry_dec_startup_lock (self);
}