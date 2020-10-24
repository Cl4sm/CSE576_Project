static void
do_sound_action (GsdMediaKeysManager *manager,
		 guint                deviceid,
                 int                  type,
                 gboolean             is_output,
                 gboolean             quiet)
{
	GvcMixerStream *stream;
        gboolean old_muted, new_muted;
        guint old_vol, new_vol, norm_vol_step;
        gboolean sound_changed;

        /* Find the stream that corresponds to the device, if any */
        stream = NULL;
#ifdef HAVE_GUDEV
        stream = get_stream_for_device_id (manager, is_output, deviceid);
#endif /* HAVE_GUDEV */

        if (stream == NULL) {
                if (is_output)
                        stream = manager->priv->sink;
                else
                        stream = manager->priv->source;
        }

        if (stream == NULL)
                return;

        norm_vol_step = PA_VOLUME_NORM * VOLUME_STEP / 100;

        /* FIXME: this is racy */
        new_vol = old_vol = gvc_mixer_stream_get_volume (stream);
        new_muted = old_muted = gvc_mixer_stream_get_is_muted (stream);
        sound_changed = FALSE;

        switch (type) {
        case MUTE_KEY:
                new_muted = !old_muted;
                break;
        case VOLUME_DOWN_KEY:
                if (old_vol <= norm_vol_step) {
                        new_vol = 0;
                        new_muted = TRUE;
                } else {
                        new_vol = old_vol - norm_vol_step;
                }
                break;
        case VOLUME_UP_KEY:
                new_muted = FALSE;
                /* When coming out of mute only increase the volume if it was 0 */
                if (!old_muted || old_vol == 0)
                        new_vol = MIN (old_vol + norm_vol_step, MAX_VOLUME);
                break;
        }

        if (old_muted != new_muted) {
                gvc_mixer_stream_change_is_muted (stream, new_muted);
                sound_changed = TRUE;
        }

        if (old_vol != new_vol) {
                if (gvc_mixer_stream_set_volume (stream, new_vol) != FALSE) {
                        gvc_mixer_stream_push_volume (stream);
                        sound_changed = TRUE;
                }
        }

        update_dialog (manager, stream, new_vol, new_muted, sound_changed, quiet);
}