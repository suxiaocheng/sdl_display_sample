#include <SDL.h>
#include <unistd.h>
#include "jpeglib.h"
SDL_Window *g_pWindow = 0;
SDL_Renderer *g_pRenderer = 0;
int main(int argc, char *args[])
{
	int ret;
	SDL_Texture* m_pTexture; // the new SDL_Texture variable
	SDL_Rect m_sourceRectangle; // the first rectangle
	SDL_Rect m_destinationRectangle; // another rectangle
	SDL_Surface* pTempSurface = NULL;

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
	// if succeeded create our window
		g_pWindow = SDL_CreateWindow("Chapter 1: Setting up SDL",
					     SDL_WINDOWPOS_CENTERED,
					     SDL_WINDOWPOS_CENTERED, 1280, 720,
					     SDL_WINDOW_SHOWN);
	// if the window creation succeeded create our renderer
		if (g_pWindow != 0) {
			g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
		}
	} else {
		return 1;	// sdl could not initialize
	}
	// everything succeeded lets draw the window
	// set to black // This function expects Red, Green, Blue and
	// Alpha as color values
#if 1
	{
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
		FILE * infile;
		JSAMPARRAY buffer;
		int row_stride;
		const char *filename="color_bar.jpeg";//"test.jpg";
		unsigned char *file_buf;
		unsigned long file_size = 0;
		unsigned long file_size_read = 0;
		unsigned char *display_buf;

		if ((infile = fopen(filename, "rb")) == NULL) {
			fprintf(stderr, "can't open %s\n", filename);
			exit(1);
		}

		cinfo.err = jpeg_std_error(&jerr);
		//jerr.pub.error_exit = my_error_exit;
		/*if (setjmp(jerr.setjmp_buffer)) {
			jpeg_destroy_decompress(&cinfo);
			fclose(infile);
			return 0;
		}*/
		jpeg_create_decompress(&cinfo);

#if 1
		jpeg_stdio_src(&cinfo, infile);
#else
		fseek(infile, 0, SEEK_END);
		file_size = ftell(infile);
		printf("Jpeg file size: %ld\n", file_size);
		file_buf = (unsigned char *)malloc(file_size);
		if (file_buf == NULL) {
			printf("file_buf is null\n");
		}
		fseek(infile, 0, SEEK_SET);
		file_size_read = fread(file_buf, file_size, 1, infile);
		if(file_size_read != 1) {
			 printf("file_buf is not equal, %ld != %ld\n", 
					 file_size_read, file_size);
		}
		jpeg_mem_src(&cinfo, file_buf, file_size);
#endif
		(void) jpeg_read_header(&cinfo, TRUE);
		// cinfo.out_color_space=JCS_RGB;
		(void) jpeg_start_decompress(&cinfo);

		row_stride = cinfo.output_width * cinfo.output_components;
		printf("r: %d, w: %d, ppb: %d, type: %d\n", row_stride, cinfo.output_width,
				cinfo.output_components, cinfo.out_color_space);
		//sleep(1);
		buffer = (*cinfo.mem->alloc_sarray)
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);


		// create the sdl surface for display
		{
			int Amask = 0xFF000000;
			int Rmask = 0x00FF0000;
			int Gmask = 0x0000FF00;
			int Bmask = 0x000000FF;
			int i;
			int ret;
			unsigned char *pbuf;
			unsigned int *pp;
			pTempSurface = SDL_CreateRGBSurface(0, cinfo.output_width,
				       cinfo.output_height, 32, 0, 0, 0, 0);
			pp = pTempSurface->pixels;
			while (cinfo.output_scanline < cinfo.output_height) {
				(void) jpeg_read_scanlines(&cinfo, buffer, 1);//TODO
				pbuf = buffer;
				pp = pTempSurface->pixels + (cinfo.output_scanline - 1) * cinfo.output_width * 4;
				for(i=0; i<cinfo.output_width; i++) {
#if 0
					*pp = ((unsigned int)pbuf[0]<<16) | 
						((unsigned int)(pbuf[1])<<8) | 
						((unsigned int)(pbuf[2])<<0) |
						((unsigned int)0xff <<24);
#else
#if 0
					if(i<cinfo.output_width/3){
						*pp = 0x000000ff;
					} else if(i<cinfo.output_width/3*2){
						*pp = 0x0000ff00;
					} else {
						*pp = 0x00ff0000;
					}
#else
					unsigned char val = i*255/cinfo.output_width;
					*pp = (val<<16) | (val<<8) | val;
#endif
#endif
					pp++;
					pbuf+=3;
				}
				// put_scanline_someplace(buffer[0], row_stride);
			}
			printf("last output line is: %d\n", cinfo.output_scanline);
		}
		(void) jpeg_finish_decompress(&cinfo);

		jpeg_destroy_decompress(&cinfo);
		free(file_buf);
		fclose(infile);
	}
#else
		
	SDL_Surface* pTempSurface = SDL_LoadBMP("./test.bmp"); 
	if (pTempSurface == NULL) { 
		printf("Load BMP fail: %s\n", SDL_GetError()); 
	}
#endif	
	m_pTexture = SDL_CreateTextureFromSurface(g_pRenderer,
			pTempSurface);
	if (m_pTexture == NULL) { 
		printf("SDL_CreateTextureFromSurface fail: %s\n", SDL_GetError()); 
	}
	SDL_FreeSurface(pTempSurface);

	ret = SDL_QueryTexture(m_pTexture, NULL, NULL,
			&m_sourceRectangle.w, &m_sourceRectangle.h);
	if(ret < 0) {
		printf("SDL_QueryTexture fail: %s\n", SDL_GetError()); 
	} else {
		printf("width: %d, height: %d\n",  m_sourceRectangle.w,
				m_sourceRectangle.h);
	}

	m_destinationRectangle.x = m_sourceRectangle.x = 0;
	m_destinationRectangle.y = m_sourceRectangle.y = 0;
	m_destinationRectangle.w = m_sourceRectangle.w;
	m_destinationRectangle.h = m_sourceRectangle.h;

	SDL_RenderCopy(g_pRenderer, m_pTexture, &m_sourceRectangle,
			&m_destinationRectangle);
	
	// SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
	// clear the window to black
	// SDL_RenderClear(g_pRenderer);
	// show the window

	SDL_RenderPresent(g_pRenderer);
	// set a delay before quitting
	SDL_Delay(5000);
	// clean up SDL
	SDL_Quit();
	return 0;
}
