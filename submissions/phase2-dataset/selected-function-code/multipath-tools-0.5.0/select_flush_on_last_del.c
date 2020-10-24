extern int
select_flush_on_last_del(struct multipath *mp)
{
	if (mp->flush_on_last_del == FLUSH_IN_PROGRESS)
		return 0;
	if (mp->mpe && mp->mpe->flush_on_last_del != FLUSH_UNDEF) {
		mp->flush_on_last_del = mp->mpe->flush_on_last_del;
		condlog(3, "%s: flush_on_last_del = %i (multipath setting)",
			mp->alias, mp->flush_on_last_del);
		return 0;
	}
	if (mp->hwe && mp->hwe->flush_on_last_del != FLUSH_UNDEF) {
		mp->flush_on_last_del = mp->hwe->flush_on_last_del;
		condlog(3, "%s: flush_on_last_del = %i (controler setting)",
			mp->alias, mp->flush_on_last_del);
		return 0;
	}
	if (conf->flush_on_last_del != FLUSH_UNDEF) {
		mp->flush_on_last_del = conf->flush_on_last_del;
		condlog(3, "%s: flush_on_last_del = %i (config file default)",
			mp->alias, mp->flush_on_last_del);
		return 0;
	}
	mp->flush_on_last_del = FLUSH_UNDEF;
	condlog(3, "%s: flush_on_last_del = DISABLED (internal default)",
		mp->alias);
	return 0;
}
