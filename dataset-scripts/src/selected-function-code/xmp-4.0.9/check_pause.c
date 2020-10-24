static void check_pause(xmp_context xc, struct control *ctl,
	struct xmp_module_info *mi, struct xmp_frame_info *fi, int verbose)
{
	if (ctl->pause) {
		sound->pause();
		if (verbose) {
			info_frame(mi, fi, ctl, 1);
		}
		while (ctl->pause) {
			usleep(100000);
			read_command(xc, mi, ctl);
			if (ctl->display) {
				show_info(ctl->display, mi);
				if (verbose) {
					info_frame(mi, fi, ctl, 1);
				}
				ctl->display = 0;
			}
		}
		sound->resume();
	}
}