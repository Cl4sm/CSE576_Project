struct substvar *addstdenv(struct substvar *sv, const char *prefix)
{
	struct substvar *list = sv;
	struct thread_stdenv_vars *tsv;
	char numbuf[16];

	tsv = pthread_getspecific(key_thread_stdenv_vars);
	if (tsv) {
		const struct substvar *mv;
		int ret;
		long num;

		num = (long) tsv->uid;
		ret = sprintf(numbuf, "%ld", num);
		if (ret > 0)
			list = do_macro_addvar(list, prefix, "UID", numbuf);
		num = (long) tsv->gid;
		ret = sprintf(numbuf, "%ld", num);
		if (ret > 0)
			list = do_macro_addvar(list, prefix, "GID", numbuf);
		list = do_macro_addvar(list, prefix, "USER", tsv->user);
		list = do_macro_addvar(list, prefix, "GROUP", tsv->group);
		list = do_macro_addvar(list, prefix, "HOME", tsv->home);
		mv = macro_findvar(list, "HOST", 4);
		if (mv) {
			char *shost = strdup(mv->val);
			if (shost) {
				char *dot = strchr(shost, '.');
				if (dot)
					*dot = '\0';
				list = do_macro_addvar(list,
						       prefix, "SHOST", shost);
				free(shost);
			}
		}
	}
	return list;
}