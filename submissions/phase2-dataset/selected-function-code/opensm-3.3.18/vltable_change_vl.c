static void vltable_change_vl(vltable_t * vltable, uint8_t from, uint8_t to,
			      uint64_t count)
{
	uint64_t set = 0, stop = 0;
	uint64_t ind1 = 0, ind2 = 0;

	for (ind1 = 0; ind1 < vltable->num_lids; ind1++) {
		for (ind2 = 0; ind2 < vltable->num_lids; ind2++) {
			if (set == count) {
				stop = 1;
				break;
			}
			if (ind1 != ind2) {
				if (vltable->
				    vls[ind1 + ind2 * vltable->num_lids] ==
				    from) {
					vltable->vls[ind1 +
						     ind2 * vltable->num_lids] =
					    to;
					set++;
				}
			}
		}
		if (stop)
			break;
	}
}