void print_verifying(int cylinder, int head)
{
	if (verbosity >= 5) {
		printf("verifying cylinder %d head %d\n",
			cylinder, head);
	} else if (verbosity >= 3) {
		printf("\r Verifying cylinder %2d, head %d ", cylinder, head);
		fflush(stdout);
	} else if (verbosity == 2) {
		if (!verify_later && !dosverify) {
			if (head)
				printf("\b+");
			else
				printf("\bx");
		} else {
			if (head)
				printf("\b+");
			else
				printf("x");
		}
		fflush(stdout);
	}
}
