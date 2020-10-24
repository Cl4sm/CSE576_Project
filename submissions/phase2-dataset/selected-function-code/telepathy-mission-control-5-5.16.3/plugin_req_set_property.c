static void
plugin_req_set_property (GObject *object,
    guint prop_id,
    const GValue *value,
    GParamSpec *pspec)
{
  McdPluginRequest *self = (McdPluginRequest *) object;

  switch (prop_id)
    {
    case PROP_REAL_REQUEST:
      g_assert (self->real_request == NULL); /* construct-only */
      self->real_request = g_value_dup_object (value);
      break;

    case PROP_ACCOUNT:
      g_assert (self->account == NULL); /* construct-only */
      self->account = g_value_dup_object (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}