void
_mcd_request_predict_handler (McdRequest *self)
{
  GHashTable *properties;
  TpClient *predicted_handler;

  g_return_if_fail (!self->is_complete);
  g_return_if_fail (self->predicted_handler == NULL);

  predicted_handler = guess_request_handler (self);

  if (!predicted_handler)
    {
      /* No handler found. But it's possible that by the time that the
       * channel will be created some handler will have popped up, so we
       * must not destroy it. */
      DEBUG ("No known handler for request %s", self->object_path);
      return;
    }

  if (!tp_proxy_has_interface_by_id (predicted_handler,
      TP_IFACE_QUARK_CLIENT_INTERFACE_REQUESTS))
    {
      DEBUG ("Default handler %s for request %s doesn't want AddRequest",
             tp_proxy_get_bus_name (predicted_handler), self->object_path);
      return;
    }

  DEBUG ("Calling AddRequest on default handler %s for request %s",
      tp_proxy_get_bus_name (predicted_handler), self->object_path);

  properties = _mcd_request_dup_immutable_properties (self);
  tp_cli_client_interface_requests_call_add_request (predicted_handler, -1,
      self->object_path, properties,
      NULL, NULL, NULL, NULL);
  g_hash_table_unref (properties);

  /* Remember it so we can call RemoveRequest when appropriate */
  self->predicted_handler = g_object_ref (predicted_handler);
}