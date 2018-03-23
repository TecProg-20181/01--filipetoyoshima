#include <stdio.h>


typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;


typedef struct _image {
    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
    char type[4];
    int max_color;
} Image;


/* //Non used function
int max(int a, int b) {
    if (a > b)
        return a;
    return b;
} */
/* //Non used function
int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.red == p2.red &&
        p1.green == p2.green &&
        p1.blue == p2.blue)
        return 1;
    return 0;
} */


void strcpy(char * s1, char * s2);
Image read_image();
void print_image(Image img);
//Effects:
Image grey_scale(Image img);
Image sepia_filter(Image img);
Image blur(Image img);
Image rotate_90_clockwise(Image img);
Image mirroring(Image img);
Image negative_color(Image img);
Image cut_image(Image img);


int main() {
     Image img;

     img = read_image();

     int qOptions;
     scanf("%d", &qOptions);

     for(int i = 0; i < qOptions; ++i) {
          int option;
          scanf("%d", &option);

          switch(option) {
               case 1: { // Escala de Cinza
                    img = grey_scale(img);
                    break;
               }
               case 2: { // Filtro Sepia
                    img = sepia_filter(img);
                    break;
               }
               case 3: { // Blur
                    img = blur(img);
                    break;
               }
               case 4: { // Rotacao
                    img = rotate_90_clockwise(img);
                    break;
               }
               case 5: { // Espelhamento
                    img = mirroring(img);
                    break;
               }
               case 6: { // Inversao de Cores
                    img = negative_color(img);
                    break;
               }
               case 7: { // Cortar Imagem
                    img = cut_image(img);
                    break;
               }
          }

     }

     print_image(img);
     return 0;
}


// I'm not gonna add a library just to use ONE method
void strcpy(char * s1, char * s2) {
     int i = 0;
     while (s2[i] != '\0') {
          s1[i] = s2[i];
          i++;
     }
}


Image read_image() {
    Image img;
    // FILE *debugLog = fopen("debug.log", "w");
    scanf("%s", img.type);
    scanf("%u %u", &img.width, &img.height);
    scanf("%d", &img.max_color);
    /*
    // testLog:
    fprintf (debugLog, "%s\n", img.type);
    fprintf (debugLog, "%u %u\n", img.width, img.height);
    fprintf (debugLog, "%d\n", img.max_color);
    */
    // read all pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].red,
                                 &img.pixel[i][j].green,
                                 &img.pixel[i][j].blue);

        }
    }
    return img;
}


void print_image(Image img) {

    printf("%s\n", img.type);
    printf("%u %u\n", img.width, img.height);
    printf("%d\n", img.max_color);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].red,
                                   img.pixel[i][j].green,
                                   img.pixel[i][j].blue);

        }
        printf("\n");
    }

}


Image grey_scale(Image img) {
    /*for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            print("%u", img.pixel[i][j].red +
                        img.pixel[i][j].green +
                        img.pixel[i][j].blue);
        }
    }*/

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel[i][j].red +
                        img.pixel[i][j].green +
                        img.pixel[i][j].blue;
            media /= 3;
            img.pixel[i][j].red = media;
            img.pixel[i][j].green = media;
            img.pixel[i][j].blue = media;
        }
    }
    return img;
}


Image sepia_filter(Image img) {

     for (unsigned int i = 0; i < img.height; ++i) {
         for (unsigned int j = 0; j < img.width; ++j) {
            Pixel pixel;
            pixel.red = img.pixel[i][j].red;
            pixel.green = img.pixel[i][j].green;
            pixel.blue = img.pixel[i][j].blue;

            int p =  pixel.red * .393 + pixel.green * .769 + pixel.blue * .189;
            int menor_r = (255 >  p) ? p : 255;
            img.pixel[i][j].red = menor_r;

            p =  pixel.red * .349 + pixel.green * .686 + pixel.blue * .168;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[i][j].green = menor_r;

            p =  pixel.red * .272 + pixel.green * .534 + pixel.blue * .131;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[i][j].blue = menor_r;
         }
     }
     return img;
}


Image blur(Image img) {
     Image blurred;
     int blurScale;

     scanf("%d", &blurScale);
     blurred.width = img.width;
     blurred.height = img.height;
     blurred.max_color = img.max_color;
     strcpy(blurred.type, img.type);

     for (short i = 0; i < img.height; i++) {
          for (short j = 0; j < img.width; j++) {
               int qPixels = 0;
               Pixel media = {0, 0, 0};

               // Quadrant limits
               int originX = i - blurScale/2;
               if (originX < 0) {
                    originX = 0;
               }
               int originY = j - blurScale/2;
               if (originY < 0) {
                    originY = 0;
               }
               int limitX = i + blurScale/2;
               if (limitX > img.width) {
                    limitX = img.width - 1;
               }
               int limitY = j + blurScale/2;
               if (limitY > img.height) {
                    limitY = img.height - 1;
               }

               // Calculating the media of the quadrant
               for (; originX <= limitX; originX++) {
                    for (; originY <= limitY; originY ++) {
                         media.red += img.pixel[originX][originY].red;
                         media.green += img.pixel[originX][originY].green;
                         media.blue += img.pixel[originX][originY].blue;
                         qPixels++;
                    }
               }

               blurred.pixel[i][j].red = media.red / qPixels;
               blurred.pixel[i][j].green = media.green / qPixels;
               blurred.pixel[i][j].blue = media.blue / qPixels;

          }
     }
     return blurred;
}


Image rotate_90_clockwise(Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;
    rotated.max_color = img.max_color;
    strcpy(rotated.type, img.type);

    int quantas_vezes = 0;
    scanf("%d", &quantas_vezes);
    quantas_vezes %= 4;
    for (int j = 0; j < quantas_vezes; ++j) {

         for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
              for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
                   rotated.pixel[i][j].red = img.pixel[x][y].red;
                   rotated.pixel[i][j].green = img.pixel[x][y].green;
                   rotated.pixel[i][j].blue = img.pixel[x][y].blue;
              }
         }

    }
    return rotated;
}


Image mirroring(Image img) {
     int horizontal = 0;
     scanf("%d", &horizontal);

     int horizontalLimit = img.width;
     int verticalLimit = img.height;
     if (horizontal == 1) horizontalLimit /= 2;
     else verticalLimit /= 2;

     for (int i = 0; i < verticalLimit; ++i) {
         for (int j = 0; j < horizontalLimit; ++j) {
            int x = i, y = j;

            if (horizontal == 1) {
                  y = img.width - 1 - j;
            } else {
                  x = img.height - 1 - i;
            }
            Pixel aux;
            aux.red = img.pixel[i][y].red;
            aux.green = img.pixel[i][y].green;
            aux.blue = img.pixel[i][y].blue;

            img.pixel[i][y].red = img.pixel[x][y].red;
            img.pixel[i][y].green = img.pixel[x][y].green;
            img.pixel[i][y].blue = img.pixel[x][y].blue;

            img.pixel[x][y].red = aux.red;
            img.pixel[x][y].green = aux.green;
            img.pixel[x][y].blue = aux.blue;
         }
     }
     return img;
}


Image negative_color(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j].red = 255 - img.pixel[i][j].red;
            img.pixel[i][j].green = 255 - img.pixel[i][j].green;
            img.pixel[i][j].blue = 255 - img.pixel[i][j].blue;
        }
    }
    return img;
}


Image cut_image(Image img) {
    int originX, originY;
    int newWidth, newHeight;
    Image cuted;

    scanf("%d %d", &originX, &originY);
    scanf("%d %d", &newWidth, &newHeight);

    cuted.max_color = img.max_color;
    cuted.width = newWidth;
    cuted.height = newHeight;
    strcpy(cuted.type, img.type);

    for(int i = 0; i < newHeight; ++i) {
        for(int j = 0; j < newWidth; ++j) {
            cuted.pixel[i][j].red = img.pixel[i + originY][j + originX].red;
            cuted.pixel[i][j].green = img.pixel[i + originY][j + originX].green;
            cuted.pixel[i][j].blue = img.pixel[i + originY][j + originX].blue;
        }
    }
    return cuted;
}
