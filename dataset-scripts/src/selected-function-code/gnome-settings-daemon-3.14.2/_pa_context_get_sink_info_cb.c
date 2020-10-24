static void
_pa_context_get_sink_info_cb (pa_context         *context,
                              const pa_sink_info *i,
                              int                 eol,
                              void               *userdata)
{
        GvcMixerControl *control = GVC_MIXER_CONTROL (userdata);

        if (eol < 0) {
                if (pa_context_errno (context) == PA_ERR_NOENTITY) {
                        return;
                }

                g_warning ("Sink callback failure");
                return;
        }

        if (eol > 0) {
                dec_outstanding (control);
                return;
        }

        update_sink (control, i);
}