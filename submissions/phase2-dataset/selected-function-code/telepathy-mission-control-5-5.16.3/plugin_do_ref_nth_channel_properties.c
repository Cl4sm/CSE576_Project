static GHashTable *
plugin_do_ref_nth_channel_properties (McpDispatchOperation *obj,
    guint n)
{
  McdPluginDispatchOperation *self = MCD_PLUGIN_DISPATCH_OPERATION (obj);
  McdChannel *channel;
  GVariant *variant;
  GValue value = G_VALUE_INIT;
  GHashTable *ret;

  g_return_val_if_fail (self != NULL, NULL);

  channel = _mcd_dispatch_operation_peek_channel (self->real_cdo);

  if (channel == NULL || n != 0)
    return NULL;

  variant = mcd_channel_dup_immutable_properties (channel);

  if (variant == NULL)
    return NULL;

  /* For compatibility, we have to return the older type here. */
  dbus_g_value_parse_g_variant (variant, &value);
  g_assert (G_VALUE_HOLDS (&value, TP_HASH_TYPE_STRING_VARIANT_MAP));
  ret = g_value_dup_boxed (&value);
  g_value_unset (&value);

  return ret;
}