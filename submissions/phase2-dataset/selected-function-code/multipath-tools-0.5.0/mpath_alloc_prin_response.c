{
	void * ptr = NULL;
	int size=0;
	switch (prin_sa)
	{
		case MPATH_PRIN_RKEY_SA:
			size = sizeof(struct prin_readdescr);
			ptr = malloc(size);
			memset(ptr, 0, size);
			break;
		case MPATH_PRIN_RRES_SA:
			size = sizeof(struct prin_resvdescr);
			ptr = malloc(size);
			memset(ptr, 0, size);
			break;
		case MPATH_PRIN_RCAP_SA:
			size=sizeof(struct prin_capdescr);
			ptr = malloc(size);
			memset(ptr, 0, size);
			break;
		case MPATH_PRIN_RFSTAT_SA:
			size = sizeof(struct print_fulldescr_list) + 
				sizeof(struct prin_fulldescr *)*MPATH_MX_TIDS;
			ptr = malloc(size);
			memset(ptr, 0, size);
			break;
	}
	return ptr;
}
