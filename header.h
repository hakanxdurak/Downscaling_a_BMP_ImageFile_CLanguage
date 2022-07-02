#include <stdint.h>

typedef struct
{
    uint16_t bfType;            // Magic identifier: 0x4d42 //{'B', 'M'}
                                //'B' (0x42) , 'M' (0x4d)
    uint32_t bfSize;            //The size, in bytes, of the bitmap file.
                                //As we are creating a 480 by 720 bitmap image,
                                // total size is the sum of size of meta data (14 + 40 = 54 bytes)
                                // and size of pixels (480* 720 * 3 = 1.036.800, each pixel is of 3 bytes in 24 bit bitmap file).
    uint16_t bfReserved1;       // Not used     //0
    uint16_t bfReserved2;       // Not used     //0
    uint32_t bfOffBits;         // Offset to image data in bytes from beginning of file
                                // In our case the offset is 54 bytes, as our pixel data is right after the bitmap file header and info header (14 + 40 = 54).
} __attribute__((__packed__)) BITMAPFILEHEADER; //Total 14 bytes

typedef struct
{
    uint32_t biSize;            //Header size in bytes (40 bytes)
    int32_t  biWidth;           //Specifies the width of the bitmap, in pixels.
    int32_t  biHeight;          //Specifies the height of the bitmap, in pixels.
                                //When height is negative the first pixel in file is drawn at top left of the image.
                                //However the standard for bitmap files is to use positive height
                                //and the first pixel in file is drawn at the bottom left of the image followed by other pixels.
    uint16_t biPlanes;          // Number of color planes   // must be 1
    uint16_t biBitCount;        // Bits per pixel           //24
                                /*Bits per Pixel used to store palette entry information. This also identifies in an indirect way the number of possible colors. Possible values are:
                                1 = monochrome palette. NumColors = 1
                                4 = 4bit palletized. NumColors = 16
                                8 = 8bit palletized. NumColors = 256
                                16 = 16bit RGB. NumColors = 65536
                                24 = 24bit RGB. NumColors = 16M */
    uint32_t biCompression;     // Compression type         //0
                                /*Type of Compression
                                0 = BI_RGB   no compression
                                1 = BI_RLE8 8bit RLE encoding
                                2 = BI_RLE4 4bit RLE encoding */
    uint32_t biSizeImage;       // Image size in bytes  //Can be 0 if image is not compressed, otherwise is the size of the compressed image
    int32_t  biXPelsPerMeter;   // horizontal resolution: Pixels/meter
    int32_t  biYPelsPerMeter;   // vertical resolution: Pixels/meter
    uint32_t biClrUsed;         /*Number of actually used colors. For a 8-bit / pixel bitmap this will be 100h or 256.*/
    uint32_t biClrImportant;
                                /*Number of important colors
                                0 = all*/
} __attribute__((__packed__)) BITMAPINFOHEADER; //Total 40 bytes

//Device Independent Bitmap (DIB) = DIB format that will allow Windows to display the bitmap on any type of display device.

//Pixel Data =>
//The system maps pixels beginning with the bottom scan line of the rectangular region and ending with the top scan line.

//You may have noticed the odd __attribute__((__packed__)) in the declaration of the structures in the file, and this line instructs the compiler to not add any extra padding bytes to those structures.