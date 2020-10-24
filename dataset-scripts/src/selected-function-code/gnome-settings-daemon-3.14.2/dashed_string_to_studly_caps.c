static char *
dashed_string_to_studly_caps (const char *dashed_string)
{
        char *studly_string;
        size_t studly_string_length;
        size_t i;

        i = 0;

        studly_string = g_strdup (dashed_string);
        studly_string_length = strlen (studly_string);

        studly_string[i] = g_ascii_toupper (studly_string[i]);
        i++;

        while (i < studly_string_length) {
                if (studly_string[i] == '-' || studly_string[i] == '_') {
                        memmove (studly_string + i,
                                 studly_string + i + 1,
                                 studly_string_length - i - 1);
                        studly_string_length--;
                        if (g_ascii_isalpha (studly_string[i])) {
                                studly_string[i] = g_ascii_toupper (studly_string[i]);
                        }
                }
                i++;
        }
        studly_string[studly_string_length] = '\0';

        return studly_string;
}