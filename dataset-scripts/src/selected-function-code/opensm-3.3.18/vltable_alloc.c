static int vltable_alloc(vltable_t ** vltable, uint64_t size)
{
	/* allocate VL table and indexing array */
	*vltable = (vltable_t *) malloc(sizeof(vltable_t));
	if (!(*vltable))
		goto ERROR;
	(*vltable)->num_lids = size;
	(*vltable)->lids = (ib_net16_t *) malloc(size * sizeof(ib_net16_t));
	if (!((*vltable)->lids))
		goto ERROR;
	(*vltable)->vls = (uint8_t *) malloc(size * size * sizeof(uint8_t));
	if (!((*vltable)->vls))
		goto ERROR;
	memset((*vltable)->vls, OSM_DEFAULT_SL, size * size);

	return 0;

ERROR:
	vltable_dealloc(vltable);

	return 1;
}