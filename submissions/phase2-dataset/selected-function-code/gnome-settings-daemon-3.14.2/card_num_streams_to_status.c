static char *
card_num_streams_to_status (guint sinks,
                            guint sources)
{
        char *sinks_str;
        char *sources_str;
        char *ret;

        if (sinks == 0 && sources == 0) {
                /* translators:
                 * The device has been disabled */
                return g_strdup (_("Disabled"));
        }
        if (sinks == 0) {
                sinks_str = NULL;
        } else {
                /* translators:
                 * The number of sound outputs on a particular device */
                sinks_str = g_strdup_printf (ngettext ("%u Output",
                                                       "%u Outputs",
                                                       sinks),
                                             sinks);
        }
        if (sources == 0) {
                sources_str = NULL;
        } else {
                /* translators:
                 * The number of sound inputs on a particular device */
                sources_str = g_strdup_printf (ngettext ("%u Input",
                                                         "%u Inputs",
                                                         sources),
                                               sources);
        }
        if (sources_str == NULL)
                return sinks_str;
        if (sinks_str == NULL)
                return sources_str;
        ret = g_strdup_printf ("%s / %s", sinks_str, sources_str);
        g_free (sinks_str);
        g_free (sources_str);
        return ret;
}