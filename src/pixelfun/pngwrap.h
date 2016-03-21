#ifndef PNGWRAP_H
#define PNGWRAP_H

//#include <png/png.h>
//
//#define BYTE unsigned char
//#define DWORD unsigned int
//
//struct PNGImage
//{
//	int width;
//	int height;
//	DWORD* data;
//};
//
//DWORD* LoadPNG(char* filename, int& width, int& height, DWORD*& data)
//{
//	FILE *fp=fopen(filename, "rb"); if(!fp) return false; // open file
//	png_structp png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); if(!png_ptr) return NULL;
//	png_infop info_ptr=png_create_info_struct(png_ptr);
//	if(!info_ptr) { png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL); return NULL; }
//	png_init_io(png_ptr, fp); png_read_info(png_ptr, info_ptr); png_read_update_info(png_ptr, info_ptr);
//	width=info_ptr->width; height=info_ptr->height; int channels=info_ptr->channels; // get image info
//	BYTE *rgb_image=(BYTE*)malloc(width*height*channels); // temporary buffer
//	data=(DWORD*)malloc(width*height*sizeof(DWORD)); // target buffer in image
//	png_byte **row_pointers=(png_byte**)malloc(height*sizeof(png_byte*));
//	for(int i=0;i<height;i++) row_pointers[i]=(png_byte*)(rgb_image+i*width*channels);
//	png_set_rows(png_ptr, info_ptr, (png_bytepp)(&row_pointers));
//	png_read_image(png_ptr, row_pointers); png_read_end(png_ptr, NULL); // read png image
//	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL); fclose(fp); free(row_pointers);
//	DWORD* dp=data; int si=0; // destination pointer and source pixel index
//	for(int i=0;i<width*height;i++) // copy 24-bit pixel data to 32-bit image buffer
//	{
//		DWORD pixel=0xFF000000|(rgb_image[si+0]<<16)|(rgb_image[si+1]<<8)|rgb_image[si+2]; // use opaque alpha
//		*dp++=pixel; si+=channels;
//	}
//	free(rgb_image); return data;
//}
//
//DWORD* LoadPNG(char* filename, PNGImage& image)
//{
//	return LoadPNG(filename, image.width, image.height, image.data);
//}

#endif

