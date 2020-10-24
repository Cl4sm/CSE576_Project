static int
def_gid_handler(vector strvec)
{
	gid_t gid;
	char *buff;
	char passwd_buf[1024];
	struct passwd info, *found;

	buff = set_value(strvec);
	if (!buff)
		return 1;

	if (getpwnam_r(buff, &info, passwd_buf, 1024, &found) == 0 && found) {
		conf->attribute_flags |= (1 << ATTR_GID);
		conf->gid = info.pw_gid;
	}
	else if (sscanf(buff, "%u", &gid) == 1){
		conf->attribute_flags |= (1 << ATTR_GID);
		conf->gid = gid;
	}
	FREE(buff);
	return 0;
}
