int
theme_load(const char *name)
{
  GError *error = NULL;

  char file[PATH_MAX];
  int i;
  int ret;

  ret = snprintf(file, PATH_MAX, "%s/%s/background.png", THEME_BASE, name);
  if (ret >= PATH_MAX)
    return -1;

  if (theme.background)
    g_object_unref(G_OBJECT(theme.background));

  theme.background = gdk_pixbuf_new_from_file(file, &error);

  if (error != NULL)
    {
      printf("Error loading theme background: %s\n", error->message);

      g_error_free(error);
      return -1;
    }

  theme.width = gdk_pixbuf_get_width (theme.background);
  theme.height = gdk_pixbuf_get_height (theme.background);

  /*
   * We need to up the refcount to prevent GTK from destroying
   * the images by itself when we start adding/removing them
   * to/from a GtkContainer.
   */

  for (i = 0; i < IMG_NIMG; i++)
    {
      if (theme.images[i])
	g_object_unref(G_OBJECT(theme.images[i]));
    }

  theme.images[IMG_LCD_BCK] = load_image(name, "brightness.png");
  theme.images[IMG_KBD_BCK] = load_image(name, "kbdlight.png");
  theme.images[IMG_AUDIO_VOL_ON] = load_image(name, "volume.png");
  theme.images[IMG_AUDIO_VOL_OFF] = load_image(name, "mute.png");
  theme.images[IMG_AUDIO_MUTE] = load_image(name, "noaudio.png");
  theme.images[IMG_CD_EJECT] = load_image(name, "cdrom.png");

  for (i = 0; i < IMG_NIMG; i++)
    {
      g_object_ref(G_OBJECT(theme.images[i]));
    }

  return 0;
}