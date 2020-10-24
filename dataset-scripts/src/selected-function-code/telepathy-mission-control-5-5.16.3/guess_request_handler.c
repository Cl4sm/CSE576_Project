static TpClient *
guess_request_handler (McdRequest *self)
{
  GList *sorted_handlers;
  GVariant *properties;

  if (!tp_str_empty (self->preferred_handler))
    {
      McdClientProxy *client = _mcd_client_registry_lookup (
          self->clients, self->preferred_handler);

        if (client != NULL)
            return (TpClient *) client;
    }

  properties = mcd_request_dup_properties (self);
  sorted_handlers = _mcd_client_registry_list_possible_handlers (
      self->clients, self->preferred_handler, properties,
      NULL, NULL);
  g_variant_unref (properties);

  if (sorted_handlers != NULL)
    {
      McdClientProxy *first = sorted_handlers->data;

      g_list_free (sorted_handlers);
      return (TpClient *) first;
    }

  return NULL;
}