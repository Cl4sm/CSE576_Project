static void create_output(char *title) {
	int i;
	
	output=output_init(title,output_format?output_format:DEFAULT_OUTPUT_FMT);
	if (!output) {
		fprintf(stderr,"Failed to create output file\n");
		return;
	}
	output_add_column(output,"ts");
	output_add_column(output,"packets");
	output_add_column(output,"bytes");
	for(i=0;i<filter_count;++i) {
		char buff[1024];
		snprintf(buff,sizeof(buff),"%s packets",filters[i].expr);
		output_add_column(output,buff);
		snprintf(buff,sizeof(buff),"%s bytes",filters[i].expr);
		output_add_column(output,buff);
	}
	output_flush_headings(output);

}