int
uev_trigger (struct uevent * uev, void * trigger_data)
{
	int r = 0;
	struct vectors * vecs;

	vecs = (struct vectors *)trigger_data;

	if (uev_discard(uev->devpath))
		return 0;

	pthread_cleanup_push(cleanup_lock, &vecs->lock);
	lock(vecs->lock);
	pthread_testcancel();

	/*
	 * device map event
	 * Add events are ignored here as the tables
	 * are not fully initialised then.
	 */
	if (!strncmp(uev->kernel, "dm-", 3)) {
		if (!strncmp(uev->action, "change", 6)) {
			r = uev_add_map(uev, vecs);
			goto out;
		}
		if (!strncmp(uev->action, "remove", 6)) {
			r = uev_remove_map(uev, vecs);
			goto out;
		}
		goto out;
	}

	/*
	 * path add/remove event
	 */
	if (filter_devnode(conf->blist_devnode, conf->elist_devnode,
			   uev->kernel) > 0)
		goto out;

	if (!strncmp(uev->action, "add", 3)) {
		r = uev_add_path(uev, vecs);
		goto out;
	}
	if (!strncmp(uev->action, "remove", 6)) {
		r = uev_remove_path(uev, vecs);
		goto out;
	}
	if (!strncmp(uev->action, "change", 6)) {
		r = uev_update_path(uev, vecs);
		goto out;
	}

out:
	lock_cleanup_pop(vecs->lock);
	return r;
}
