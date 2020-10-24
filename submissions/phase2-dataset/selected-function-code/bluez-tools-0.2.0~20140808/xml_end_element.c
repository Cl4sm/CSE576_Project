static void xml_end_element(GMarkupParseContext *context,
                            const gchar *element_name,
                            gpointer user_data,
                            GError **error)
{
    if (g_strcmp0(element_name, "record") == 0)
    {
        xml_t[ATTR] = 0;
        xml_t[SEQ] = 0;
        xml_t[ELEM] = 0;

        xml_t[ATTR_ID] = -1;
        xml_t[UUID_ID] = -1;
    }
    else if (g_strcmp0(element_name, "attribute") == 0)
    {
        xml_t[SEQ] = 0;
        xml_t[ELEM] = 0;

        int old_attr_id = xml_t[ATTR_ID];
        xml_t[ATTR_ID] = -1;
        xml_t[UUID_ID] = -1;

        if (!verbose_arg && is_verbose_attr(old_attr_id)) return;

        g_print("\n");
    }
    else if (g_strcmp0(element_name, "sequence") == 0)
    {
        xml_t[SEQ]--;
        xml_t[ELEM] = 0;

        xml_t[UUID_ID] = -1;
    }
}
