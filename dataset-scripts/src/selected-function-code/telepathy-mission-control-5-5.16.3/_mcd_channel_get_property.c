static void
_mcd_channel_get_property (GObject * obj, guint prop_id,
			   GValue * val, GParamSpec * pspec)
{
    McdChannelPrivate *priv = MCD_CHANNEL_PRIV (obj);

    switch (prop_id)
    {
    case PROP_TP_CHANNEL:
	g_value_set_object (val, priv->tp_chan);
	break;

    case PROP_OUTGOING:
	g_value_set_boolean (val, priv->outgoing);
	break;

    case PROP_ACCOUNT_PATH:
        if (priv->request != NULL)
        {
            g_object_get_property ((GObject *) priv->request,
                                   "account-path", val);
            break;
        }
        g_value_set_static_boxed (val, "/");
        break;

    case PROP_USER_ACTION_TIME:
        if (priv->request != NULL)
        {
            g_object_get_property ((GObject *) priv->request,
                                   "user-action-time", val);
            break;
        }
        g_value_set_int64 (val, TP_USER_ACTION_TIME_NOT_USER_ACTION);
        break;

    case PROP_PREFERRED_HANDLER:
        if (priv->request != NULL)
        {
            g_object_get_property ((GObject *) priv->request,
                                   "preferred-handler", val);
            break;
        }
        g_value_set_static_string (val, "");
        break;

    case PROP_REQUESTS:
        if (priv->request != NULL)
        {
            g_object_get_property ((GObject *) priv->request,
                                   "requests", val);
            break;
        }
        g_value_take_boxed (val, g_ptr_array_sized_new (0));
        break;

    case PROP_INTERFACES:
        if (priv->request != NULL)
        {
            g_object_get_property ((GObject *) priv->request,
                                   "interfaces", val);
            break;
        }
        g_value_take_boxed (val, NULL);
        break;

    case PROP_HINTS:
        if (priv->request != NULL)
        {
            g_object_get_property ((GObject *) priv->request,
                                   "hints", val);
            break;
        }
        g_value_take_boxed (val, g_hash_table_new (NULL, NULL));
        break;

    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, prop_id, pspec);
	break;
    }
}