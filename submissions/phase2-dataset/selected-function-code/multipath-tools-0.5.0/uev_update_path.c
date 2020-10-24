static int
uev_update_path (struct uevent *uev, struct vectors * vecs)
{
	int ro, retval = 0;

	ro = uevent_get_disk_ro(uev);

	if (ro >= 0) {
		struct path * pp;

		condlog(2, "%s: update path write_protect to '%d' (uevent)",
			uev->kernel, ro);
		pp = find_path_by_dev(vecs->pathvec, uev->kernel);
		if (!pp) {
			condlog(0, "%s: spurious uevent, path not found",
				uev->kernel);
			return 1;
		}
		if (pp->mpp) {
			retval = reload_map(vecs, pp->mpp, 0);

			condlog(2, "%s: map %s reloaded (retval %d)",
				uev->kernel, pp->mpp->alias, retval);
		}

	}

	return retval;
}
