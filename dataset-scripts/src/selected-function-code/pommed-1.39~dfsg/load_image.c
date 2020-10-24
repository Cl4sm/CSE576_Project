static GtkWidget *
load_image(const char *name, const char *img)
{
  GError *error = NULL;
  GdkPixbuf *pixbuf;

  char file[PATH_MAX];
  int ret;

  ret = snprintf(file, PATH_MAX, "%s/%s/%s", THEME_BASE, name, img);
  if (ret >= PATH_MAX)
    return NULL;

  pixbuf = gdk_pixbuf_new_from_file(file, &error);

  if (error != NULL)
    {
      printf("Error loading theme file %s: %s\n", name, error->message);

      g_error_free(error);
      return NULL;
    }

  return gtk_image_new_from_pixbuf(pixbuf);
}