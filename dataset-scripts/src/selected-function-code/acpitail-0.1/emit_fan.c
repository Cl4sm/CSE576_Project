void emit_fan(global_t *globals, char *header_shown)
{
	int loop;
	static int *prevs = NULL;

	if (!prevs)
	{
		int bytes = sizeof(int) * globals -> fan_count;
		prevs = malloc(bytes);
		memset(prevs, 0x00, bytes);
	}

	for(loop=0; loop<globals -> fan_count; loop++)
	{
		int rc = read_acpi_fan(loop);

		if (rc == ITEM_EXCEED)
			break;
		if (rc == ALLOC_ERR)
			error_exit("reac_acpi_fan(%d) failed\n", loop);

		if (prevs[loop] != fans[loop].fan_state)
		{
			header(header_shown);
			printf("fan %s: %s\n", fans[loop].name, fan_state_str(fans[loop].fan_state));
			prevs[loop] = fans[loop].fan_state;
		}
	}
}