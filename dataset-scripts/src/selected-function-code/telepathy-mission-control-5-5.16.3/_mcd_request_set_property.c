static void
_mcd_request_set_property (GObject *object,
    guint prop_id,
    const GValue *value,
    GParamSpec *pspec)
{
  McdRequest *self = (McdRequest *) object;

  switch (prop_id)
    {
    case PROP_USE_EXISTING:
      self->use_existing = g_value_get_boolean (value);
      break;

    case PROP_CLIENT_REGISTRY:
      g_assert (self->clients == NULL); /* construct-only */
      self->clients = g_value_dup_object (value);
      break;

    case PROP_ACCOUNT:
      g_assert (self->account == NULL); /* construct-only */
      self->account = g_value_dup_object (value);
      break;

    case PROP_PROPERTIES:
      g_assert (self->properties == NULL); /* construct-only */
      self->properties = g_hash_table_ref (g_value_get_boxed (value));
      break;

    case PROP_USER_ACTION_TIME:
      g_assert (self->user_action_time == 0); /* construct-only */
      self->user_action_time = g_value_get_int64 (value);
      break;

    case PROP_PREFERRED_HANDLER:
      if (self->preferred_handler != NULL)
        g_free (self->preferred_handler);

      self->preferred_handler = g_value_dup_string (value);
      break;

    case PROP_HINTS:
      g_assert (self->hints == NULL); /* construct-only */
      self->hints = g_value_dup_boxed (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}