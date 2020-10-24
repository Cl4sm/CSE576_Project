void
_mcd_request_set_success (McdRequest *self,
    TpChannel *channel)
{
  g_return_if_fail (TP_IS_CHANNEL (channel));

  if (!self->is_complete)
    {
      /* might be used for the connection's properties in future; empty
       * for now */
      GHashTable *future_conn_props = g_hash_table_new (g_str_hash,
          g_str_equal);
      GVariant *variant;
      GValue value = G_VALUE_INIT;

      DEBUG ("Request succeeded");
      self->is_complete = TRUE;
      self->cancellable = FALSE;

      variant = tp_channel_dup_immutable_properties (channel);
      dbus_g_value_parse_g_variant (variant, &value);
      g_assert (G_VALUE_HOLDS (&value, TP_HASH_TYPE_STRING_VARIANT_MAP));

      tp_svc_channel_request_emit_succeeded_with_channel (self,
          tp_proxy_get_object_path (tp_channel_get_connection (channel)),
          future_conn_props,
          tp_proxy_get_object_path (channel),
          g_value_get_boxed (&value));
      tp_svc_channel_request_emit_succeeded (self);

      g_hash_table_unref (future_conn_props);
      g_value_unset (&value);
      g_variant_unref (variant);

      _mcd_request_clean_up (self);
    }
  else
    {
      DEBUG ("Ignoring an attempt to succeed after already complete");
    }
}