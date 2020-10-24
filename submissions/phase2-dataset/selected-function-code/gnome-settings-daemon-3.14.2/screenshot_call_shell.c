static void
screenshot_call_shell (ScreenshotContext *ctx)
{
  const gchar *method_name;
  GVariant *method_params;

  if (ctx->type == SCREENSHOT_TYPE_SCREEN)
    {
      method_name = "Screenshot";
      method_params = g_variant_new ("(bbs)",
                                     FALSE, /* include pointer */
                                     TRUE,  /* flash */
                                     ctx->save_filename);
    }
  else if (ctx->type == SCREENSHOT_TYPE_WINDOW)
    {
      method_name = "ScreenshotWindow";
      method_params = g_variant_new ("(bbbs)",
                                     TRUE,  /* include border */
                                     FALSE, /* include pointer */
                                     TRUE,  /* flash */
                                     ctx->save_filename);
    }
  else
    {
      method_name = "ScreenshotArea";
      method_params = g_variant_new ("(iiiibs)",
                                     ctx->area_selection.x, ctx->area_selection.y,
                                     ctx->area_selection.width, ctx->area_selection.height,
                                     TRUE, /* flash */
                                     ctx->save_filename);
    }

  g_dbus_connection_call (ctx->connection,
                          SHELL_SCREENSHOT_BUS_NAME,
                          SHELL_SCREENSHOT_BUS_PATH,
                          SHELL_SCREENSHOT_BUS_IFACE,
                          method_name,
                          method_params,
                          NULL,
                          G_DBUS_CALL_FLAGS_NO_AUTO_START,
                          -1,
                          NULL,
                          bus_call_ready_cb,
                          ctx);
}