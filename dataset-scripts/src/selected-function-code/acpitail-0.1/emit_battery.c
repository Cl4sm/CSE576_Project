void emit_battery(global_t *globals, char *header_shown)
{
	int loop;
	static int *prevs = NULL;

	if (!prevs)
	{
		int bytes = sizeof(int) * globals -> batt_count;
		prevs = malloc(bytes);
		memset(prevs, 0x00, bytes);
	}

	for(loop=0; loop<globals -> batt_count; loop++)
	{
		int rc = read_acpi_batt(loop);

		if (rc == ITEM_EXCEED)
			break;
		if (rc == ALLOC_ERR)
			error_exit("reac_acpi_batt(%d) failed\n", loop);

		if (prevs[loop] != batteries[loop].percentage)
		{
			header(header_shown);
			printf("%s remaining capacity: %d%% (%d minutes)\n", batteries[loop].name, batteries[loop].percentage, batteries[loop].remaining_time);
			prevs[loop] = batteries[loop].percentage;
		}
	}
}