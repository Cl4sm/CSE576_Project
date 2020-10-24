static void
gvc_mixer_ui_device_get_property  (GObject       *object,
                                   guint         property_id,
                                   GValue        *value,
                                   GParamSpec    *pspec)
{
        GvcMixerUIDevice *self = GVC_MIXER_UI_DEVICE (object);

        switch (property_id) {
        case PROP_DESC_LINE_1:
                g_value_set_string (value, self->priv->first_line_desc);
                break;
        case PROP_DESC_LINE_2:
                g_value_set_string (value, self->priv->second_line_desc);
                break;
        case PROP_CARD:
                g_value_set_pointer (value, self->priv->card);
                break;
        case PROP_PORT_NAME:
                g_value_set_string (value, self->priv->port_name);
                break;
        case PROP_STREAM_ID:
                g_value_set_int (value, self->priv->stream_id);
                break;
        case PROP_UI_DEVICE_TYPE:
                g_value_set_uint (value, (guint)self->priv->type);
                break;
        case PROP_PORT_AVAILABLE:
                g_value_set_boolean (value, self->priv->port_available);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}