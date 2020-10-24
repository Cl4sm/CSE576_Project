static void
namespace_watcher_stop (gpointer data)
{
  NamespaceWatcher *watcher = data;

  g_cancellable_cancel (watcher->cancellable);
  g_object_unref (watcher->cancellable);

  if (watcher->subscription_id)
    g_dbus_connection_signal_unsubscribe (watcher->connection, watcher->subscription_id);

  if (watcher->vanished_handler)
    {
      GHashTableIter it;
      const gchar *name;

      g_hash_table_iter_init (&it, watcher->names);
      while (g_hash_table_iter_next (&it, (gpointer *) &name, NULL))
        watcher->vanished_handler (watcher->connection, name, watcher->user_data);
    }

  if (watcher->user_data_destroy)
    watcher->user_data_destroy (watcher->user_data);

  if (watcher->connection)
    {
      g_signal_handlers_disconnect_by_func (watcher->connection, namespace_watcher_stop, watcher);
      g_object_unref (watcher->connection);
    }

  g_hash_table_unref (watcher->names);

  g_hash_table_remove (namespace_watcher_watchers, GUINT_TO_POINTER (watcher->id));
  if (g_hash_table_size (namespace_watcher_watchers) == 0)
    g_clear_pointer (&namespace_watcher_watchers, g_hash_table_destroy);

  g_free (watcher);
}