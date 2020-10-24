{
	int j, num, scope=0, type=0;
	unsigned char *keyp;
	uint64_t prkey;	

	num = pr_buff->prin_descriptor.prin_readresv.additional_length / 8;
	if (0 == num)
	{
		printf("  PR generation=0x%x, there is NO reservation held \n", pr_buff->prin_descriptor.prin_readresv.prgeneration);
		return ;
	}
	else
		printf("  PR generation=0x%x, Reservation follows:\n", pr_buff->prin_descriptor.prin_readresv.prgeneration);
	keyp = (unsigned  char *)&pr_buff->prin_descriptor.prin_readkeys.key_list[0]; 
	prkey = 0;
	for (j = 0; j < 8; ++j) {
		if (j > 0)
			prkey <<= 8;
		prkey |= keyp[j];
	}

	printf("   Key = 0x%" PRIx64 "\n", prkey);

	scope = (pr_buff->prin_descriptor.prin_readresv.scope_type >> 4) &  0x0f;
	type = pr_buff->prin_descriptor.prin_readresv.scope_type & 0x0f;

	if (scope == 0)	
		printf("  scope = LU_SCOPE, type = %s", pr_type_strs[type]);
	else
		printf("  scope = %d, type = %s", scope, pr_type_strs[type]);

	printf("\n");

}
