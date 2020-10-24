guint
bus_watch_namespace (GBusType                  bus_type,
                     const gchar              *name_space,
                     GBusNameAppearedCallback  appeared_handler,
                     GBusNameVanishedCallback  vanished_handler,
                     gpointer                  user_data,
                     GDestroyNotify            user_data_destroy)
{
  NamespaceWatcher *watcher;

  /* same rules for interfaces and well-known names */
  g_return_val_if_fail (name_space != NULL && g_dbus_is_interface_name (name_space), 0);
  g_return_val_if_fail (appeared_handler || vanished_handler, 0);

  watcher = g_new0 (NamespaceWatcher, 1);
  watcher->id = namespace_watcher_next_id++;
  watcher->name_space = g_strdup (name_space);
  watcher->appeared_handler = appeared_handler;
  watcher->vanished_handler = vanished_handler;
  watcher->user_data = user_data;
  watcher->user_data_destroy = user_data_destroy;
  watcher->cancellable = g_cancellable_new ();;
  watcher->names = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);

  if (namespace_watcher_watchers == NULL)
    namespace_watcher_watchers = g_hash_table_new (g_direct_hash, g_direct_equal);
  g_hash_table_insert (namespace_watcher_watchers, GUINT_TO_POINTER (watcher->id), watcher);

  g_bus_get (bus_type, watcher->cancellable, got_bus, watcher);

  return watcher->id;
}