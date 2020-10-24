{
	int i;
	int sectors;

	sectors= fd->nssect = fd->dsect;

	fd->length = fd->raw_capacity;
	fd->chunksize = 0x6c + 574;

	fd->need_init = 0;
	fd->sequence = SafeNewArray(fd->dsect,struct fparm2);

	fd->sizecode = 2;
	if ( fd->rate & 0x40 )
		fd->fmt_gap = 0x54;
	else
		fd->fmt_gap = 0x6c;
	fd->min = 0;

	for(i=0; i<sectors; i++){
		fd->sequence[i].sect = i+1;
		fd->sequence[i].size = 2;
		fd->sequence[i].offset = i;
	}
}
