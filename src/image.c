#include <stdio.h>

#include <wand/magick_wand.h>
#define BLOCK_SIZE 16

void split(int block_size, int pos_x, int pos_y) {
	MagickWand *m_wand = NULL;
	
	int width,height;
	
	MagickWandGenesis();
	
	m_wand = NewMagickWand();
	
	MagickReadImage(m_wand, "../data/panda.jpg");
	
	// Get the image's width and height
	width = MagickGetImageWidth(m_wand);
	height = MagickGetImageHeight(m_wand);
	
	// Cut them in half but make sure they don't underflow
	if((width /= 2) < 1)width = 1;
	if((height /= 2) < 1)height = 1;
	
	// Resize the image using the Lanczos filter
	// The blur factor is a "double", where > 1 is blurry, < 1 is sharp
	// I haven't figured out how you would change the blur parameter of MagickResizeImage
	// on the command line so I have set it to its default of one.
	MagickResizeImage(m_wand, width, height, LanczosFilter, 1);
	
	// Set the compression quality to 95 (high quality = low compression)
	MagickSetImageCompressionQuality(m_wand,95);
	
	/* Write the new image */
	MagickWriteImage(m_wand,"logo_resize.jpg");
	
	/* Clean up */
	if(m_wand)m_wand = DestroyMagickWand(m_wand);
	
	MagickWandTerminus();
}

int create_image(int x, int y, char* pixels) {
	
	MagickWand *m_wand = NULL;
	unsigned int status;
	
	m_wand=NewMagickWand();
	MagickSetSize(m_wand, BLOCK_SIZE, BLOCK_SIZE);
	//MagickImportImagePixels(m_wand, 0, 0, BLOCK_SIZE, BLOCK_SIZE, "RGB", CharPixel, pixels);
	
	MagickSetImageCompressionQuality(m_wand,95);	
	int res = MagickWriteImage(m_wand,"x.jpg");
	

	if(m_wand)m_wand = DestroyMagickWand(m_wand);

	return res;
	
}

void xx() {
	MagickWand *m_wand = NULL;
	
	int width,height;
		
	MagickWandGenesis();
	
	m_wand = NewMagickWand();
	
	MagickReadImage(m_wand, "../data/panda.jpg");	

	
	// Get the image's width and height
	width = MagickGetImageWidth(m_wand);
	height = MagickGetImageHeight(m_wand);


	int blocks_w = width / BLOCK_SIZE;
	int blocks_h = height / BLOCK_SIZE;

	for (int r = 0; r < blocks_h; r++) {
		for (int c = 0; c < blocks_w; c++) {
			
			unsigned char pixels[BLOCK_SIZE * BLOCK_SIZE * 3];
			//MagickExportImagePixels(m_wand, c*BLOCK_SIZE, r*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, "RGB", CharPixel, pixels);	
			int res = create_image(c, r, pixels);
			printf("block r:%d c:%d err:%d\n", r, c, res);
		}	
	}
	
	/* Clean up */
	if(m_wand)m_wand = DestroyMagickWand(m_wand);
	
	MagickWandTerminus();
}

int main() {
	printf("hello\n");
	split(16, 0, 0);
	xx();	
	return 0;
}