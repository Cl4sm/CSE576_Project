static void
_pa_context_get_card_info_by_index_cb (pa_context *context,
                                       const pa_card_info *i,
                                       int eol,
                                       void *userdata)
{
        GvcMixerControl *control = GVC_MIXER_CONTROL (userdata);

        if (eol < 0) {
                if (pa_context_errno (context) == PA_ERR_NOENTITY)
                        return;

                g_warning ("Card callback failure");
                return;
        }

        if (eol > 0) {
                dec_outstanding (control);
                return;
        }

        update_card (control, i);
}