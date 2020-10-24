static gchar *
get_tag_value (const gchar *tag_string,
               const gchar *tag_name)
{
        gchar **tag_string_splitted;
        gchar  *tag_value = NULL;
        gint    tag_name_length;
        gint    i;

        if (!tag_string ||
            !tag_name)
                return NULL;

        tag_name_length = strlen (tag_name);
        tag_string_splitted = g_strsplit (tag_string, ";", 0);
        if (tag_string_splitted) {
                for (i = 0; i < g_strv_length (tag_string_splitted); i++)
                        if (g_ascii_strncasecmp (tag_string_splitted[i], tag_name, tag_name_length) == 0)
                                if (strlen (tag_string_splitted[i]) > tag_name_length + 1)
                                        tag_value = g_strdup (tag_string_splitted[i] + tag_name_length + 1);

                g_strfreev (tag_string_splitted);
        }

        return tag_value;
}