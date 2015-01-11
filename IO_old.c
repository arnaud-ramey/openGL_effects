/*
 * IO.c
 *
 *  Created on: Sep 17, 2009
 */

static unsigned char *loadJPEG(const char *filename, unsigned int *w,
		unsigned int *h, unsigned int *components) {

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * infile;
	unsigned char *buffer, *ptr;
	unsigned int nval;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	if ((infile = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "Can't open '%s'\n", filename);
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}

	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	*w = cinfo.output_width;
	*h = cinfo.output_height;
	*components = cinfo.output_components;

	nval = cinfo.output_width * cinfo.output_components * cinfo.output_height;
	buffer = (unsigned char *) malloc(sizeof(unsigned char) * nval);
	if (!buffer) {
		fprintf(stderr, "Out of memory\n");
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return NULL;
	}

	/* We want to flip the image over the x-axis, so start reading at
	 the last row and move backwards. */

	ptr = buffer + (cinfo.output_width * cinfo.output_components)
			* (cinfo.output_height - 1);

	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, &ptr, 1);
		ptr -= cinfo.output_width * cinfo.output_components;
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	return buffer;
}

static unsigned char *loadBMP(const char *filename, unsigned int *w,
		unsigned int *h, unsigned int *components) {
	Bitmap bitmap( filename);
	*w = bitmap.width;
	*h = bitmap.height;
	*components = 3;
	unsigned char *data = new unsigned char[bitmap.width * bitmap.height * 3];
	memcpy(data, bitmap.data, bitmap.width * bitmap.height * 3);
	return data;
}

