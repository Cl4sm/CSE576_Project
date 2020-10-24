{
	int i,j,k, num;
	unsigned char *keyp;
	uint64_t prkey;
	printf("  PR generation=0x%x, ", pr_buff->prin_descriptor.prin_readkeys.prgeneration);

	num = pr_buff->prin_descriptor.prin_readkeys.additional_length / 8;
	if (0 == num) {
		printf("	0 registered reservation key.\n");
		return;
	}
	else if (1 == num)
		printf("	1 registered reservation key follows:\n");
	else
		printf("	%d registered reservation keys follow:\n", num);


	keyp = (unsigned char *)&pr_buff->prin_descriptor.prin_readkeys.key_list[0];
	for (i = 0; i < num ; i++)
	{
		prkey = 0;
		for (j = 0; j < 8; ++j) {

			if (j > 0)
				prkey <<= 8;
			prkey |= keyp[j];
		}
		printf("    0x%" PRIx64 "\n", prkey);
		k=8*i+j;
		keyp = (unsigned char *)&pr_buff->prin_descriptor.prin_readkeys.key_list[k];
	}
}
