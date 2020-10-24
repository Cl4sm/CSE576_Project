parse_command(char *command, char ***argv, int *argc)
{
    LinkedList *list = NULL;
    char *token, *line;
    int count, i;

    line = command;
    do {
	token = next_token(line, &line);
	if (token) {	    
	    list = list_cons(token, list);
	}
    } while (token!=NULL && line!=NULL);

    count = list_length(list);
    *argv = malloc(sizeof(char*)*count);
    i = count;
    while (list!=NULL) {
	(*argv)[--i] = list->head;
	list_remove_head(&list);
    }
    *argc = count;
}
