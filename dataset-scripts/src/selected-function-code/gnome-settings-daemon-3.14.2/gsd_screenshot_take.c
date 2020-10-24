void
gsd_screenshot_take (MediaKeyType key_type)
{
  ScreenshotContext *ctx = g_slice_new0 (ScreenshotContext);

  ctx->copy_to_clipboard = (key_type == SCREENSHOT_CLIP_KEY ||
                            key_type == WINDOW_SCREENSHOT_CLIP_KEY ||
                            key_type == AREA_SCREENSHOT_CLIP_KEY);

  switch (key_type)
    {
    case SCREENSHOT_KEY:
    case SCREENSHOT_CLIP_KEY:
      ctx->type = SCREENSHOT_TYPE_SCREEN;
      break;
    case WINDOW_SCREENSHOT_KEY:
    case WINDOW_SCREENSHOT_CLIP_KEY:
      ctx->type = SCREENSHOT_TYPE_WINDOW;
      break;
    case AREA_SCREENSHOT_KEY:
    case AREA_SCREENSHOT_CLIP_KEY:
      ctx->type = SCREENSHOT_TYPE_AREA;
      break;
    default:
      g_assert_not_reached ();
      break;
    }

  screenshot_check_name_ready (ctx);
}