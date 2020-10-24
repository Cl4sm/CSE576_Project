execCommand(char *command)
{
    pid_t pid;
    char **argv;
    int argc;

    parse_command(command, &argv, &argc);
    
    if (argv==NULL) {
        return 0;
    }
    
    if ((pid=fork())==0) {
        char **args;
        int i;
        
        args = malloc(sizeof(char*)*(argc+1));
        if (!args)
          exit(10);
        for (i=0; i<argc; i++) {
            args[i] = argv[i];
        }
        args[argc] = NULL;
        execvp(argv[0], args);
        exit(10);
    }
    return pid;
}
