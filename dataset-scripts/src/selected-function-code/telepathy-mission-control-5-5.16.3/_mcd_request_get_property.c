static void
_mcd_request_get_property (GObject *object,
    guint prop_id,
    GValue *value,
    GParamSpec *pspec)
{
  McdRequest *self = (McdRequest *) object;

  switch (prop_id)
    {
    case PROP_USE_EXISTING:
      g_value_set_boolean (value, self->use_existing);
      break;

    case PROP_CLIENT_REGISTRY:
      g_value_set_object (value, self->clients);
      break;

    case PROP_ACCOUNT:
      g_value_set_object (value, self->account);
      break;

    case PROP_ACCOUNT_PATH:
      g_value_set_boxed (value, mcd_account_get_object_path (self->account));
      break;

    case PROP_PROPERTIES:
      g_value_set_boxed (value, self->properties);
      break;

    case PROP_PREFERRED_HANDLER:
      if (self->preferred_handler == NULL)
        {
          g_value_set_static_string (value, "");
        }
      else
        {
          g_value_set_string (value, self->preferred_handler);
        }
      break;

    case PROP_USER_ACTION_TIME:
      g_value_set_int64 (value, self->user_action_time);
      break;

    case PROP_HINTS:
      if (self->hints == NULL)
        {
          g_value_take_boxed (value, g_hash_table_new (NULL, NULL));
        }
      else
        {
          g_value_set_boxed (value, self->hints);
        }
      break;

    case PROP_REQUESTS:
        {
          GPtrArray *arr = g_ptr_array_sized_new (1);

          g_ptr_array_add (arr, g_hash_table_ref (self->properties));
          g_value_take_boxed (value, arr);
        }
      break;

    case PROP_INTERFACES:
      /* we have no interfaces */
      g_value_set_static_boxed (value, NULL);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}