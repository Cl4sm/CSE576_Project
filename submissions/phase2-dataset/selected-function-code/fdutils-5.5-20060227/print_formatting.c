{
	switch(verbosity) {
		case 0:
			break;
		case 1:
			if (!head) {
				printf(".");
				fflush(stdout);
			}
			break;
		case 2:
			if (head)
				printf("\b=");
			else
				printf("-");
			fflush(stdout);
			break;
		case 3:
		case 4:
			printf("\rFormatting cylinder %2d, head %d ",
				cylinder, head);
			fflush(stdout);
			break;
		default:
			printf("formatting cylinder %d, head %d\n",
				cylinder, head);
			break;
	}
}
