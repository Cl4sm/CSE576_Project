static char *
dashed_string_to_dbus_error_string (const char *dashed_string,
                                    const char *old_prefix,
                                    const char *new_prefix,
                                    const char *suffix)
{
        char *studly_suffix;
        char *dbus_error_string;
        size_t dbus_error_string_length;
        size_t i;

        i = 0;

        if (g_str_has_prefix (dashed_string, old_prefix) &&
            (dashed_string[strlen(old_prefix)] == '-' ||
             dashed_string[strlen(old_prefix)] == '_')) {
                dashed_string += strlen (old_prefix) + 1;
        }

        studly_suffix = dashed_string_to_studly_caps (suffix);
        dbus_error_string = g_strdup_printf ("%s.%s.%s", new_prefix, dashed_string, studly_suffix);
        g_free (studly_suffix);
        i += strlen (new_prefix) + 1;

        dbus_error_string_length = strlen (dbus_error_string);

        dbus_error_string[i] = g_ascii_toupper (dbus_error_string[i]);
        i++;

        while (i < dbus_error_string_length) {
                if (dbus_error_string[i] == '_' || dbus_error_string[i] == '-') {
                        dbus_error_string[i] = '.';

                        if (g_ascii_isalpha (dbus_error_string[i + 1])) {
                                dbus_error_string[i + 1] = g_ascii_toupper (dbus_error_string[i + 1]);
                        }
                }

                i++;
        }

        return dbus_error_string;
}