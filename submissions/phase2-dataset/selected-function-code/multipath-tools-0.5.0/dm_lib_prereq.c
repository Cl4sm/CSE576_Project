dm_lib_prereq (void)
{
	char version[64];
	int v[3];
#ifdef LIBDM_API_COOKIE
	int minv[3] = {1, 2, 38};
#else
	int minv[3] = {1, 2, 8};
#endif

	dm_get_library_version(version, sizeof(version));
	condlog(3, "libdevmapper version %s", version);
	sscanf(version, "%d.%d.%d ", &v[0], &v[1], &v[2]);

	if VERSION_GE(v, minv)
		return 0;
	condlog(0, "libdevmapper version must be >= %d.%.2d.%.2d",
		minv[0], minv[1], minv[2]);
	return 1;
}
