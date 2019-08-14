#include <SDL.h>
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

		jpeg_stdio_src(&cinfo, infile);
		jpeg_read_header(&cinfo, TRUE);
		jpeg_start_decompress(&cinfo);

		row_stride = cinfo.output_width * cinfo.output_components;
		printf("r: %d, w: %d, ppb: %d\n", row_stride, cinfo.output_width,
				cinfo.output_components);
		buffer = (*cinfo.mem->alloc_sarray)
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);


		// create the sdl surface for display
		{
			int Amask = 0xFF000000;
			int Rmask = 0x00FF0000;
			int Gmask = 0x0000FF00;
			int Bmask = 0x000000FF;
			int i;
			unsigned char *pbuf;
			unsigned int *pp;
			pTempSurface = SDL_CreateRGBSurface(0, cinfo.output_width,
				       cinfo.output_height, 32, 0, 0, 0, 0);
			pp = pTempSurface->pixels;
			while (cinfo.output_scanline < cinfo.output_height) {
				jpeg_read_scanlines(&cinfo, buffer, 1);//TODO
				pbuf = buffer;
				for(i=0; i<cinfo.output_width; i++) {
#if 1
					*pp = ((unsigned int)pbuf[0]<<16) | 
						((unsigned int)(pbuf[1])<<8) | 
						((unsigned int)(pbuf[2])<<0) |
						((unsigned int)0 <<24);
#else
					if(i<cinfo.output_width/3){
						*pp = 0x000000ff;
					} else if(i<cinfo.output_width/3*2){
						*pp = 0x0000ff00;
					} else {
						*pp = 0x00ff0000;
					}
#endif
					pp++;
					pbuf+=3;
				}
				// put_scanline_someplace(buffer[0], row_stride);
			}
		}
		jpeg_finish_decompress(&cinfo);

		jpeg_destroy_decompress(&cinfo);
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
