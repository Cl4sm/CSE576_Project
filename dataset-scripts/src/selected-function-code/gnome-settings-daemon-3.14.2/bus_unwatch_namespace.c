void
bus_unwatch_namespace (guint id)
{
  /* namespace_watcher_stop() might have already removed the watcher
   * with @id in the case of a connection error. Thus, this function
   * doesn't warn when @id is absent from the hash table.
   */

  if (namespace_watcher_watchers)
    {
      NamespaceWatcher *watcher;

      watcher = g_hash_table_lookup (namespace_watcher_watchers, GUINT_TO_POINTER (id));
      if (watcher)
        {
          /* make sure vanished() is not called as a result of this function */
          g_hash_table_remove_all (watcher->names);

          namespace_watcher_stop (watcher);
        }
    }
}