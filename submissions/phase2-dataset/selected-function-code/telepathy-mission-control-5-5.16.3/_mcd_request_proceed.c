void
_mcd_request_proceed (McdRequest *self,
    DBusGMethodInvocation *context)
{
  McdConnection *connection = NULL;
  McdPluginRequest *plugin_api = NULL;
  gboolean urgent = FALSE;
  gboolean blocked = FALSE;
  const GList *mini_plugins;

  if (self->proceeding)
    {
      GError na = { TP_ERROR, TP_ERROR_NOT_AVAILABLE,
          "Proceed has already been called; stop calling it" };

      if (context != NULL)
        dbus_g_method_return_error (context, &na);

      return;
    }

  self->proceeding = TRUE;

  tp_clear_pointer (&context, tp_svc_channel_request_return_from_proceed);

  connection = mcd_account_get_connection (self->account);

  if (connection != NULL)
    {
      const gchar *name =
        tp_asv_get_string (self->properties, TP_PROP_CHANNEL_TARGET_ID);

      if (name != NULL)
        {
          urgent = _mcd_connection_target_id_is_urgent (connection, name);
        }
      else
        {
          guint handle = tp_asv_get_uint32 (self->properties,
              TP_PROP_CHANNEL_TARGET_HANDLE, NULL);

          urgent = _mcd_connection_target_handle_is_urgent (connection, handle);
        }
    }

  /* urgent calls (eg emergency numbers) are not subject to policy *
   * delays: they automatically pass go and collect 200 qwatloos   */
  if (urgent)
    goto proceed;

  /* requests can pick up an extra delay (and ref) here */
  blocked = _queue_blocked_requests (self);

  if (blocked)
    DEBUG ("Request delayed in favour of internal request on %s",
        mcd_account_get_object_path (self->account));

  /* now regular request policy plugins get their shot at denying/delaying */
  for (mini_plugins = _mcd_request_policy_plugins ();
       mini_plugins != NULL;
       mini_plugins = mini_plugins->next)
    {
      DEBUG ("Checking request with policy");

      /* Lazily create a plugin-API object if anything cares */
      if (plugin_api == NULL)
        {
          plugin_api = _mcd_plugin_request_new (self->account, self);
        }

      mcp_request_policy_check (mini_plugins->data, MCP_REQUEST (plugin_api));
    }

  /* this is paired with the delay set when the request was created */
 proceed:
  _mcd_request_end_delay (self);

  tp_clear_object (&plugin_api);
}