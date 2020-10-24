int main(int argc, char *argv[])
{
	global_t globals;
	int c;

	while((c = getopt(argc, argv, "B:A:Z:F:")) != -1)
	{
		switch(c)
		{
			case 'B':
				set_interval(SCHED_BATT, scheds, atoi(optarg));
				break;

			case 'A':
				set_interval(SCHED_ACSTATE, scheds, atoi(optarg));
				break;

			case 'Z':
				set_interval(SCHED_ZONE, scheds, atoi(optarg));
				break;

			case 'F':
				set_interval(SCHED_FAN, scheds, atoi(optarg));
				break;

			case 'V':
				version();
				return 0;

			case 'h':
				help();
				return 0;

			default:
				help();
				return 1;
		}
	}

	if (check_acpi_support() == NOT_SUPPORTED)
		error_exit("This system does not support ACPI.\n");

	memset(&globals, 0x00, sizeof(globals));

	if (init_acpi_batt(&globals) == ALLOC_ERR)
		error_exit("Failed to obtain battery information.\n");

	if (init_acpi_acadapt(&globals) == ALLOC_ERR)
		error_exit("Failed to obtain AC adapter information.\n");

	if (init_acpi_thermal(&globals) == ALLOC_ERR)
		error_exit("Failed to obtain thermal areas information.\n");

	if (init_acpi_fan(&globals) == ALLOC_ERR)
		error_exit("Failed to obtain fan information.\n");

	for(;;)
	{
		int loop;
		char header_shown = 0;

		for(loop=0; loop<N_SCHED; loop++)
		{
			time_t now = time(NULL);
			time_t cur_next_event = scheds[loop].last_run + scheds[loop].interval;

			if (cur_next_event <= now)
			{
				switch(scheds[loop].what)
				{
					case SCHED_BATT:
						emit_battery(&globals, &header_shown);
						break;

					case SCHED_ACSTATE:
						emit_acstate(&globals, &header_shown);
						break;

					case SCHED_ZONE:
						emit_zone(&globals, &header_shown);
						break;

					case SCHED_FAN:
						emit_fan(&globals, &header_shown);
						break;
				}

				scheds[loop].last_run = now;
			}
		}

		sleep_untill_next_event(scheds, N_SCHED);
	}

	return 0;
}