static void
factorize_hwtable (vector hw, int n)
{
	struct hwentry *hwe1, *hwe2;
	int i, j;

restart:
	vector_foreach_slot(hw, hwe1, i) {
		if (i == n)
			break;
		j = n;
		vector_foreach_slot_after(hw, hwe2, j) {
			if (hwe_regmatch(hwe1, hwe2))
				continue;
			/* dup */
			merge_hwe(hwe2, hwe1);
			if (hwe_strmatch(hwe2, hwe1) == 0) {
				vector_del_slot(hw, i);
				free_hwe(hwe1);
				n -= 1;
				/*
				 * Play safe here; we have modified
				 * the original vector so the outer
				 * vector_foreach_slot() might
				 * become confused.
				 */
				goto restart;
			}
		}
	}
	return;
}
