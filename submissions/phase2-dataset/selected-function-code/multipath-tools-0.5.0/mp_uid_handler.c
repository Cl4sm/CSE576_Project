static int
mp_uid_handler(vector strvec)
{
	uid_t uid;
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
		mpe->attribute_flags |= (1 << ATTR_UID);
		mpe->uid = info.pw_uid;
	}
	else if (sscanf(buff, "%u", &uid) == 1){
		mpe->attribute_flags |= (1 << ATTR_UID);
		mpe->uid = uid;
	}
	FREE(buff);
	return 0;
}
