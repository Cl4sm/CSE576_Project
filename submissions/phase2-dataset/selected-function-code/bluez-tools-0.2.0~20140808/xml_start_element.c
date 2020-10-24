                              const gchar *element_name,
                              const gchar **attribute_names,
                              const gchar **attribute_values,
                              gpointer user_data,
                              GError **error)
{
    const gchar *id_t = xml_get_attr_value("id", attribute_names, attribute_values);
    const gchar *value_t = xml_get_attr_value("value", attribute_names, attribute_values);

    if (g_strcmp0(element_name, "record") == 0)
    {
        xml_t[REC]++;
    }
    else if (g_strcmp0(element_name, "attribute") == 0 && id_t)
    {
        int attr_id = xtoi(id_t);
        const gchar *attr_name = sdp_get_attr_id_name(attr_id);

        xml_t[ATTR]++;
        xml_t[ATTR_ID] = attr_id;

        if (!verbose_arg && is_verbose_attr(xml_t[ATTR_ID])) return;

        if (attr_name == NULL)
        {
            g_print("AttrID-%s: ", id_t);
        }
        else
        {
            g_print("%s: ", attr_name);
        }
    }
    else if (g_strcmp0(element_name, "sequence") == 0)
    {
        xml_t[SEQ]++;
    }
    else if (g_pattern_match_simple("*int*", element_name) && value_t)
    {
        xml_t[ELEM]++;

        if (!verbose_arg && is_verbose_attr(xml_t[ATTR_ID])) return;

        if (xml_t[ELEM] == 1 && xml_t[SEQ] > 1)
        {
            g_print("\n");
            for (int i = 0; i < xml_t[SEQ]; i++) g_print("  ");
        }
        else if (xml_t[ELEM] > 1)
        {
            g_print(", ");
        }

        if (xml_t[UUID_ID] == SDP_UUID_RFCOMM)
        {
            g_print("Channel: %d", xtoi(value_t));
        }
        else
        {
            g_print("0x%x", xtoi(value_t));
        }
    }
    else if (g_strcmp0(element_name, "uuid") == 0 && value_t)
    {
        int uuid_id = -1;
        const gchar *uuid_name;

        if (value_t[0] == '0' && value_t[1] == 'x')
        {
            uuid_id = xtoi(value_t);
            uuid_name = sdp_get_uuid_name(uuid_id);
        }
        else
        {
            uuid_name = uuid2name(value_t);
        }

        xml_t[ELEM]++;
        xml_t[UUID_ID] = uuid_id;

        if (!verbose_arg && is_verbose_attr(xml_t[ATTR_ID])) return;

        if (xml_t[ELEM] == 1 && xml_t[SEQ] > 1)
        {
            g_print("\n");
            for (int i = 0; i < xml_t[SEQ]; i++) g_print("  ");
        }
        else if (xml_t[ELEM] > 1)
        {
            g_print(", ");
        }

        if (uuid_name == NULL)
        {
            g_print("\"UUID-%s\"", value_t);
        }
        else
        {
            g_print("\"%s\"", uuid_name);
        }
    }
    else if (g_strcmp0(element_name, "text") == 0 && value_t)
    {
        xml_t[ELEM]++;

        if (!verbose_arg && is_verbose_attr(xml_t[ATTR_ID])) return;

        if (xml_t[ELEM] == 1 && xml_t[SEQ] > 1)
        {
            g_print("\n");
            for (int i = 0; i < xml_t[SEQ]; i++) g_print("  ");
        }
        else if (xml_t[ELEM] > 1)
        {
            g_print(", ");
        }

        g_print("\"%s\"", value_t);
    }
    else if (g_strcmp0(element_name, "boolean") == 0 && value_t)
    {
        xml_t[ELEM]++;

        if (!verbose_arg && is_verbose_attr(xml_t[ATTR_ID])) return;

        if (xml_t[ELEM] == 1 && xml_t[SEQ] > 1)
        {
            g_print("\n");
            for (int i = 0; i < xml_t[SEQ]; i++) g_print("  ");
        }
        else if (xml_t[ELEM] > 1)
        {
            g_print(", ");
        }

        g_print("%s", value_t);
    }
    else
    {
        if (error)
            *error = g_error_new(G_MARKUP_ERROR, G_MARKUP_ERROR_UNKNOWN_ELEMENT, "Invalid XML element: %s", element_name);
    }
}
