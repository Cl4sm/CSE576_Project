	int cmd_result;
	pid_t child = fork();
	switch(child){
	case 0:			// child
		cmd_result = system(command);
		if(cmd_result==-1) perror("fork_and_exec");
		if(cmd_result!=0){
			fprintf(stderr, "warning: system(%s) returned %d\n",
					command, cmd_result);
		}
		exit(cmd_result);
		break;
	case -1:
		perror("unable to fork in fork_and_exec");
		break;
	default: break;
	}
	return child;
}
