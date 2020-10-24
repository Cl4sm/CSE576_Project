static void
plugin_do_set_property (GObject *object,
    guint prop_id,
    const GValue *value,
    GParamSpec *pspec)
{
  McdPluginDispatchOperation *self = (McdPluginDispatchOperation *) object;

  switch (prop_id)
    {
    case PROP_REAL_CDO:
      g_assert (self->real_cdo == NULL); /* construct-only */
      /* The real CDO is borrowed, because this plugin API is owned by it */
      self->real_cdo = g_value_get_object (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}