static void
gvc_mixer_stream_set_property (GObject       *object,
                               guint          prop_id,
                               const GValue  *value,
                               GParamSpec    *pspec)
{
        GvcMixerStream *self = GVC_MIXER_STREAM (object);

        switch (prop_id) {
        case PROP_PA_CONTEXT:
                self->priv->pa_context = g_value_get_pointer (value);
                break;
        case PROP_INDEX:
                self->priv->index = g_value_get_ulong (value);
                break;
        case PROP_ID:
                self->priv->id = g_value_get_ulong (value);
                break;
        case PROP_CHANNEL_MAP:
                gvc_mixer_stream_set_channel_map (self, g_value_get_object (value));
                break;
        case PROP_NAME:
                gvc_mixer_stream_set_name (self, g_value_get_string (value));
                break;
        case PROP_DESCRIPTION:
                gvc_mixer_stream_set_description (self, g_value_get_string (value));
                break;
        case PROP_APPLICATION_ID:
                gvc_mixer_stream_set_application_id (self, g_value_get_string (value));
                break;
        case PROP_ICON_NAME:
                gvc_mixer_stream_set_icon_name (self, g_value_get_string (value));
                break;
        case PROP_FORM_FACTOR:
                gvc_mixer_stream_set_form_factor (self, g_value_get_string (value));
                break;
	case PROP_SYSFS_PATH:
		gvc_mixer_stream_set_sysfs_path (self, g_value_get_string (value));
		break;
        case PROP_VOLUME:
                gvc_mixer_stream_set_volume (self, g_value_get_ulong (value));
                break;
        case PROP_DECIBEL:
                gvc_mixer_stream_set_decibel (self, g_value_get_double (value));
                break;
        case PROP_IS_MUTED:
                gvc_mixer_stream_set_is_muted (self, g_value_get_boolean (value));
                break;
        case PROP_IS_EVENT_STREAM:
                gvc_mixer_stream_set_is_event_stream (self, g_value_get_boolean (value));
                break;
        case PROP_IS_VIRTUAL:
                gvc_mixer_stream_set_is_virtual (self, g_value_get_boolean (value));
                break;
        case PROP_CAN_DECIBEL:
                gvc_mixer_stream_set_can_decibel (self, g_value_get_boolean (value));
                break;
        case PROP_PORT:
                gvc_mixer_stream_set_port (self, g_value_get_string (value));
                break;
        case PROP_CARD_INDEX:
                self->priv->card_index = g_value_get_long (value);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                break;
        }
}