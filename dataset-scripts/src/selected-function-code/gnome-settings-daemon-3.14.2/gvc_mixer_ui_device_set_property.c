static void
gvc_mixer_ui_device_set_property  (GObject      *object,
                                   guint         property_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
        GvcMixerUIDevice *self = GVC_MIXER_UI_DEVICE (object);

        switch (property_id) {
        case PROP_DESC_LINE_1:
                g_free (self->priv->first_line_desc);
                self->priv->first_line_desc = g_value_dup_string (value);
                g_debug ("gvc-mixer-output-set-property - 1st line: %s\n",
                         self->priv->first_line_desc);
                break;
        case PROP_DESC_LINE_2:
                g_free (self->priv->second_line_desc);
                self->priv->second_line_desc = g_value_dup_string (value);
                g_debug ("gvc-mixer-output-set-property - 2nd line: %s\n",
                         self->priv->second_line_desc);
                break;
        case PROP_CARD:
                self->priv->card = g_value_get_pointer (value);
                g_debug ("gvc-mixer-output-set-property - card: %p\n",
                         self->priv->card);
                break;
        case PROP_PORT_NAME:
                g_free (self->priv->port_name);
                self->priv->port_name = g_value_dup_string (value);
                g_debug ("gvc-mixer-output-set-property - card port name: %s\n",
                         self->priv->port_name);
                break;
        case PROP_STREAM_ID:
                self->priv->stream_id = g_value_get_int (value);
                g_debug ("gvc-mixer-output-set-property - sink/source id: %i\n",
                         self->priv->stream_id);
                break;
        case PROP_UI_DEVICE_TYPE:
                self->priv->type = (GvcMixerUIDeviceDirection) g_value_get_uint (value);
                break;
        case PROP_PORT_AVAILABLE:
                self->priv->port_available = g_value_get_boolean (value);
                g_debug ("gvc-mixer-output-set-property - port available %i, value passed in %i \n",
                         self->priv->port_available, g_value_get_boolean (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
                break;
        }
}