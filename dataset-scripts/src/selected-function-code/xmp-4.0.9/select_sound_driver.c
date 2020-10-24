struct sound_driver *select_sound_driver(struct options *options)
{
	struct list_head *head;
	struct sound_driver *sd;
	char *pref = options->driver_id;

	if (pref) {
		list_for_each(head, &sound_driver_list) {
			sd = list_entry(head, struct sound_driver, list);
			if (strcmp(sd->id, pref) == 0) {
				if (sd->init(options) == 0) {
					return sd;
				}
			}
		}
	} else {
		list_for_each(head, &sound_driver_list) {
			sd = list_entry(head, struct sound_driver, list);
			/* Probing */
			if (sd->init(options) == 0) {
				/* found */
				return sd;
			}
		}
	}

	return NULL;
}