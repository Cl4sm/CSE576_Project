void emit_zone(global_t *globals, char *header_shown)
{
	int loop;
	static int *prevs = NULL;
	static int *prevs_ts = NULL;

	if (!prevs)
	{
		int bytes = sizeof(int) * globals -> thermal_count;
		prevs = malloc(bytes);
		prevs_ts = malloc(bytes);
		memset(prevs, 0x00, bytes);
		memset(prevs_ts, 0x00, bytes);
	}

	for(loop=0; loop<globals -> thermal_count; loop++)
	{
		int rc = read_acpi_zone(loop, globals);

		if (rc == ITEM_EXCEED)
			break;
		if (rc == ALLOC_ERR)
			error_exit("reac_acpi_zone(%d) failed\n", loop);

		if (prevs[loop] != thermals[loop].temperature ||
		    prevs_ts[loop] != thermals[loop].therm_state)
		{
			header(header_shown);
			printf("temperature %s: %d (%s)\n", thermals[loop].name, thermals[loop].temperature, thermal_state_str(thermals[loop].therm_state));
			prevs[loop] = thermals[loop].temperature;
			prevs_ts[loop] = thermals[loop].therm_state;
		}
	}
}