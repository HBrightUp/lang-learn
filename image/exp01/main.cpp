#include<iostream>
#include<fstream>

// seven color of rainbow
uint rainbowColors[] = {
        0XFF0000, // red
        0XFFA500, // orange
        0XFFFF00, // yellow
        0X00FF00, // green
        0X007FFF, // cyan-blue
        0X0000FF, // blue
        0X8B00FF  // Purple
};

void writeRainbow(std::string rgb_file_path , int width, int height) {


    std::ofstream outfile;
    outfile.open(rgb_file_path, std::ios::app);
    if (!outfile.is_open()) {
        std::cout << " open rgb file failed!" << std::endl;
        return ;
    }

    for (int i = 0; i < width; ++i) {
        
        uint currentColor = 0X000000;
        if(i < 100) {
            //  red
            currentColor = rainbowColors[0];
        } else if(i < 200) {
            // 100-200  orange
            currentColor = rainbowColors[1];
        } else if(i < 300) {
            // 200-300  yellow
            currentColor = rainbowColors[2];
        } else if(i < 400) {
            // 300-400  green
            currentColor = rainbowColors[3];
        } else if(i < 500) {
            // 400-500  青色
            currentColor = rainbowColors[4];
        } else if(i < 600) {
            // 500-600  blue
            currentColor = rainbowColors[5];
        } else if(i < 700) {
            // 600-700  purpple
            currentColor = rainbowColors[6];
        }

        unsigned char R = (currentColor & 0xFF0000) >> 16;
        unsigned char G = (currentColor & 0x00FF00) >> 8;
        unsigned char B = currentColor & 0x0000FF;

        for (int j = 0; j < height; ++j) {
            //fputc(R, rgbFile);
            //fputc(G, rgbFile);
            //fputc(B, rgbFile);
            outfile << R << G << B ;
        }
    }

    outfile.close();
}

/*bmp file header,  12 bytes*/
typedef struct {
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} BitmapFileHeader;

/*bmp info header, 40 bytes */
typedef struct {
    unsigned int   biSize; /* Size of info header */
    int            biWidth; /* Width of image */
    int            biHeight; /* Height of image */
    unsigned short biPlanes; /* Number of color planes */
    unsigned short biBitCount; /* Number of bits per pixel */
    unsigned int   biCompression; /* Type of compression to use */
    unsigned int   biSizeImage; /* Size of image data */
    int            biXPelsPerMeter; /* X pixels per meter */
    int            biYPelsPerMeter; /* Y pixels per meter */
    unsigned int   biClrUsed; /* Number of colors used */
    unsigned int   biClrImportant; /* Number of important colors */
} BitmapInfoHeader;

void writeRGBToBmp( std::string bmp_file_path, int width, int height) {


     std::ofstream outfile;
    try {
        
         outfile.open(bmp_file_path, std::ios::app);
        if (!outfile.is_open()) {
            std::cout << "Open bmp file failed!" << std::endl;
        
        }
    } catch(std::exception &e) {
        std::cout << e.what() << std::endl;
        return ;
    }
   
   


    unsigned short bfType = 0x4d42; //  BM

    BitmapFileHeader fileHeader;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfSize = 2 + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + width*height*3;
    fileHeader.bfOffBits = 0x36;

    BitmapInfoHeader infoHeader;
    infoHeader.biSize = sizeof(BitmapInfoHeader);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biSizeImage = 0;
    infoHeader.biCompression = 0;
    infoHeader.biXPelsPerMeter = 5000;
    infoHeader.biYPelsPerMeter = 5000;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    outfile << bfType ;
    outfile.write(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader)); 
    outfile.write(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeader)); 

    // write data of  image
    for (int i = 0; i < width; ++i) {

        u_int32_t currentColor = rainbowColors[0];
        if(i < 100) {
            currentColor = rainbowColors[0];
        } else if(i < 200) {
            currentColor = rainbowColors[1];
        } else if(i < 300) {
            currentColor = rainbowColors[2];
        } else if(i < 400) {
            currentColor = rainbowColors[3];
        } else if(i < 500) {
            currentColor = rainbowColors[4];
        } else if(i < 600) {
            currentColor = rainbowColors[5];
        } else if(i < 700) {
            currentColor = rainbowColors[6];
        }
    
        u_int8_t R = (currentColor & 0xFF0000) >> 16;
        u_int8_t G = (currentColor & 0x00FF00) >> 8;
        u_int8_t B = currentColor & 0x0000FF;

        for (int j = 0; j < height; ++j) {
            outfile << R << G << B;
        }
    }

    outfile.close();
}

// 计算每一行像素 4 字节对齐后的字节数
int caculateLineBytes(int width) {
    //******* 四字节对齐 *******
    return (24 * width + 31)/32 *4;
    //******* 四字节对齐 *******
}

void writeRGBToBmpWithalign(std::string rgb_file_path, int width, int height) {
    std::ofstream outfile;
    outfile.open(rgb_file_path, std::ios::app);
    if (!outfile.is_open()) {
        std::cout << " open rgb file failed!" << std::endl;
        return ;
    }

    unsigned short bfType = 0x4d42;

    int lineBytes = caculateLineBytes(width);

    BitmapFileHeader fileHeader;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfSize = 2 + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + lineBytes*height;
    fileHeader.bfOffBits = 0x36;

    BitmapInfoHeader infoHeader;
    infoHeader.biSize = sizeof(BitmapInfoHeader);
    infoHeader.biWidth = width;
    infoHeader.biHeight = -height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biSizeImage = 0;
    infoHeader.biCompression = 0;
    infoHeader.biXPelsPerMeter = 5000;
    infoHeader.biYPelsPerMeter = 5000;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    //fwrite(&bfType, sizeof(bfType), 1, bitmapFile);
    //fwrite(&fileHeader, sizeof(fileHeader), 1, bitmapFile);
    //fwrite(&infoHeader, sizeof(infoHeader), 1, bitmapFile);
    outfile << bfType ;
    outfile.write(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader)); 
    outfile.write(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeader)); 


    for (int i = 0; i < width; ++i) {

        u_int32_t currentColor = rainbowColors[0];
        if(i < 100) {
            currentColor = rainbowColors[0];
        } else if(i < 200) {
            currentColor = rainbowColors[1];
        } else if(i < 300) {
            currentColor = rainbowColors[2];
        } else if(i < 400) {
            currentColor = rainbowColors[3];
        } else if(i < 500) {
            currentColor = rainbowColors[4];
        } else if(i < 600) {
            currentColor = rainbowColors[5];
        } else if(i < 700) {
            currentColor = rainbowColors[6];
        }
    
        unsigned char R = (currentColor & 0xFF0000) >> 16;
        unsigned char G = (currentColor & 0x00FF00) >> 8;
        unsigned char B = currentColor & 0x0000FF;

        unsigned char lineBytesArray[lineBytes];

        for (int j = 0; j < height; ++j) {
            int currentIndex = 3*j;
            lineBytesArray[currentIndex] = B;
            lineBytesArray[currentIndex+1] = G;
            lineBytesArray[currentIndex+2] = R;
        }

        //fwrite(lineBytesArray, sizeof(lineBytesArray), 1, bitmapFile);
        outfile.write(reinterpret_cast<char*>(&lineBytesArray), sizeof(lineBytesArray)); 
    }
    outfile.close();
}

int main() {

    writeRainbow("./rainbow.rgb24", 700, 700);
    writeRGBToBmp("./rainbow.bmp", 700, 700);
    writeRGBToBmpWithalign("./rainbow-711x711-fix.bmp", 703, 703);
    return 0;
}