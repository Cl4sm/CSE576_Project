static void
mcd_account_self_contact_notify_avatar_file_cb (McdAccount *self,
    GParamSpec *unused_param_spec G_GNUC_UNUSED,
    TpContact *self_contact)
{
  const gchar *token;
  gchar *prev_token;
  GFile *file;
  GError *error = NULL;
  gboolean changed;

  if (self_contact != self->priv->self_contact)
    return;

  file = tp_contact_get_avatar_file (self_contact);
  token = tp_contact_get_avatar_token (self_contact);

  if (self->priv->setting_avatar)
    {
      DEBUG ("Ignoring avatar change notification: we are setting ours");
      return;
    }

  if (self->priv->waiting_for_initial_avatar)
    {
      DEBUG ("Ignoring avatar change notification: we are waiting for the "
          "initial value");
      return;
    }

  prev_token = _mcd_account_get_avatar_token (self);
  changed = tp_strdiff (prev_token, token);
  g_free (prev_token);

  if (!changed)
    {
      DEBUG ("Avatar unchanged: '%s'", token);
      return;
    }

  if (file == NULL)
    {
      if (!_mcd_account_set_avatar (self, NULL, "", "", &error))
        {
          DEBUG ("Attempt to clear avatar failed: %s", error->message);
          g_clear_error (&error);
        }
    }
  else
    {
      gchar *contents = NULL;
      gsize len = 0;
      GArray *arr;

      if (!g_file_load_contents (file, NULL, &contents, &len, NULL, &error))
        {
          gchar *uri = g_file_get_uri (file);

          WARNING ("Unable to read avatar file %s: %s", uri, error->message);
          g_clear_error (&error);
          g_free (uri);
          return;
        }

      if (G_UNLIKELY (len > G_MAXUINT))
        {
          gchar *uri = g_file_get_uri (file);

          WARNING ("Avatar file %s was ludicrously huge", uri);
          g_free (uri);
          g_free (contents);
          return;
        }

      arr = g_array_sized_new (TRUE, FALSE, 1, (guint) len);
      g_array_append_vals (arr, contents, (guint) len);
      g_free (contents);

      if (!_mcd_account_set_avatar (self, arr,
              tp_contact_get_avatar_mime_type (self_contact),
              tp_contact_get_avatar_token (self_contact), &error))
        {
          DEBUG ("Attempt to save avatar failed: %s", error->message);
          g_clear_error (&error);
        }

      g_array_unref (arr);
    }
}