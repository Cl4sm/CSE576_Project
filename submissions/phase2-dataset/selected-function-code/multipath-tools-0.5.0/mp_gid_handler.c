mp_gid_handler(vector strvec)
{
	gid_t gid;
	char *buff;
	char passwd_buf[1024];
	struct passwd info, *found;

	struct mpentry *mpe = VECTOR_LAST_SLOT(conf->mptable);

	if (!mpe)
		return 1;

	buff = set_value(strvec);
	if (!buff)
		return 1;

	if (getpwnam_r(buff, &info, passwd_buf, 1024, &found) == 0 && found) {
		mpe->attribute_flags |= (1 << ATTR_GID);
		mpe->gid = info.pw_gid;
	}
	else if (sscanf(buff, "%u", &gid) == 1) {
		mpe->attribute_flags |= (1 << ATTR_GID);
		mpe->gid = gid;
	}
	FREE(buff);
	return 0;
}
