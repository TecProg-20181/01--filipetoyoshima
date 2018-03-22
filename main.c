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


int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.red == p2.red &&
        p1.green == p2.green &&
        p1.blue == p2.blue)
        return 1;
    return 0;
}

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
            print("%u", img.pixel[i][j].red + img.pixel[i][j].green + img.pixel[i][j].blue);
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

void blur(unsigned int h, Pixel pixel[512][512], int T, unsigned int w) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (h - 1 > i + T/2) ? i + T/2 : h - 1;
            int min_w = (w - 1 > j + T/2) ? j + T/2 : w - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.red += pixel[x][y].red;
                    media.green += pixel[x][y].green;
                    media.blue += pixel[x][y].blue;
                }
            }

            // printf("%u", media.r)
            media.red /= T * T;
            media.green /= T * T;
            media.blue /= T * T;

            pixel[i][j].red = media.red;
            pixel[i][j].green = media.green;
            pixel[i][j].blue = media.blue;
        }
    }
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

void inverter_cores(Pixel pixel[512][512],
                    unsigned int w, unsigned int h) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            pixel[i][j].red = 255 - pixel[i][j].red;
            pixel[i][j].green = 255 - pixel[i][j].green;
            pixel[i][j].blue = 255 - pixel[i][j].blue;
        }
    }
}

Image cortar_imagem(Image img, int x, int y, int w, int h) {
    Image cortada;
    cortada.max_color = img.max_color;
    cortada.width = w;
    cortada.height = h;
    strcpy(cortada.type, img.type);

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cortada.pixel[i][j].red = img.pixel[i + y][j + x].red;
            cortada.pixel[i][j].green = img.pixel[i + y][j + x].green;
            cortada.pixel[i][j].blue = img.pixel[i + y][j + x].blue;
        }
    }

    return cortada;
}


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
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.height, img.pixel, tamanho, img.width);
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
                inverter_cores(img.pixel, img.width, img.height);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int w, h;
                scanf("%d %d", &w, &h);

                img = cortar_imagem(img, x, y, w, h);
                break;
            }
        }

    }

    print_image(img);
    return 0;
}
