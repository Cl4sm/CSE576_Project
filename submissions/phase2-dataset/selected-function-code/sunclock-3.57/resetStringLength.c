void
resetStringLength(max, entry)
int max;
TextEntry * entry;
{
        int a;

	entry->maxlength = max;

	a = (entry->string == NULL);
       	entry->string = (char *)
           realloc(entry->string, (max+2)*sizeof(char));
        if (a) {
	   *entry->string = '\0';
	   entry->caret = 0;
	}
	if (entry->caret > max) {
	   entry->string[max] = '\0';
	   entry->caret = max;
	}
}
