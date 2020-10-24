static void
gvc_mixer_stream_get_property (GObject     *object,
                               guint        prop_id,
                               GValue      *value,
                               GParamSpec  *pspec)
{
        GvcMixerStream *self = GVC_MIXER_STREAM (object);

        switch (prop_id) {
        case PROP_PA_CONTEXT:
                g_value_set_pointer (value, self->priv->pa_context);
                break;
        case PROP_INDEX:
                g_value_set_ulong (value, self->priv->index);
                break;
        case PROP_ID:
                g_value_set_ulong (value, self->priv->id);
                break;
        case PROP_CHANNEL_MAP:
                g_value_set_object (value, self->priv->channel_map);
                break;
        case PROP_NAME:
                g_value_set_string (value, self->priv->name);
                break;
        case PROP_DESCRIPTION:
                g_value_set_string (value, self->priv->description);
                break;
        case PROP_APPLICATION_ID:
                g_value_set_string (value, self->priv->application_id);
                break;
        case PROP_ICON_NAME:
                g_value_set_string (value, self->priv->icon_name);
                break;
        case PROP_FORM_FACTOR:
                g_value_set_string (value, self->priv->form_factor);
                break;
	case PROP_SYSFS_PATH:
		g_value_set_string (value, self->priv->sysfs_path);
		break;
        case PROP_VOLUME:
                g_value_set_ulong (value,
                                   pa_cvolume_max(gvc_channel_map_get_cvolume(self->priv->channel_map)));
                break;
        case PROP_DECIBEL:
                g_value_set_double (value,
                                    pa_sw_volume_to_dB(pa_cvolume_max(gvc_channel_map_get_cvolume(self->priv->channel_map))));
                break;
        case PROP_IS_MUTED:
                g_value_set_boolean (value, self->priv->is_muted);
                break;
        case PROP_IS_EVENT_STREAM:
                g_value_set_boolean (value, self->priv->is_event_stream);
                break;
        case PROP_IS_VIRTUAL:
                g_value_set_boolean (value, self->priv->is_virtual);
                break;
        case PROP_CAN_DECIBEL:
                g_value_set_boolean (value, self->priv->can_decibel);
                break;
        case PROP_PORT:
                g_value_set_string (value, self->priv->port);
                break;
        case PROP_CARD_INDEX:
                g_value_set_long (value, self->priv->card_index);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                break;
        }
}