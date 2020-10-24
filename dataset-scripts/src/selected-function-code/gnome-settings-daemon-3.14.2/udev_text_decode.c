static gchar *
udev_text_decode (const gchar *data)
{
        guint i;
        guint j;
        gchar *decode;

        decode = g_strdup (data);
        for (i = 0, j = 0; data[i] != '\0'; j++) {
                if (memcmp (&data[i], "\\x2f", 4) == 0) {
                        decode[j] = '/';
                        i += 4;
                } else if (memcmp (&data[i], "\\x5c", 4) == 0) {
                        decode[j] = '\\';
                        i += 4;
                } else {
                        decode[j] = data[i];
                        i++;
                }
        }
        decode[j] = '\0';
        return decode;
}