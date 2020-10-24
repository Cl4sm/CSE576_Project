static void output_png_destroy(struct output_data_t *out)
{
	struct private_png_t *prv=out->private_format_data;
	int i,j;
	char *labels[prv->rows];
	float data1[(out->columns-1)/2][prv->rows];
	float data2[(out->columns-1)/2][prv->rows];
	for(i=0;i<prv->rows;++i) {
		if (asprintf(&labels[i],"%i",(int)prv->data[i*out->columns])==-1) {
			err(1,"Out of memory");
		}
		for(j=0;j<(out->columns-1)/2;++j) {
			data1[j][i]=prv->data[i*out->columns+j*2+1];
			data2[j][i]=prv->data[i*out->columns+j*2+2];
		}
	}

	GDC_image_type = GDC_PNG;
	GDC_title = out->title;
	GDC_out_graph( 640, 480,
			stdout,
			GDC_COMBO_LINE_LINE,
			prv->rows,
			labels,	
			(out->columns-1)/2,
			(float*)data1,
			(float*)data2);
	free(prv->data);
	free(prv);
}