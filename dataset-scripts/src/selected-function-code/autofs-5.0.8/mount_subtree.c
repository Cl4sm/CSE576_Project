static int mount_subtree(struct autofs_point *ap, struct mapent *me,
			 const char *name, char *loc, char *options, void *ctxt)
{
	struct mapent *mm;
	struct mapent *ro;
	char *mm_root, *mm_base, *mm_key;
	const char *mnt_root;
	unsigned int mm_root_len, mnt_root_len;
	int start, ret = 0, rv;

	rv = 0;

	mm = me->multi;
	mm_key = mm->key;

	if (*mm_key == '/') {
		mm_root = mm_key;
		start = strlen(mm_key);
	} else {
		start = strlen(ap->path) + strlen(mm_key) + 1;
		mm_root = alloca(start + 3);
		strcpy(mm_root, ap->path);
		strcat(mm_root, "/");
		strcat(mm_root, mm_key);
	}
	mm_root_len = strlen(mm_root);

	mnt_root = mm_root;
	mnt_root_len = mm_root_len;

	if (me == me->multi) {
		/* name = NULL */
		/* destination = mm_root */
		mm_base = "/";

		/* Mount root offset if it exists */
		ro = cache_lookup_offset(mm_base, mm_base, strlen(mm_root), &me->multi_list);
		if (ro) {
			char *myoptions, *ro_loc, *tmp;
			int namelen = name ? strlen(name) : 0;
			const char *root;
			int ro_len;

			rv = parse_mapent(ro->mapent,
				options, &myoptions, &ro_loc, ap->logopt);
			if (!rv) {
				warn(ap->logopt,
				      MODPREFIX "failed to parse root offset");
				cache_delete_offset_list(me->mc, name);
				return 1;
			}
			ro_len = strlen(ro_loc);

			tmp = alloca(mnt_root_len + 2);
			strcpy(tmp, mnt_root);
			tmp[mnt_root_len] = '/';
			tmp[mnt_root_len + 1] = '\0';
			root = tmp;

			rv = sun_mount(ap, root, name, namelen, ro_loc, ro_len, myoptions, ctxt);

			free(myoptions);
			free(ro_loc);
		}

		if (ro && rv == 0) {
			ret = mount_multi_triggers(ap, me, mnt_root, start, mm_base);
			if (ret == -1) {
				error(ap->logopt, MODPREFIX
					 "failed to mount offset triggers");
				cleanup_multi_triggers(ap, me, mnt_root, start, mm_base);
				return 1;
			}
		} else if (rv <= 0) {
			ret = mount_multi_triggers(ap, me, mm_root, start, mm_base);
			if (ret == -1) {
				error(ap->logopt, MODPREFIX
					 "failed to mount offset triggers");
				cleanup_multi_triggers(ap, me, mm_root, start, mm_base);
				return 1;
			}
		}
	} else {
		int loclen = strlen(loc);
		int namelen = strlen(name);

		mnt_root = name;

		/* name = mm_root + mm_base */
		/* destination = mm_root + mm_base = name */
		mm_base = &me->key[start];

		rv = sun_mount(ap, mnt_root, name, namelen, loc, loclen, options, ctxt);
		if (rv == 0) {
			ret = mount_multi_triggers(ap, me->multi, mnt_root, start, mm_base);
			if (ret == -1) {
				error(ap->logopt, MODPREFIX
					 "failed to mount offset triggers");
				cleanup_multi_triggers(ap, me, mnt_root, start, mm_base);
				return 1;
			}
		} else if (rv < 0) {
			char *mm_root_base = alloca(strlen(mm_root) + strlen(mm_base) + 1);
	
			strcpy(mm_root_base, mm_root);
			strcat(mm_root_base, mm_base);

			ret = mount_multi_triggers(ap, me->multi, mm_root_base, start, mm_base);
			if (ret == -1) {
				error(ap->logopt, MODPREFIX
					 "failed to mount offset triggers");
				cleanup_multi_triggers(ap, me, mm_root, start, mm_base);
				return 1;
			}
		}
	}

	/* Mount for base of tree failed */
	if (rv > 0)
		return rv;

	/*
	 * Convert fail on nonstrict, non-empty multi-mount
	 * to success
	 */
	if (rv < 0 && ret > 0)
		rv = 0;

	return rv;
}