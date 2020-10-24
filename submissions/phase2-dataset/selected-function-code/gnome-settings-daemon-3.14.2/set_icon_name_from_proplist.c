static void
set_icon_name_from_proplist (GvcMixerStream *stream,
                             pa_proplist    *l,
                             const char     *default_icon_name)
{
        const char *t;

        if ((t = pa_proplist_gets (l, PA_PROP_DEVICE_ICON_NAME))) {
                goto finish;
        }

        if ((t = pa_proplist_gets (l, PA_PROP_MEDIA_ICON_NAME))) {
                goto finish;
        }

        if ((t = pa_proplist_gets (l, PA_PROP_WINDOW_ICON_NAME))) {
                goto finish;
        }

        if ((t = pa_proplist_gets (l, PA_PROP_APPLICATION_ICON_NAME))) {
                goto finish;
        }

        if ((t = pa_proplist_gets (l, PA_PROP_MEDIA_ROLE))) {

                if (strcmp (t, "video") == 0 ||
                    strcmp (t, "phone") == 0) {
                        goto finish;
                }

                if (strcmp (t, "music") == 0) {
                        t = "audio";
                        goto finish;
                }

                if (strcmp (t, "game") == 0) {
                        t = "applications-games";
                        goto finish;
                }

                if (strcmp (t, "event") == 0) {
                        t = "dialog-information";
                        goto finish;
                }
        }

        t = default_icon_name;

 finish:
        gvc_mixer_stream_set_icon_name (stream, t);
}