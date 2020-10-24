extern int
verify_paths(struct multipath * mpp, struct vectors * vecs, vector rpvec)
{
	struct path * pp;
	int count = 0;
	int i, j;

	if (!mpp)
		return 0;

	vector_foreach_slot (mpp->paths, pp, i) {
		/*
		 * see if path is in sysfs
		 */
		if (sysfs_get_dev(pp->udev, pp->dev_t, BLK_DEV_SIZE) <= 0) {
			if (pp->state != PATH_DOWN) {
				condlog(1, "%s: removing valid path %s in state %d",
					mpp->alias, pp->dev, pp->state);
			} else {
				condlog(3, "%s: failed to access path %s",
					mpp->alias, pp->dev);
			}
			count++;
			vector_del_slot(mpp->paths, i);
			i--;

			if (rpvec)
				store_path(rpvec, pp);
			else {
				if ((j = find_slot(vecs->pathvec,
						   (void *)pp)) != -1)
					vector_del_slot(vecs->pathvec, j);
				free_path(pp);
			}
		} else {
			condlog(4, "%s: verified path %s dev_t %s",
				mpp->alias, pp->dev, pp->dev_t);
		}
	}
	return count;
}
