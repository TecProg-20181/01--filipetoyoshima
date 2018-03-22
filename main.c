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
Image escala_de_cinza(Image img);
Image filtro_sepia(Image img);
Image blur(Image img);
Image rotacionar90direita(Image img);
Image espelhamento(Image img);
Image inverter_cores(Image img);
Image cortar_imagem(Image img);

int main() {
     Image img;

     img = read_image();

     int n_opcoes;
     scanf("%d", &n_opcoes);

     for(int i = 0; i < n_opcoes; ++i) {
          int opcao;
          scanf("%d", &opcao);

          switch(opcao) {
               case 1: { // Escala de Cinza
                    img = escala_de_cinza(img);
                    break;
               }
               case 2: { // Filtro Sepia
                    img = filtro_sepia(img);
                    break;
               }
               case 3: { // Blur
                    img = blur(img);
                    break;
               }
               case 4: { // Rotacao
                    img = rotacionar90direita(img);
                    break;
               }
               case 5: { // Espelhamento
                    img = espelhamento(img);
                    break;
               }
               case 6: { // Inversao de Cores
                    img = inverter_cores(img);
                    break;
               }
               case 7: { // Cortar Imagem
                    img = cortar_imagem(img);
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

Image escala_de_cinza(Image img) {
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

Image filtro_sepia(Image img) {

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
               int originY = j - blurScale/2;
               int limitX = i + blurScale/2;
               int limitY = j + blurScale/2;

               // Calculating the media of the quadrant
               for (; originX <= limitX; originX++) {
                    if (originX > 0 && originX < img.width) {
                         for (; originY <= limitY; originY ++) {
                              if (originY > 0 && originY < img.height) {
                                   media.red += img.pixel[originX][originY].red;
                                   media.green += img.pixel[originX][originY].green;
                                   media.blue += img.pixel[originX][originY].blue;
                                   qPixels++;
                              }
                         }
                    }
               }

               blurred.pixel[i][j].red = media.red / qPixels;
               blurred.pixel[i][j].green = media.green / qPixels;
               blurred.pixel[i][j].blue = media.blue / qPixels;

          }
     }

     return blurred;
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;
    rotacionada.max_color = img.max_color;
    strcpy(rotacionada.type, img.type);

    int quantas_vezes = 0;
    scanf("%d", &quantas_vezes);
    quantas_vezes %= 4;
    for (int j = 0; j < quantas_vezes; ++j) {

         for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
              for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
                   rotacionada.pixel[i][j].red = img.pixel[x][y].red;
                   rotacionada.pixel[i][j].green = img.pixel[x][y].green;
                   rotacionada.pixel[i][j].blue = img.pixel[x][y].blue;
              }
         }

    }


    return rotacionada;
}

Image espelhamento(Image img) {
     int horizontal = 0;
     scanf("%d", &horizontal);

     int w = img.width, h = img.height;

     if (horizontal == 1) w /= 2;
     else h /= 2;

     for (int i2 = 0; i2 < h; ++i2) {
         for (int j = 0; j < w; ++j) {
            int x = i2, y = j;

            if (horizontal == 1) y = img.width - 1 - j;
            else x = img.height - 1 - i2;

            Pixel aux1;
            aux1.red = img.pixel[i2][y].red;
            aux1.green = img.pixel[i2][y].green;
            aux1.blue = img.pixel[i2][y].blue;

            img.pixel[i2][y].red = img.pixel[x][y].red;
            img.pixel[i2][y].green = img.pixel[x][y].green;
            img.pixel[i2][y].blue = img.pixel[x][y].blue;

            img.pixel[x][y].red = aux1.red;
            img.pixel[x][y].green = aux1.green;
            img.pixel[x][y].blue = aux1.blue;
         }
     }
     return img;
}

Image inverter_cores(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            img.pixel[i][j].red = 255 - img.pixel[i][j].red;
            img.pixel[i][j].green = 255 - img.pixel[i][j].green;
            img.pixel[i][j].blue = 255 - img.pixel[i][j].blue;
        }
    }
    return img;
}

Image cortar_imagem(Image img) {
    int originX, originY;
    int newWidth, newHeight;
    Image cortada;

    scanf("%d %d", &originX, &originY);
    scanf("%d %d", &newWidth, &newHeight);

    cortada.max_color = img.max_color;
    cortada.width = newWidth;
    cortada.height = newHeight;
    strcpy(cortada.type, img.type);

    for(int i = 0; i < newHeight; ++i) {
        for(int j = 0; j < newWidth; ++j) {
            cortada.pixel[i][j].red = img.pixel[i + originY][j + originX].red;
            cortada.pixel[i][j].green = img.pixel[i + originY][j + originX].green;
            cortada.pixel[i][j].blue = img.pixel[i + originY][j + originX].blue;
        }
    }

    return cortada;
}
