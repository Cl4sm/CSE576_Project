static int
dm_drv_prereq (void)
{
	unsigned int minv[3] = {1, 0, 3};
	unsigned int version[3] = {0, 0, 0};
        unsigned int * v = version;

	if (dm_drv_version(v, TGT_MPATH)) {
		/* in doubt return not capable */
		return 1;
	}

	/* test request based multipath capability */
	condlog(3, "DM multipath kernel driver v%u.%u.%u",
		v[0], v[1], v[2]);

	if VERSION_GE(v, minv)
		return 0;

	condlog(0, "DM multipath kernel driver must be >= v%u.%u.%u",
		minv[0], minv[1], minv[2]);
	return 1;
}
