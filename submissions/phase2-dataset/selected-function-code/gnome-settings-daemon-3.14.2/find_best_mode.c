static GnomeRRMode *
find_best_mode (GnomeRROutput *output)
{
        GnomeRRMode *preferred;
        GnomeRRMode **modes;
        int best_size;
        int best_width, best_height, best_rate;
        int i;
        GnomeRRMode *best_mode;

        preferred = gnome_rr_output_get_preferred_mode (output);
        if (preferred)
                return preferred;

        modes = gnome_rr_output_list_modes (output);
        if (!modes)
                return NULL;

        best_size = best_width = best_height = best_rate = 0;
        best_mode = NULL;

        for (i = 0; modes[i] != NULL; i++) {
                int w, h, r;
                int size;

                w = gnome_rr_mode_get_width (modes[i]);
                h = gnome_rr_mode_get_height (modes[i]);
                r = gnome_rr_mode_get_freq  (modes[i]);

                size = w * h;

                if (size > best_size) {
                        best_size   = size;
                        best_width  = w;
                        best_height = h;
                        best_rate   = r;
                        best_mode   = modes[i];
                } else if (size == best_size) {
                        if (r > best_rate) {
                                best_rate = r;
                                best_mode = modes[i];
                        }
                }
        }

        return best_mode;
}