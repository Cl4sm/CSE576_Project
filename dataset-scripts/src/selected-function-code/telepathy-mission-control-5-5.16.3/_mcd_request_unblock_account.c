guint
_mcd_request_unblock_account (const gchar *account)
{
  guint count = 0;
  gchar *key = NULL;

  if (G_UNLIKELY (account_locks == NULL))
    {
      g_warning ("Unbalanced account-request-unblock for %s", account);
      return 0;
    }

  count = GPOINTER_TO_UINT (g_hash_table_lookup (account_locks, account));

  switch (count)
    {
      GQueue *queue;

      case 0:
        g_warning ("Unbalanced account-request-unblock for %s", account);
        return 0;

      case 1:
        DEBUG ("removing lock from account %s", account);
        g_hash_table_remove (account_locks, account);
        queue = g_hash_table_lookup (blocked_reqs, account);

        if (queue == NULL)
          return 0;

        g_queue_foreach (queue, _unblock_request, NULL);
        g_queue_clear (queue);

        return 0;

      default:
        DEBUG ("reducing lock count for %s", account);
        key = g_strdup (account);
        g_hash_table_replace (account_locks, key, GUINT_TO_POINTER (--count));
        return count;
    }
}