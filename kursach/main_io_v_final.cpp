//#include "mainwindow.h"
//#include <QApplication>
#include <iostream>
#include <fstream>

#include "main.h"

/*
 padding ==3 : buf-2
 */


using namespace std;

RGBQUAD ** read_file(BITMAPFILEHEADER *fileHeader,
              BITMAPINFOHEADER *fileInfoHeader,
              const char fileName[]);

int write_file (BITMAPFILEHEADER *fileHeader,
                BITMAPINFOHEADER *fileInfoHeader,
                RGBQUAD ** rgbInfo,
                const char fileName[]);

int main()
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    //return a.exec();
/*
    if (argc < 2) {
         cout << "Usage: " << argv[0] << " file_name" << endl;
         return 0;
     }

     //char *fileName = argv[1];
    char fileName[] ="/home/kot/qt_project/amin/bmp/lena.bmp";
*/
    int suces =7;
    BITMAPFILEHEADER fileHeader={};
    BITMAPINFOHEADER fileInfoHeader={};
    RGBQUAD **rgbtmp=read_file(&fileHeader,&fileInfoHeader,"/home/kot/etu_v2.0/cpp/lena.bmp");  // выделить ли память?
    suces = write_file(&fileHeader,&fileInfoHeader,rgbtmp,"/home/kot/etu_v2.0/cpp/test.bmp");
    cout << "успешно " << suces << endl;

    return 0;
}

RGBQUAD ** read_file(BITMAPFILEHEADER *fileHeader,
              BITMAPINFOHEADER *fileInfoHeader,
              const char fileName[]) {
    //char *fileName = argv[1];
    //char fileName[] ="/home/kot/qt_project/amin/bmp/lena.bmp";

    // открываем файл
    ifstream fileStream;
    fileStream.open(fileName, ifstream::binary);
    //std::ifstream fileStream(fileName, std::ifstream::binary);

    if (!fileStream) {
        cout << "Error opening file '" << fileName << "'." << endl;
        return nullptr;
    }

    // заголовк изображения
    //BITMAPFILEHEADER fileHeader;
    read(fileStream, fileHeader->bfType, sizeof(fileHeader->bfType));
    read(fileStream, fileHeader->bfSize, sizeof(fileHeader->bfSize));
    read(fileStream, fileHeader->bfReserved1, sizeof(fileHeader->bfReserved1));
    read(fileStream, fileHeader->bfReserved2, sizeof(fileHeader->bfReserved2));
    read(fileStream, fileHeader->bfOffBits, sizeof(fileHeader->bfOffBits));

    if (fileHeader->bfType != 0x4D42) {
        cout << "Error: '" << fileName << "' is not BMP file." << endl;
        return nullptr;
    }

    // информация изображения
    //BITMAPINFOHEADER fileInfoHeader;
    read(fileStream, fileInfoHeader->biSize, sizeof(fileInfoHeader->biSize));

    // bmp core
    if (fileInfoHeader->biSize >= 12) {
        read(fileStream, fileInfoHeader->biWidth, sizeof(fileInfoHeader->biWidth));
        read(fileStream, fileInfoHeader->biHeight, sizeof(fileInfoHeader->biHeight));
        read(fileStream, fileInfoHeader->biPlanes, sizeof(fileInfoHeader->biPlanes));
        read(fileStream, fileInfoHeader->biBitCount, sizeof(fileInfoHeader->biBitCount));
    }

    // получаем информацию о битности
    int colorsCount = fileInfoHeader->biBitCount >> 3;
    if (colorsCount < 3) {
        colorsCount = 3;
    }

    int bitsOnColor = fileInfoHeader->biBitCount / colorsCount;
    int maskValue = (1 << bitsOnColor) - 1;

    // bmp v1
    if (fileInfoHeader->biSize >= 40) {
        read(fileStream, fileInfoHeader->biCompression, sizeof(fileInfoHeader->biCompression));
        read(fileStream, fileInfoHeader->biSizeImage, sizeof(fileInfoHeader->biSizeImage));
        read(fileStream, fileInfoHeader->biXPelsPerMeter, sizeof(fileInfoHeader->biXPelsPerMeter));
        read(fileStream, fileInfoHeader->biYPelsPerMeter, sizeof(fileInfoHeader->biYPelsPerMeter));
        read(fileStream, fileInfoHeader->biClrUsed, sizeof(fileInfoHeader->biClrUsed));
        read(fileStream, fileInfoHeader->biClrImportant, sizeof(fileInfoHeader->biClrImportant));
    }

    // bmp v2
    fileInfoHeader->biRedMask = 0;
    fileInfoHeader->biGreenMask = 0;
    fileInfoHeader->biBlueMask = 0;

    if (fileInfoHeader->biSize >= 52) {
        read(fileStream, fileInfoHeader->biRedMask, sizeof(fileInfoHeader->biRedMask));
        read(fileStream, fileInfoHeader->biGreenMask, sizeof(fileInfoHeader->biGreenMask));
        read(fileStream, fileInfoHeader->biBlueMask, sizeof(fileInfoHeader->biBlueMask));
    }

    // если маска не задана, то ставим маску по умолчанию
    if (fileInfoHeader->biRedMask == 0 || fileInfoHeader->biGreenMask == 0 || fileInfoHeader->biBlueMask == 0) {
        fileInfoHeader->biRedMask = static_cast<unsigned int>(maskValue << (bitsOnColor * 2));
        fileInfoHeader->biGreenMask = static_cast<unsigned int>(maskValue << bitsOnColor);
        fileInfoHeader->biBlueMask = static_cast<unsigned int>(maskValue);
    }

    // bmp v3
    if (fileInfoHeader->biSize >= 56) {
        read(fileStream, fileInfoHeader->biAlphaMask, sizeof(fileInfoHeader->biAlphaMask));
    } else {
        fileInfoHeader->biAlphaMask = static_cast<unsigned int>(maskValue << (bitsOnColor * 3));
    }

    // bmp v4
    if (fileInfoHeader->biSize >= 108) {
        read(fileStream, fileInfoHeader->biCSType, sizeof(fileInfoHeader->biCSType));
        read(fileStream, fileInfoHeader->biEndpoints, sizeof(fileInfoHeader->biEndpoints));
        read(fileStream, fileInfoHeader->biGammaRed, sizeof(fileInfoHeader->biGammaRed));
        read(fileStream, fileInfoHeader->biGammaGreen, sizeof(fileInfoHeader->biGammaGreen));
        read(fileStream, fileInfoHeader->biGammaBlue, sizeof(fileInfoHeader->biGammaBlue));
    }

    // bmp v5
    if (fileInfoHeader->biSize >= 124) {
        read(fileStream, fileInfoHeader->biIntent, sizeof(fileInfoHeader->biIntent));
        read(fileStream, fileInfoHeader->biProfileData, sizeof(fileInfoHeader->biProfileData));
        read(fileStream, fileInfoHeader->biProfileSize, sizeof(fileInfoHeader->biProfileSize));
        read(fileStream, fileInfoHeader->biReserved, sizeof(fileInfoHeader->biReserved));
    }

    // проверка на поддерку этой версии формата
    if (fileInfoHeader->biSize != 12 && fileInfoHeader->biSize != 40 && fileInfoHeader->biSize != 52 &&
        fileInfoHeader->biSize != 56 && fileInfoHeader->biSize != 108 && fileInfoHeader->biSize != 124) {
        cout << "Error: Unsupported BMP format." << endl;
        return nullptr;
    }

    if (fileInfoHeader->biBitCount != 16 && fileInfoHeader->biBitCount != 24 && fileInfoHeader->biBitCount != 32) {
        cout << "Error: Unsupported BMP bit count." << endl;
        return nullptr;
    }

    if (fileInfoHeader->biCompression != 0 && fileInfoHeader->biCompression != 3) {
        cout << "Error: Unsupported BMP compression." << endl;
        return nullptr;
    }

    // rgb info
    auto rgbInfo = new RGBQUAD *[fileInfoHeader->biHeight];

    for (unsigned int i = 0; i < fileInfoHeader->biHeight; i++) {
        rgbInfo[i] = new RGBQUAD[fileInfoHeader->biWidth];
    }

    // определение размера отступа в конце каждой строки
    int linePadding = ((fileInfoHeader->biWidth * (fileInfoHeader->biBitCount / 8)) % 4) & 3;

    // чтение
    unsigned int bufer;
    for (unsigned int i = 0; i < fileInfoHeader->biHeight; i++) {
        for (unsigned int j = 0; j < fileInfoHeader->biWidth; j++) {
            read(fileStream, bufer, static_cast<size_t>(fileInfoHeader->biBitCount / 8));

            rgbInfo[i][j].rgbRed = bitextract(bufer, fileInfoHeader->biRedMask);
            rgbInfo[i][j].rgbGreen = bitextract(bufer, fileInfoHeader->biGreenMask);
            rgbInfo[i][j].rgbBlue = bitextract(bufer, fileInfoHeader->biBlueMask);
            rgbInfo[i][j].rgbReserved = bitextract(bufer, fileInfoHeader->biAlphaMask);
        }
        fileStream.seekg(linePadding-2, std::ios_base::cur);
    }

    cout << "mx = " << fileInfoHeader->biHeight << " my = " << fileInfoHeader->biWidth << " size "
         << fileInfoHeader->biSize << endl;
    cout<< "padding before = "<<linePadding<<endl;

    return rgbInfo;
}

    // WRITE
int write_file (BITMAPFILEHEADER *fileHeader,
               BITMAPINFOHEADER *fileInfoHeader,
               RGBQUAD ** rgbInfo,
               const char fileName[]){
    ofstream fileOut;
    fileOut.open(fileName, ofstream::binary);

    if (!fileOut) {
        cout << "Error opening file '" << fileName << "'." << endl;
        return 0;
    }
    // заголовк изображения
    //BITMAPFILEHEADER fileHeader;
    write(fileOut, fileHeader->bfType, sizeof(fileHeader->bfType));
    write(fileOut, fileHeader->bfSize, sizeof(fileHeader->bfSize));
    write(fileOut, fileHeader->bfReserved1, sizeof(fileHeader->bfReserved1));
    write(fileOut, fileHeader->bfReserved2, sizeof(fileHeader->bfReserved2));
    write(fileOut, fileHeader->bfOffBits, sizeof(fileHeader->bfOffBits));

    // информация изображения
    //BITMAPINFOHEADER fileInfoHeader;
    write(fileOut, fileInfoHeader->biSize, sizeof(fileInfoHeader->biSize));

    // bmp core
    if (fileInfoHeader->biSize >= 12) {
        write(fileOut, fileInfoHeader->biWidth, sizeof(fileInfoHeader->biWidth));
        write(fileOut, fileInfoHeader->biHeight, sizeof(fileInfoHeader->biHeight));
        write(fileOut, fileInfoHeader->biPlanes, sizeof(fileInfoHeader->biPlanes));
        write(fileOut, fileInfoHeader->biBitCount, sizeof(fileInfoHeader->biBitCount));
    }
    // bmp v1
    write(fileOut, fileInfoHeader->biCompression, sizeof(fileInfoHeader->biCompression));
    write(fileOut, fileInfoHeader->biSizeImage, sizeof(fileInfoHeader->biSizeImage));
    write(fileOut, fileInfoHeader->biXPelsPerMeter, sizeof(fileInfoHeader->biXPelsPerMeter));
    write(fileOut, fileInfoHeader->biYPelsPerMeter, sizeof(fileInfoHeader->biYPelsPerMeter));
    write(fileOut, fileInfoHeader->biClrUsed, sizeof(fileInfoHeader->biClrUsed));
    write(fileOut, fileInfoHeader->biClrImportant, sizeof(fileInfoHeader->biClrImportant));
    // bmp v2
    write(fileOut, fileInfoHeader->biRedMask, sizeof(fileInfoHeader->biRedMask));
    write(fileOut, fileInfoHeader->biGreenMask, sizeof(fileInfoHeader->biGreenMask));
    write(fileOut, fileInfoHeader->biBlueMask, sizeof(fileInfoHeader->biBlueMask));
    // bmp v3
    write(fileOut, fileInfoHeader->biAlphaMask, sizeof(fileInfoHeader->biAlphaMask));
    // bmp v4
    write(fileOut, fileInfoHeader->biCSType, sizeof(fileInfoHeader->biCSType));
    write(fileOut, fileInfoHeader->biEndpoints, sizeof(fileInfoHeader->biEndpoints));
    write(fileOut, fileInfoHeader->biGammaRed, sizeof(fileInfoHeader->biGammaRed));
    write(fileOut, fileInfoHeader->biGammaGreen, sizeof(fileInfoHeader->biGammaGreen));
    write(fileOut, fileInfoHeader->biGammaBlue, sizeof(fileInfoHeader->biGammaBlue));
    // bmp v5
    write(fileOut, fileInfoHeader->biIntent, sizeof(fileInfoHeader->biIntent));
    write(fileOut, fileInfoHeader->biProfileData, sizeof(fileInfoHeader->biProfileData));
    write(fileOut, fileInfoHeader->biProfileSize, sizeof(fileInfoHeader->biProfileSize));
    write(fileOut, fileInfoHeader->biReserved, sizeof(fileInfoHeader->biReserved));
    // colors
    int linePaddingOut = ((fileInfoHeader->biWidth * (fileInfoHeader->biBitCount / 8)) % 4) & 3;
    unsigned int bufer_out;
    //for (unsigned int i = fileInfoHeader->biHeight-1; i > 0 ; i--) {
    for (unsigned int i = 0; i < fileInfoHeader->biHeight ; i++) {
        for (unsigned int j = 0; j < fileInfoHeader->biWidth; j++) {
        //for (unsigned int j = fileInfoHeader->biWidth; j > 0; j--) {
            bufer_out = bitextrevers(rgbInfo[i][j].rgbRed,
                                     rgbInfo[i][j].rgbGreen,
                                     rgbInfo[i][j].rgbBlue,
                                     rgbInfo[i][j].rgbReserved,
                                     fileInfoHeader->biRedMask,
                                     fileInfoHeader->biGreenMask,
                                     fileInfoHeader->biBlueMask,
                                     fileInfoHeader->biAlphaMask);


            write(fileOut, bufer_out, static_cast<size_t>(fileInfoHeader->biBitCount / 8));

        }
        fileOut.seekp(linePaddingOut-2,std::ios_base::cur);
    }
    cout<< "padding_out = "<<linePaddingOut<<endl;
    //END
    return 1;

}




unsigned char bitextract(const unsigned int byte, const unsigned int mask) {
    if (mask == 0) {
        return 0;
    }

    // определение количества нулевых бит справа от маски
    int maskBufer,
                maskPadding = 0;
    maskBufer = mask;

    while (!(maskBufer & 1)) {
        maskBufer >>= 1;
        maskPadding++;
    }

    // применение маски и смещение
    return static_cast<unsigned char>((byte & mask) >> maskPadding);
}

unsigned int bitextrevers(const unsigned char byte_r,
                          const unsigned char byte_g,
                          const unsigned char byte_b,
                          const unsigned char byte_alpha,
                          const unsigned int mask_r,
                          const unsigned int mask_g,
                          const unsigned int mask_b,
                          const unsigned int mask_alpha) {
    if (mask_r == 0 || mask_g == 0 || mask_b == 0) {
        return 0;
    }
    unsigned int result=0, tmp=0;
    int maskBufer,
            maskPadding=0;
    //r
    maskPadding=0;
    maskBufer = mask_r;
    while (!(maskBufer & 1)) {
        maskBufer >>= 1;
        maskPadding++;
    }

    tmp = byte_r << maskPadding;
    result=result | tmp;

    //g
    maskPadding=0;
    maskBufer = mask_g;
    while (!(maskBufer & 1)) {
        maskBufer >>= 1;
        maskPadding++;
    }

    tmp = byte_g << maskPadding;
    result=result | tmp;

    //b

    maskPadding=0;
    maskBufer = mask_b;
    while (!(maskBufer & 1)) {
        maskBufer >>= 1;
        maskPadding++;
    }

    tmp = byte_b << maskPadding;
    result=result | tmp;
    // reserved
    if(mask_alpha == 0|| byte_alpha == 0){
        return result;
    }
    maskPadding=0;
    maskBufer = mask_alpha;
    while (!(maskBufer & 1)) {
        maskBufer >>= 1;
        maskPadding++;
    }

    tmp = byte_alpha << maskPadding;
    result=result | tmp;
    return result;
}
