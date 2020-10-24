static void remove_empty_args(char **argv, int *argc)
{
	int next_to_last = *argc - 1;
	int i, j;

	for (i = j = 1; i < *argc; i++) {
		if (*argv[i]) {
			j++;
			continue;
		}

		while (i < *argc && argv[i] && !*argv[i]) i++;

		if (i == *argc)
			break;

		if (i == next_to_last) {
			if (*argv[i])
				argv[j++] = argv[i];
			break;
		} else {
			argv[j++] = argv[i];
			argv[i--] = "";
		}
	}
	*argc = j;
}