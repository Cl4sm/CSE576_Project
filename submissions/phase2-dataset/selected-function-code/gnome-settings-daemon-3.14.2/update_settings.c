static gboolean
update_settings (GvcMixerEventRole *role,
                 gboolean           is_muted,
                 gpointer          *op)
{
        pa_operation              *o;
        const GvcChannelMap       *map;
        pa_context                *context;
        pa_ext_stream_restore_info info;

        map = gvc_mixer_stream_get_channel_map (GVC_MIXER_STREAM(role));

        info.volume = *gvc_channel_map_get_cvolume(map);
        info.name = "sink-input-by-media-role:event";
        info.channel_map = *gvc_channel_map_get_pa_channel_map(map);
        info.device = role->priv->device;
        info.mute = is_muted;

        context = gvc_mixer_stream_get_pa_context (GVC_MIXER_STREAM (role));

        o = pa_ext_stream_restore_write (context,
                                         PA_UPDATE_REPLACE,
                                         &info,
                                         1,
                                         TRUE,
                                         NULL,
                                         NULL);

        if (o == NULL) {
                g_warning ("pa_ext_stream_restore_write() failed");
                return FALSE;
        }

        if (op != NULL)
                *op = o;

        return TRUE;
}