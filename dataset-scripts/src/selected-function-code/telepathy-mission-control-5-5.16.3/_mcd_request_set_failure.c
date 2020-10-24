void
_mcd_request_set_failure (McdRequest *self,
    GQuark domain,
    gint code,
    const gchar *message)
{
  if (!self->is_complete)
    {
      GError e = { domain, code, (gchar *) message };
      gchar *err_string;

      DEBUG ("Request failed: %s %d: %s", g_quark_to_string (domain),
          code, message);

      err_string = _mcd_build_error_string (&e);

      self->is_complete = TRUE;
      self->cancellable = FALSE;
      self->failure_domain = domain;
      self->failure_code = code;
      self->failure_message = g_strdup (message);

      if (self->predicted_handler != NULL)
        {
          /* no callback, as we don't really care: this method call acts as a
           * pseudo-signal */
          DEBUG ("calling RemoveRequest on %s for %s",
                 tp_proxy_get_object_path (self->predicted_handler),
                 self->object_path);
          tp_cli_client_interface_requests_call_remove_request (
              self->predicted_handler, -1, self->object_path, err_string,
              message, NULL, NULL, NULL, NULL);
        }

      tp_svc_channel_request_emit_failed (self, err_string, message);

      g_free (err_string);

      _mcd_request_clean_up (self);
    }
  else
    {
      DEBUG ("Ignoring an attempt to fail after already complete");
    }
}