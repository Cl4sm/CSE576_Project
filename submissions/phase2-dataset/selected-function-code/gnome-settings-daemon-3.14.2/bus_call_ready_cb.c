static void
bus_call_ready_cb (GObject *source,
                   GAsyncResult *res,
                   gpointer user_data)
{
  GError *error = NULL;
  ScreenshotContext *ctx = user_data;
  GVariant *variant;
  gboolean success;

  variant = g_dbus_connection_call_finish (G_DBUS_CONNECTION (source), res, &error);

  if (error != NULL)
    {
      screenshot_context_error (ctx, error, "Failed to save a screenshot: %s\n");
      screenshot_context_free (ctx);

      return;
    }

  g_variant_get (variant, "(bs)", &success, &ctx->used_filename);

  if (success)
    {
      if (ctx->copy_to_clipboard)
        {
          screenshot_save_to_clipboard (ctx);
        }
      else
        {
          screenshot_play_sound_effect ("screen-capture", _("Screenshot taken"));
          screenshot_save_to_recent (ctx);
        }
    }

  screenshot_context_free (ctx);
  g_variant_unref (variant);
}