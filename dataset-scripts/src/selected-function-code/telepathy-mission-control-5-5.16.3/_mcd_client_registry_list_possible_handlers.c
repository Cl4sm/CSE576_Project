GList *
_mcd_client_registry_list_possible_handlers (McdClientRegistry *self,
    const gchar *preferred_handler,
    GVariant *request_props,
    TpChannel *channel,
    const gchar *must_have_unique_name)
{
  GList *handlers = NULL;
  GList *handlers_iter;
  GHashTableIter client_iter;
  gpointer client_p;

  _mcd_client_registry_init_hash_iter (self, &client_iter);

  while (g_hash_table_iter_next (&client_iter, NULL, &client_p))
    {
      McdClientProxy *client = MCD_CLIENT_PROXY (client_p);
      gsize quality;

      if (must_have_unique_name != NULL &&
          tp_strdiff (must_have_unique_name,
            _mcd_client_proxy_get_unique_name (client)))
        {
          /* we're trying to redispatch to an existing handler, and this is
           * not it */
          continue;
        }

      if (!tp_proxy_has_interface_by_id (client,
            TP_IFACE_QUARK_CLIENT_HANDLER))
        {
            /* not a handler at all */
            continue;
        }

      if (channel == NULL)
        {
          /* We don't know the channel's properties (the next part will not
           * execute), so we must work out the quality of match from the
           * channel request. We can assume that the request will return one
           * channel, with the requested properties, plus Requested == TRUE.
           */
          g_assert (request_props != NULL);
          quality = _mcd_client_match_filters (request_props,
              _mcd_client_proxy_get_handler_filters (client), TRUE);
        }
      else
        {
          GVariant *properties;

          g_assert (TP_IS_CHANNEL (channel));
          properties = tp_channel_dup_immutable_properties (channel);
          quality = _mcd_client_match_filters (properties,
              _mcd_client_proxy_get_handler_filters (client), FALSE);
          g_variant_unref (properties);
        }

      if (quality > 0)
        {
          PossibleHandler *ph = g_slice_new0 (PossibleHandler);

          ph->client = client;
          ph->bypass = _mcd_client_proxy_get_bypass_approval (client);
          ph->quality = quality;

          handlers = g_list_prepend (handlers, ph);
        }
    }

  /* if no handlers can take them all, fail - unless we're operating on
   * a request that specified a preferred handler, in which case assume
   * it's suitable */
  if (handlers == NULL)
    {
      McdClientProxy *client;

      if (preferred_handler == NULL || preferred_handler[0] == '\0')
        {
          return NULL;
        }

      client = _mcd_client_registry_lookup (self, preferred_handler);

      if (client == NULL)
        {
          return NULL;
        }

      return g_list_append (NULL, client);
    }

  /* We have at least one handler that can take the whole batch. Sort
   * the possible handlers, most preferred first (i.e. sort by ascending
   * quality then reverse) */
  handlers = g_list_sort (handlers, possible_handler_cmp);
  handlers = g_list_reverse (handlers);

  /* convert in-place from a list of PossibleHandler to a list of
   * McdClientProxy */
  for (handlers_iter = handlers;
       handlers_iter != NULL;
       handlers_iter = handlers_iter->next)
    {
      PossibleHandler *ph = handlers_iter->data;

      handlers_iter->data = ph->client;
      g_slice_free (PossibleHandler, ph);
    }

  return handlers;
}