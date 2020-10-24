static void
gvc_mixer_control_stream_restore_cb (pa_context *c,
				     GvcMixerStream *new_stream,
                                     const pa_ext_stream_restore_info *info,
                                     GvcMixerControl *control)
{
        pa_operation *o;
        pa_ext_stream_restore_info new_info;

        if (new_stream == NULL)
                return;

        new_info.name = info->name;
        new_info.channel_map = info->channel_map;
        new_info.volume = info->volume;
        new_info.mute = info->mute;

        new_info.device = gvc_mixer_stream_get_name (new_stream);

        o = pa_ext_stream_restore_write (control->priv->pa_context,
                                         PA_UPDATE_REPLACE,
                                         &new_info, 1,
                                         TRUE, NULL, NULL);

        if (o == NULL) {
                g_warning ("pa_ext_stream_restore_write() failed: %s",
                           pa_strerror (pa_context_errno (control->priv->pa_context)));
                return;
        }

        g_debug ("Changed default device for %s to %s", info->name, new_info.device);

        pa_operation_unref (o);
}