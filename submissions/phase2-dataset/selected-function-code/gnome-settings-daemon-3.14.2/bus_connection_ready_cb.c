static void
bus_connection_ready_cb (GObject *source,
                         GAsyncResult *res,
                         gpointer user_data)
{
  GError *error = NULL;
  ScreenshotContext *ctx = user_data;

  ctx->connection = g_bus_get_finish (res, &error);

  if (error != NULL)
    {
      screenshot_context_error (ctx, error, "Failed to save a screenshot: %s\n");
      screenshot_context_free (ctx);

      return;
    }

  if (ctx->type == SCREENSHOT_TYPE_AREA)
    g_dbus_connection_call (ctx->connection,
                            SHELL_SCREENSHOT_BUS_NAME,
                            SHELL_SCREENSHOT_BUS_PATH,
                            SHELL_SCREENSHOT_BUS_IFACE,
                            "SelectArea",
                            NULL,
                            NULL,
                            G_DBUS_CALL_FLAGS_NO_AUTO_START,
                            -1,
                            NULL,
                            area_selection_ready_cb,
                            ctx);
  else
    screenshot_call_shell (ctx);
}