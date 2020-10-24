static int
def_uid_handler(vector strvec)
{
	uid_t uid;
	char *buff;
	char passwd_buf[1024];
	struct passwd info, *found;

	buff = set_value(strvec);
	if (!buff)
		return 1;
	if (getpwnam_r(buff, &info, passwd_buf, 1024, &found) == 0 && found) {
		conf->attribute_flags |= (1 << ATTR_UID);
		conf->uid = info.pw_uid;
	}
	else if (sscanf(buff, "%u", &uid) == 1){
		conf->attribute_flags |= (1 << ATTR_UID);
		conf->uid = uid;
	}

	FREE(buff);
	return 0;
}
