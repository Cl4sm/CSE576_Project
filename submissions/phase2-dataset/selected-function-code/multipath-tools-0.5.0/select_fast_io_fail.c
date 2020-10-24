extern int
select_fast_io_fail(struct multipath *mp)
{
	if (mp->hwe && mp->hwe->fast_io_fail != MP_FAST_IO_FAIL_UNSET) {
		mp->fast_io_fail = mp->hwe->fast_io_fail;
		if (mp->fast_io_fail == MP_FAST_IO_FAIL_OFF)
			condlog(3, "%s: fast_io_fail_tmo = off "
				"(controller setting)", mp->alias);
		else
			condlog(3, "%s: fast_io_fail_tmo = %d "
				"(controller setting)", mp->alias,
				mp->fast_io_fail == MP_FAST_IO_FAIL_ZERO ? 0 : mp->fast_io_fail);
		return 0;
	}
	if (conf->fast_io_fail != MP_FAST_IO_FAIL_UNSET) {
		mp->fast_io_fail = conf->fast_io_fail;
		if (mp->fast_io_fail == MP_FAST_IO_FAIL_OFF)
			condlog(3, "%s: fast_io_fail_tmo = off "
				"(config file default)", mp->alias);
		else
			condlog(3, "%s: fast_io_fail_tmo = %d "
				"(config file default)", mp->alias,
				mp->fast_io_fail == MP_FAST_IO_FAIL_ZERO ? 0 : mp->fast_io_fail);
		return 0;
	}
	mp->fast_io_fail = MP_FAST_IO_FAIL_UNSET;
	return 0;
}
