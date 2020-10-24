{
	GError *err;

	/* Hackish ... needs fixing obviously :-)
	 *
	 * We scale the icons to 18x18 because the ones I randomly grabbed
	 * don't look good with my default font :-) Ideally, we should
	 * get the font metrics yadada yadada...
	 */
#define ICON_SIZE	18
#define PATH_PREFIX	"/usr/share/latencytop/"

	err = NULL;
	global_icon = gdk_pixbuf_new_from_file_at_size(PATH_PREFIX "global.png",
						       ICON_SIZE, ICON_SIZE,
						       &err);
	if (!global_icon) {
		err = NULL;
		global_icon = gdk_pixbuf_new_from_file_at_size("global.png",
							       ICON_SIZE, ICON_SIZE,
							       &err);
	}
	err = NULL;
	kthread_icon = gdk_pixbuf_new_from_file_at_size(PATH_PREFIX "kernel_thread.png",
							ICON_SIZE, ICON_SIZE,
							&err);
	if (!kthread_icon) {
		err = NULL;
		kthread_icon = gdk_pixbuf_new_from_file_at_size("kernel_thread.png",
								ICON_SIZE, ICON_SIZE,
								&err);
	}
	err = NULL;
	process_icon = gdk_pixbuf_new_from_file_at_size(PATH_PREFIX "user_process.png",
							ICON_SIZE, ICON_SIZE, &err);
	if (!process_icon) {
		err = NULL;
		process_icon = gdk_pixbuf_new_from_file_at_size("user_process.png",
								ICON_SIZE, ICON_SIZE,
								&err);
	}
}
