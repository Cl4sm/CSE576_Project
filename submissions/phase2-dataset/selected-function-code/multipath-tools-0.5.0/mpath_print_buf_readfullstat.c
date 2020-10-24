{

	int i,j, num;
	uint64_t  prkey;
	uint16_t  rel_pt_addr;
	unsigned char * keyp;

	num = pr_buff->prin_descriptor.prin_readfd.number_of_descriptor;	
	if (0 == num)
	{
		printf("  PR generation=0x%x \n", pr_buff->prin_descriptor.prin_readfd.prgeneration);
		return ;
	}
	else
		printf("  PR generation=0x%x \n", pr_buff->prin_descriptor.prin_readfd.prgeneration);

	for (i = 0 ; i < num; i++)
	{
		keyp = (unsigned  char *)&pr_buff->prin_descriptor.prin_readfd.descriptors[i]->key;

		prkey = 0;
		for (j = 0; j < 8; ++j) {
			if (j > 0)
				prkey <<= 8;
			prkey |= *keyp;
			++keyp;
		}
		printf("   Key = 0x%" PRIx64 "\n", prkey);

		if (pr_buff->prin_descriptor.prin_readfd.descriptors[i]->flag & 0x02)	
			printf("      All target ports bit set\n");
		else {
			printf("      All target ports bit clear\n");

			rel_pt_addr = pr_buff->prin_descriptor.prin_readfd.descriptors[i]->rtpi;
			printf("      Relative port address: 0x%x\n",
					rel_pt_addr);
		}

		if (pr_buff->prin_descriptor.prin_readfd.descriptors[i]->flag & 0x1) {
			printf("      << Reservation holder >>\n");
			j = ((pr_buff->prin_descriptor.prin_readfd.descriptors[i]->scope_type>> 4) & 0xf);
			if (0 == j)
				printf("      scope: LU_SCOPE, ");
			else
				printf("      scope: %d ", j);
			j = (pr_buff->prin_descriptor.prin_readfd.descriptors[i]->scope_type & 0xf);
			printf(" type: %s\n", pr_type_strs[j]);
		} else
			printf("      not reservation holder\n");
		mpath_print_transport_id(pr_buff->prin_descriptor.prin_readfd.descriptors[i]);
	}
}
