static const char *
get_icon_name_for_volume (gboolean is_mic,
                          gboolean muted,
                          int volume)
{
        static const char *icon_names[] = {
                "audio-volume-muted-symbolic",
                "audio-volume-low-symbolic",
                "audio-volume-medium-symbolic",
                "audio-volume-high-symbolic",
                NULL
        };
        static const char *mic_icon_names[] = {
                "microphone-sensitivity-muted-symbolic",
                "microphone-sensitivity-low-symbolic",
                "microphone-sensitivity-medium-symbolic",
                "microphone-sensitivity-high-symbolic",
                NULL
        };
        int n;

        if (muted) {
                n = 0;
        } else {
                /* select image */
                n = 3 * volume / 100 + 1;
                if (n < 1) {
                        n = 1;
                } else if (n > 3) {
                        n = 3;
                }
        }

	if (is_mic)
		return mic_icon_names[n];
	else
		return icon_names[n];
}