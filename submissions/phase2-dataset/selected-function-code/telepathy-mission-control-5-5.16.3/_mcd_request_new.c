McdRequest *
_mcd_request_new (McdClientRegistry *clients,
    gboolean use_existing,
    McdAccount *account,
    GHashTable *properties,
    gint64 user_action_time,
    const gchar *preferred_handler,
    GHashTable *hints)
{
  McdRequest *self;

  self = g_object_new (MCD_TYPE_REQUEST,
      "client-registry", clients,
      "use-existing", use_existing,
      "account", account,
      "properties", properties,
      "user-action-time", user_action_time,
      "preferred-handler", preferred_handler,
      "hints", hints,
      NULL);
  DEBUG ("%p (for %p)", self, account);

  return self;
}