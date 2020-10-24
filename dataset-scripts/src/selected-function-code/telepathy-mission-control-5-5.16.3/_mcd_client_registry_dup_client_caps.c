GPtrArray *
_mcd_client_registry_dup_client_caps (McdClientRegistry *self)
{
  GPtrArray *vas;
  GHashTableIter iter;
  gpointer p;

  g_return_val_if_fail (MCD_IS_CLIENT_REGISTRY (self), NULL);

  vas = g_ptr_array_sized_new (g_hash_table_size (self->priv->clients));

  g_hash_table_iter_init (&iter, self->priv->clients);

  while (g_hash_table_iter_next (&iter, NULL, &p))
    {
      g_ptr_array_add (vas,
          _mcd_client_proxy_dup_handler_capabilities (p));
    }

  return vas;
}