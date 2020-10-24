static void
screenshot_save_to_clipboard (ScreenshotContext *ctx)
{
  GdkPixbuf *screenshot;
  GtkClipboard *clipboard;
  GError *error = NULL;

  screenshot = gdk_pixbuf_new_from_file (ctx->used_filename, &error);
  if (error != NULL)
    {
      screenshot_context_error (ctx, error, "Failed to save a screenshot to clipboard: %s\n");
      return;
    }

  screenshot_play_sound_effect ("screen-capture", _("Screenshot taken"));
  clipboard = gtk_clipboard_get_for_display (gdk_display_get_default (),
                                             GDK_SELECTION_CLIPBOARD);
  gtk_clipboard_set_image (clipboard, screenshot);

  /* remove the temporary file created by the shell */
  g_unlink (ctx->used_filename);
  g_object_unref (screenshot);
}