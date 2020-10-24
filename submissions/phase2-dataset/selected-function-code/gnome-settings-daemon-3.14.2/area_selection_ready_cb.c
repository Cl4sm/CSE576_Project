static void
area_selection_ready_cb (GObject *source,
                         GAsyncResult *res,
                         gpointer user_data)
{
  GdkRectangle rectangle;
  ScreenshotContext *ctx = user_data;
  GVariant *geometry;

  geometry = g_dbus_connection_call_finish (G_DBUS_CONNECTION (source),
                                            res, NULL);

  /* cancelled by the user */
  if (!geometry)
    {
      screenshot_context_free (ctx);
      return;
    }

  g_variant_get (geometry, "(iiii)",
                 &rectangle.x, &rectangle.y,
                 &rectangle.width, &rectangle.height);

  ctx->area_selection = rectangle;
  screenshot_call_shell (ctx);
  g_variant_unref (geometry);
}