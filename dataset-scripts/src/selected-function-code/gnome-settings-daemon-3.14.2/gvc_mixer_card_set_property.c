static void
gvc_mixer_card_set_property (GObject       *object,
                             guint          prop_id,
                             const GValue  *value,
                             GParamSpec    *pspec)
{
        GvcMixerCard *self = GVC_MIXER_CARD (object);

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
        case PROP_NAME:
                gvc_mixer_card_set_name (self, g_value_get_string (value));
                break;
        case PROP_ICON_NAME:
                gvc_mixer_card_set_icon_name (self, g_value_get_string (value));
                break;
        case PROP_PROFILE:
                gvc_mixer_card_set_profile (self, g_value_get_string (value));
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                break;
        }
}