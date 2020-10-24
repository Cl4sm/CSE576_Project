void mpath_print_buf_readcap( struct prin_resp *pr_buff)
{
	if ( pr_buff->prin_descriptor.prin_readcap.length <= 2 ) {
		fprintf(stderr, "Unexpected response for PRIN Report "
				"Capabilities\n");
		return; //MALFORMED;
	}

	printf("Report capabilities response:\n");

	printf("  Compatible Reservation Handling(CRH): %d\n", !!(pr_buff->prin_descriptor.prin_readcap.flags[0] & 0x10));
	printf("  Specify Initiator Ports Capable(SIP_C): %d\n",!!(pr_buff->prin_descriptor.prin_readcap.flags[0] & 0x8));
	printf("  All Target Ports Capable(ATP_C): %d\n",!!(pr_buff->prin_descriptor.prin_readcap.flags[0] & 0x4 ));
	printf("  Persist Through Power Loss Capable(PTPL_C): %d\n",!!(pr_buff->prin_descriptor.prin_readcap.flags[0]));
	printf("  Type Mask Valid(TMV): %d\n", !!(pr_buff->prin_descriptor.prin_readcap.flags[1] & 0x80));
	printf("  Allow Commands: %d\n", !!(( pr_buff->prin_descriptor.prin_readcap.flags[1] >> 4) & 0x7));
	printf("  Persist Through Power Loss Active(PTPL_A): %d\n",
			!!(pr_buff->prin_descriptor.prin_readcap.flags[1] & 0x1));

	if(pr_buff->prin_descriptor.prin_readcap.flags[1] & 0x80)
	{
		printf("    Support indicated in Type mask:\n");

		printf("      %s: %d\n", pr_type_strs[7], pr_buff->prin_descriptor.prin_readcap.pr_type_mask & 0x80);
		printf("      %s: %d\n", pr_type_strs[6], pr_buff->prin_descriptor.prin_readcap.pr_type_mask & 0x40);
		printf("      %s: %d\n", pr_type_strs[5], pr_buff->prin_descriptor.prin_readcap.pr_type_mask & 0x20);
		printf("      %s: %d\n", pr_type_strs[3], pr_buff->prin_descriptor.prin_readcap.pr_type_mask & 0x8);
		printf("      %s: %d\n", pr_type_strs[1], pr_buff->prin_descriptor.prin_readcap.pr_type_mask & 0x2);
		printf("      %s: %d\n", pr_type_strs[8], pr_buff->prin_descriptor.prin_readcap.pr_type_mask & 0x100);
	}
}
