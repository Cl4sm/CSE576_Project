static void
_mcd_request_dispose (GObject *object)
{
  McdRequest *self = (McdRequest *) object;
  GObjectFinalizeFunc dispose =
    G_OBJECT_CLASS (_mcd_request_parent_class)->dispose;

  DEBUG ("%p", object);

  /* shouldn't ever actually get this far with a blocked account, *
   * but we have to clear the lock if we do or we'll deadlock     */
  if (_mcd_request_is_internal (self) && self->account != NULL)
    {
      const gchar *path = mcd_account_get_object_path (self->account);
      _mcd_request_unblock_account (path);
      g_warning ("internal request disposed without being handled or failed");
    }

  tp_clear_object (&self->account);
  tp_clear_object (&self->clients);
  tp_clear_object (&self->predicted_handler);
  tp_clear_pointer (&self->hints, g_hash_table_unref);

  if (dispose != NULL)
    dispose (object);
}