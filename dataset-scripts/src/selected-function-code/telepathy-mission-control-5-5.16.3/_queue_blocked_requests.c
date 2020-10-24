static gboolean
_queue_blocked_requests (McdRequest *self)
{
  const gchar *path = NULL;
  guint locks = 0;

  /* this is an internal request and therefore not subject to blocking *
     BUT the fact that this internal request is in-flight means other  *
     requests on the same account/handle type should be blocked        */
  if (self->internal_handler != NULL)
    {
      path = mcd_account_get_object_path (_mcd_request_get_account (self));
      _mcd_request_block_account (path);

      return FALSE;
    }

  /* no internal requests in flight, nothing to queue, nothing to do */
  if (account_locks == NULL)
    return FALSE;

  if (path == NULL)
    path = mcd_account_get_object_path (_mcd_request_get_account (self));

  /* account_locks tracks the # of in-flight internal requests per account */
  locks = GPOINTER_TO_UINT (g_hash_table_lookup (account_locks, path));

  /* internal reqeusts in flight => other requests on that account must wait */
  if (locks > 0)
    {
      GQueue *queue;

      queue = g_hash_table_lookup (blocked_reqs, path);

      if (queue == NULL)
        {
          queue = g_queue_new ();
          g_hash_table_insert (blocked_reqs, g_strdup (path), queue);
        }

      _mcd_request_start_delay (self);
      g_queue_push_tail (queue, self);

      return TRUE;
    }

  return FALSE;
}