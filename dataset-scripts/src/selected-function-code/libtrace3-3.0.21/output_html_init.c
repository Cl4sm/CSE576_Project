static void output_html_init(struct output_data_t *out)
{
	int i;
	printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n");
	printf("<html>\n");
	printf("<head>\n");
	printf(" <title>%s</title>\n",out->title);
	printf(" <style type=\"text/css\">\n");
	printf("  table         { border-collapse: collapse; width: 100%%}\n");
	printf("  td            { border: thin black solid; }\n");
	printf("  .numeric      { text-align: right; }\n");
	printf("  .even         { background: #e0e0e0; }\n");
	printf("  .odd          { background: #ffffff; }\n");
	printf("  .rowheading   { text-align: right; }\n");
	printf(" </style>\n");
	printf("</head>\n");
	printf("<body>\n");
	printf("<h1>%s</h1>\n",out->title);
	printf("<table>\n");
	printf(" <tr>\n");
	for(i=0;i<out->columns;++i) {
		printf("  <th>%s</th>",out->labels[i]);
	}
	printf(" </tr>\n");
	out->private_format_data=malloc(sizeof(int));
	*(int*)out->private_format_data=0;
}