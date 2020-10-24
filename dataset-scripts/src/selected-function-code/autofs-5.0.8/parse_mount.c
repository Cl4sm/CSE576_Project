int parse_mount(struct autofs_point *ap, const char *name,
		int name_len, const char *mapent, void *context)
{
	struct parse_context *ctxt = (struct parse_context *) context;
	char buf[MAX_ERR_BUF];
	struct map_source *source;
	struct mapent_cache *mc;
	struct mapent *me;
	char *pmapent, *options;
	const char *p;
	int mapent_len, rv = 0;
	int cur_state;
	int slashify = ctxt->slashify_colons;
	unsigned int append_options;

	source = ap->entry->current;
	ap->entry->current = NULL;
	master_source_current_signal(ap->entry);

	mc = source->mc;

	if (!mapent) {
		warn(ap->logopt, MODPREFIX "error: empty map entry");
		return 1;
	}

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
	macro_lock();

	ctxt->subst = addstdenv(ctxt->subst, NULL);

	mapent_len = expandsunent(mapent, NULL, name, ctxt->subst, slashify);
	if (mapent_len == 0) {
		error(ap->logopt, MODPREFIX "failed to expand map entry");
		ctxt->subst = removestdenv(ctxt->subst, NULL);
		macro_unlock();
		pthread_setcancelstate(cur_state, NULL);
		return 1;
	}

	pmapent = alloca(mapent_len + 1);
	if (!pmapent) {	
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr(MODPREFIX "alloca: %s", estr);
		ctxt->subst = removestdenv(ctxt->subst, NULL);
		macro_unlock();
		pthread_setcancelstate(cur_state, NULL);
		return 1;
	}
	pmapent[mapent_len] = '\0';

	expandsunent(mapent, pmapent, name, ctxt->subst, slashify);
	ctxt->subst = removestdenv(ctxt->subst, NULL);

	macro_unlock();
	pthread_setcancelstate(cur_state, NULL);

	debug(ap->logopt, MODPREFIX "expanded entry: %s", pmapent);

	append_options = defaults_get_append_options();
	options = strdup(ctxt->optstr ? ctxt->optstr : "");
	if (!options) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr(MODPREFIX "strdup: %s", estr);
		return 1;
	}

	p = skipspace(pmapent);

	/* Deal with 0 or more options */
	if (*p == '-') {
		char *tmp, *mnt_options = NULL;

		do {
			char *noptions = NULL;

			p = parse_options(p, &noptions, ap->logopt);
			if (mnt_options && noptions && strstr(noptions, mnt_options)) {
				free(mnt_options);
				mnt_options = noptions;
			} else {
				tmp = concat_options(mnt_options, noptions);
				if (!tmp) {
					char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
					error(ap->logopt,
					      MODPREFIX "concat_options: %s", estr);
					if (noptions)
						free(noptions);
					if (mnt_options)
						free(mnt_options);
					free(options);
					return 1;
				}
				mnt_options = tmp;
			}

			p = skipspace(p);
		} while (*p == '-');

		if (options && !append_options) {
			free(options);
			options = NULL;
		}

		if (append_options) {
			if (options && mnt_options && strstr(mnt_options, options)) {
				free(options);
				options = mnt_options;
			} else {
				tmp = concat_options(options, mnt_options);
				if (!tmp) {
					char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
					error(ap->logopt, MODPREFIX "concat_options: %s", estr);
					if (options)
						free(options);
					if (mnt_options)
						free(mnt_options);
					return 1;
				}
				options = tmp;
			}
		} else
			options = mnt_options;
	}

	debug(ap->logopt, MODPREFIX "gathered options: %s", options);

	if (check_is_multi(p)) {
		char *m_root = NULL;
		int m_root_len;
		time_t age;
		int l;

		/* If name starts with "/" it's a direct mount */
		if (*name == '/') {
			m_root_len = name_len;
			m_root = alloca(m_root_len + 1);
			if (!m_root) {
				char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
				free(options);
				logerr(MODPREFIX "alloca: %s", estr);
				return 1;
			}
			strcpy(m_root, name);
		} else {
			m_root_len = strlen(ap->path) + name_len + 1;
			m_root = alloca(m_root_len + 1);
			if (!m_root) {
				char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
				free(options);
				logerr(MODPREFIX "alloca: %s", estr);
				return 1;
			}
			strcpy(m_root, ap->path);
			strcat(m_root, "/");
			strcat(m_root, name);
		}

		cache_readlock(mc);
		me = cache_lookup_distinct(mc, name);
		if (me) {
			/* So we know we're the multi-mount root */
			if (!me->multi)
				me->multi = me;
		}

		if (!me) {
			free(options);
			cache_unlock(mc);
			error(ap->logopt,
			      MODPREFIX "can't find multi root %s", name);
			return 1;
		}

		age = me->age;

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
		cache_multi_writelock(me);
		/* It's a multi-mount; deal with it */
		do {
			char *path, *myoptions, *loc;
			int status;

			if ((*p == '"' && *(p + 1) != '/') || (*p != '"' && *p != '/')) {
				l = 0;
				path = dequote("/", 1, ap->logopt);
				debug(ap->logopt,
				      MODPREFIX "dequote(\"/\") -> %s", path);
			} else {
				l = span_space(p, mapent_len - (p - pmapent));
				path = sanitize_path(p, l, LKP_MULTI, ap->logopt);
				debug(ap->logopt, MODPREFIX
				      "dequote(\"%.*s\") -> %s", l, p, path);
			}

			if (!path) {
				warn(ap->logopt, MODPREFIX "null path or out of memory");
				cache_delete_offset_list(mc, name);
				cache_multi_unlock(me);
				cache_unlock(mc);
				free(options);
				pthread_setcancelstate(cur_state, NULL);
				return 1;
			}

			p += l;
			p = skipspace(p);

			l = parse_mapent(p, options, &myoptions, &loc, ap->logopt);
			if (!l) {
				if (!(strstr(myoptions, "fstype=autofs") &&
				      strstr(myoptions, "hosts"))) {
					error(LOGOPT_ANY, "I think I'm a hosts map? l %d", l);
					cache_delete_offset_list(mc, name);
					cache_multi_unlock(me);
					cache_unlock(mc);
					free(path);
					free(options);
					pthread_setcancelstate(cur_state, NULL);
					return 1;
				}
			}

			p += l;
			p = skipspace(p);

			master_source_current_wait(ap->entry);
			ap->entry->current = source;

			status = update_offset_entry(ap, name,
						     m_root, m_root_len,
						     path, myoptions, loc, age);

			if (status != CHE_OK) {
				warn(ap->logopt, MODPREFIX "error adding multi-mount");
				cache_delete_offset_list(mc, name);
				cache_multi_unlock(me);
				cache_unlock(mc);
				free(path);
				free(options);
				free(myoptions);
				free(loc);
				pthread_setcancelstate(cur_state, NULL);
				return 1;
			}

			free(loc);
			free(path);
			free(myoptions);
		} while (*p == '/' || (*p == '"' && *(p + 1) == '/'));

		/*
		 * We've got the ordered list of multi-mount entries so go
		 * through and remove any stale entries if this is the top
		 * of the multi-mount and set the parent entry of each.
		 */
		if (me == me->multi)
			clean_stale_multi_triggers(ap, me, NULL, NULL);
		cache_set_parents(me);

		rv = mount_subtree(ap, me, name, NULL, options, ctxt);

		cache_multi_unlock(me);
		cache_unlock(mc);

		free(options);
		pthread_setcancelstate(cur_state, NULL);

		return rv;
	} else {
		/* Normal (and non-root multi-mount) entries */
		char *loc;
		int loclen;
		int l;

		/*
		 * If this is an offset belonging to a multi-mount entry
		 * it's already been parsed (above) and any option string
		 * has already been stripped so just use the remainder.
		 */
		cache_readlock(mc);
		if (*name == '/' &&
		   (me = cache_lookup_distinct(mc, name)) && me->multi) {
			loc = strdup(p);
			if (!loc) {
				free(options);
				cache_unlock(mc);
				warn(ap->logopt, MODPREFIX "out of memory");
				return 1;
			}
			cache_multi_writelock(me);
			rv = mount_subtree(ap, me, name, loc, options, ctxt);
			cache_multi_unlock(me);
			cache_unlock(mc);
			free(loc);
			free(options);
			return rv;
		}
		cache_unlock(mc);

		l = chunklen(p, check_colon(p));
		loc = dequote(p, l, ap->logopt);
		if (!loc) {
			free(options);
			warn(ap->logopt, MODPREFIX "null location or out of memory");
			return 1;
		}

		/* Location can't begin with a '/' */
		if (*p == '/') {
			free(options);
			free(loc);
			warn(ap->logopt,
			      MODPREFIX "error location begins with \"/\"");
			return 1;
		}

		if (!validate_location(ap->logopt, loc)) {
			free(loc);
			free(options);
			return 1;
		}

		debug(ap->logopt,
		      MODPREFIX "dequote(\"%.*s\") -> %s", l, p, loc);

		p += l;
		p = skipspace(p);

		while (*p) {
			char *tmp, *ent;

			l = chunklen(p, check_colon(p));
			ent = dequote(p, l, ap->logopt);
			if (!ent) {
				free(loc);
				free(options);
				warn(ap->logopt,
				     MODPREFIX "null location or out of memory");
				return 1;
			}

			if (!validate_location(ap->logopt, ent)) {
				free(ent);
				free(loc);
				free(options);
				return 1;
			}

			debug(ap->logopt,
			      MODPREFIX "dequote(\"%.*s\") -> %s", l, p, ent);

			tmp = realloc(loc, strlen(loc) + l + 2);
			if (!tmp) {
				free(ent);
				free(loc);
				free(options);
				error(ap->logopt, MODPREFIX "out of memory");
				return 1;
			}
			loc = tmp;

			strcat(loc, " ");
			strcat(loc, ent);

			free(ent);

			p += l;
			p = skipspace(p);
		}

		/*
		 * If options are asking for a hosts map loc should be
		 * NULL but we see it can contain junk, so ....
		 */
		if ((strstr(options, "fstype=autofs") &&
		     strstr(options, "hosts"))) {
			loc = NULL;
			loclen = 0;
		} else {
			loclen = strlen(loc);
			if (loclen == 0) {
				free(loc);
				free(options);
				error(ap->logopt,
				      MODPREFIX "entry %s is empty!", name);
				return 1;
			}
		}

		debug(ap->logopt,
		      MODPREFIX "core of entry: options=%s, loc=%.*s",
		      options, loclen, loc);

		if (!strcmp(ap->path, "/-"))
			rv = sun_mount(ap, name, name, name_len,
				       loc, loclen, options, ctxt);
		else
			rv = sun_mount(ap, ap->path, name, name_len,
				       loc, loclen, options, ctxt);

		free(loc);
		free(options);
		pthread_setcancelstate(cur_state, NULL);
	}
	return rv;
}