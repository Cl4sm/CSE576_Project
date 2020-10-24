static GType
mc_param_type (const TpConnectionManagerParam *param)
{
    const gchar *dbus_signature;

    if (G_UNLIKELY (param == NULL))
        return G_TYPE_INVALID;

    dbus_signature = tp_connection_manager_param_get_dbus_signature (param);

    if (G_UNLIKELY (!dbus_signature))
        return G_TYPE_INVALID;

    switch (dbus_signature[0])
    {
    case DBUS_TYPE_STRING:
	return G_TYPE_STRING;

    case DBUS_TYPE_BYTE:
        return G_TYPE_UCHAR;

    case DBUS_TYPE_INT16:
    case DBUS_TYPE_INT32:
	return G_TYPE_INT;

    case DBUS_TYPE_UINT16:
    case DBUS_TYPE_UINT32:
	return G_TYPE_UINT;

    case DBUS_TYPE_BOOLEAN:
	return G_TYPE_BOOLEAN;

    case DBUS_TYPE_DOUBLE:
        return G_TYPE_DOUBLE;

    case DBUS_TYPE_OBJECT_PATH:
        return DBUS_TYPE_G_OBJECT_PATH;

    case DBUS_TYPE_INT64:
        return G_TYPE_INT64;

    case DBUS_TYPE_UINT64:
        return G_TYPE_UINT64;

    case DBUS_TYPE_ARRAY:
        if (dbus_signature[1] == DBUS_TYPE_STRING)
            return G_TYPE_STRV;
        /* other array types are not supported:
         * fall through the default case */
    default:
        g_warning ("skipping parameter %s, unknown type %s",
            tp_connection_manager_param_get_name (param), dbus_signature);
    }
    return G_TYPE_INVALID;
}