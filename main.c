#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    Pixel pixel[512][512];
    unsigned int w;
    unsigned int h;
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


Image escala_de_cinza(Image img) {
    /*for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            print("%u", img.pixel[i][j].red + img.pixel[i][j].green + img.pixel[i][j].blue);
        }
    }*/

    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
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

    rotacionada.w = img.h;
    rotacionada.h = img.w;

    for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
        for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j].red = img.pixel[x][y].red;
            rotacionada.pixel[i][j].green = img.pixel[x][y].green;
            rotacionada.pixel[i][j].blue = img.pixel[x][y].blue;
        }
    }

    return rotacionada;
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

    cortada.w = w;
    cortada.h = h;

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

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.w, &img.h, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].red,
                                 &img.pixel[i][j].green,
                                 &img.pixel[i][j].blue);

        }
    }

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
                for (unsigned int x = 0; x < img.h; ++x) {
                    for (unsigned int j = 0; j < img.w; ++j) {
                        Pixel pixel;
                        pixel.red = img.pixel[x][j].red;
                        pixel.green = img.pixel[x][j].green;
                        pixel.blue = img.pixel[x][j].blue;

                        int p =  pixel.red * .393 + pixel.green * .769 + pixel.blue * .189;
                        int menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j].red = menor_r;

                        p =  pixel.red * .349 + pixel.green * .686 + pixel.blue * .168;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j].green = menor_r;

                        p =  pixel.red * .272 + pixel.green * .534 + pixel.blue * .131;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j].blue = menor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.h, img.pixel, tamanho, img.w);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotacionar90direita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int w = img.w, h = img.h;

                if (horizontal == 1) w /= 2;
                else h /= 2;

                for (int i2 = 0; i2 < h; ++i2) {
                    for (int j = 0; j < w; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.w - 1 - j;
                        else x = img.h - 1 - i2;

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
                break;
            }
            case 6: { // Inversao de Cores
                inverter_cores(img.pixel, img.w, img.h);
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

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.w, img.h);

    // print pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].red,
                                   img.pixel[i][j].green,
                                   img.pixel[i][j].blue);

        }
        printf("\n");
    }
    return 0;
}
