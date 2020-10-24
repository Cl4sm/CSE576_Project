static gchar *
get_profile_canonical_name (const gchar *profile_name, const gchar *skip_prefix)
{
        gchar *result = NULL;
        gchar **s;
        int i;

        /* optimisation for the simple case. */
        if (strstr (profile_name, skip_prefix) == NULL)
                return g_strdup (profile_name);

        s = g_strsplit (profile_name, "+", 0);
        for (i = 0; i < g_strv_length (s); i++) {
                if (g_str_has_prefix (s[i], skip_prefix))
                        continue;
                if (result == NULL)
                        result = g_strdup (s[i]);
                else {
                        gchar *c = g_strdup_printf("%s+%s", result, s[i]);
                        g_free(result);
                        result = c;
                }
        }

        g_strfreev(s);

        if (!result)
                return g_strdup("off");

        return result;
}