static
struct link *alloc_flink(struct fabric *f)
{
	unsigned cnt_max;
	struct link *l = NULL;
	void *ptr;

	if (f->link_cnt >= f->link_cnt_max) {

		cnt_max = 16 + 5 * f->link_cnt_max / 4;
		ptr = realloc(f->link, cnt_max * sizeof(*f->link));
		if (!ptr) {
			OSM_LOG(&f->osm->log, OSM_LOG_ERROR,
				"ERR 4E04: realloc: %s\n", strerror(errno));
			goto out;
		}
		f->link = ptr;
		f->link_cnt_max = cnt_max;
		memset(&f->link[f->link_cnt], 0,
		       (f->link_cnt_max - f->link_cnt) * sizeof(*f->link));
	}
	l = calloc(1, sizeof(*l));
	if (!l) {
		OSM_LOG(&f->osm->log, OSM_LOG_ERROR,
			"ERR 4E05: calloc: %s\n", strerror(errno));
		goto out;
	}
	f->link[f->link_cnt++] = l;
out:
	return l;
}