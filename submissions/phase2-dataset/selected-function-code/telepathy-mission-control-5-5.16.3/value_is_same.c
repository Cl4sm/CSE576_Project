static gboolean
value_is_same (const GValue *val1, const GValue *val2)
{
    g_return_val_if_fail (val1 != NULL && val2 != NULL, FALSE);
    switch (G_VALUE_TYPE (val1))
    {
    case G_TYPE_STRING:
        return g_strcmp0 (g_value_get_string (val1),
                          g_value_get_string (val2)) == 0;
    case G_TYPE_CHAR:
    case G_TYPE_UCHAR:
    case G_TYPE_INT:
    case G_TYPE_UINT:
    case G_TYPE_BOOLEAN:
        return val1->data[0].v_uint == val2->data[0].v_uint;

    case G_TYPE_INT64:
        return g_value_get_int64 (val1) == g_value_get_int64 (val2);
    case G_TYPE_UINT64:
        return g_value_get_uint64 (val1) == g_value_get_uint64 (val2);

    case G_TYPE_DOUBLE:
        return g_value_get_double (val1) == g_value_get_double (val2);

    default:
        if (G_VALUE_TYPE (val1) == DBUS_TYPE_G_OBJECT_PATH)
        {
            return !tp_strdiff (g_value_get_boxed (val1),
                                g_value_get_boxed (val2));
        }
        else if (G_VALUE_TYPE (val1) == G_TYPE_STRV)
        {
            gchar **left = g_value_get_boxed (val1);
            gchar **right = g_value_get_boxed (val2);

            if (left == NULL || right == NULL ||
                *left == NULL || *right == NULL)
            {
                return ((left == NULL || *left == NULL) &&
                        (right == NULL || *right == NULL));
            }

            while (*left != NULL || *right != NULL)
            {
                if (tp_strdiff (*left, *right))
                {
                    return FALSE;
                }

                left++;
                right++;
            }

            return TRUE;
        }
        else
        {
            g_warning ("%s: unexpected type %s",
                       G_STRFUNC, G_VALUE_TYPE_NAME (val1));
            return FALSE;
        }
    }
}