
#ifndef PHYSICA_MAIN_H
#define PHYSICA_MAIN_H

using namespace std;
// CIEXYZTRIPLE stuff
typedef int FXPT2DOT30;

typedef struct {
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct {
    CIEXYZ  ciexyzRed;
    CIEXYZ  ciexyzGreen;
    CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;

// bitmap file header
typedef struct {
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
} BITMAPFILEHEADER;

// bitmap info header
typedef struct {
    unsigned int   biSize;
    unsigned int   biWidth;
    unsigned int   biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    unsigned int   biXPelsPerMeter;
    unsigned int   biYPelsPerMeter;
    unsigned int   biClrUsed; // количесвто цветов(если естьь таблица)
    unsigned int   biClrImportant;
    unsigned int   biRedMask;  // маски
    unsigned int   biGreenMask;
    unsigned int   biBlueMask;
    unsigned int   biAlphaMask;
    unsigned int   biCSType;
    CIEXYZTRIPLE   biEndpoints;
    unsigned int   biGammaRed;
    unsigned int   biGammaGreen;
    unsigned int   biGammaBlue;
    unsigned int   biIntent;
    unsigned int   biProfileData;
    unsigned int   biProfileSize;
    unsigned int   biReserved;
} BITMAPINFOHEADER;

// rgb quad
typedef struct {
    unsigned char  rgbBlue;
    unsigned char  rgbGreen;
    unsigned char  rgbRed;
    unsigned char  rgbReserved;
} RGBQUAD;

// read bytes
template <typename Type>
void read(std::ifstream &fp, Type &result, std::size_t size) {
    fp.read(reinterpret_cast<char*>(&result), size);
}
template <typename Type>
void write(ofstream &fp, Type &result, ::size_t size){
    fp.write(reinterpret_cast<char*>(&result),size);
}

// bit extract
unsigned char bitextract(const unsigned int byte, const unsigned int mask);
unsigned int bitextrevers(const unsigned char byte_r,
                          const unsigned char byte_g,
                          const unsigned char byte_b,
                          const unsigned char byte_alpha,
                          const unsigned int mask_r,
                          const unsigned int mask_g,
                          const unsigned int mask_b,
                          const unsigned int mask_alpha);


#endif //PHYSICA_MAIN_H
