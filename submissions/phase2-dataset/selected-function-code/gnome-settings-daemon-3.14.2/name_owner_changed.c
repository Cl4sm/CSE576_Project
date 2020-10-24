static void
name_owner_changed (GDBusConnection *connection,
                    const gchar     *sender_name,
                    const gchar     *object_path,
                    const gchar     *interface_name,
                    const gchar     *signal_name,
                    GVariant        *parameters,
                    gpointer         user_data)
{
  NamespaceWatcher *watcher = user_data;
  const gchar *name;
  const gchar *old_owner;
  const gchar *new_owner;

  g_variant_get (parameters, "(&s&s&s)", &name, &old_owner, &new_owner);

  if (old_owner[0] != '\0')
    namespace_watcher_name_vanished (watcher, name);

  if (new_owner[0] != '\0')
    namespace_watcher_name_appeared (watcher, name, new_owner);
}