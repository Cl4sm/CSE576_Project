char *
gsd_smartcard_utils_escape_object_path (const char *unescaped_string)
{
  const char *p;
  char *object_path;
  GString *string;

  g_return_val_if_fail (unescaped_string != NULL, NULL);

  string = g_string_new ("");

  for (p = unescaped_string; *p != '\0'; p++)
    {
      guchar character;

      character = (guchar) * p;

      if (((character >= ((guchar) 'a')) &&
           (character <= ((guchar) 'z'))) ||
          ((character >= ((guchar) 'A')) &&
           (character <= ((guchar) 'Z'))) ||
          ((character >= ((guchar) '0')) && (character <= ((guchar) '9'))))
        {
          g_string_append_c (string, (char) character);
          continue;
        }

      g_string_append_printf (string, "_%x_", character);
    }

  object_path = string->str;

  g_string_free (string, FALSE);

  return object_path;
}