static void output_csv_flush(struct output_data_t *out)
{
	int i;
	for(i=0;i<out->columns;++i) {
		if (i!=0) printf(",");
		switch (out->data[i].type) {
			case TYPE_int: 
				printf("%" PRIu64,out->data[i].d.d_int);
				break;
			case TYPE_str:
				printf("%s",out->data[i].d.d_str);
				free(out->data[i].d.d_str);
				break;
			case TYPE_float:
				printf("%f",out->data[i].d.d_float);
				break;
			case TYPE_time:
				printf("%.03f",out->data[i].d.d_time);
				break;
		}
	}
	printf("\n");
}