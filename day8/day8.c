#include <stdlib.h>
#include <stdio.h>
#include "day8.h"

#define PIX_BLACK 0
#define PIX_WHITE 1
#define PIX_TRANS 2

Layer *makeLayer(int width, int height) {
    int **rows = malloc(sizeof(int *) * height);
    for (int i = 0; i < height; i++) {
        rows[i] = malloc(sizeof(int) * width);
    }
    Layer *layer = malloc(sizeof(Layer));
    layer->width = width;
    layer->height = height;
    layer->pixels = rows;
    layer->next = NULL;
    return layer;
}

Layer *parseImage(const char *inputPath, int width, int height) {
    FILE *fp = fopen(inputPath, "r");
    char c;
    int parsedInt;
    int x, y;
    x = y = 0;
    Layer *firstLayer = makeLayer(width, height);
    Layer *currentLayer = firstLayer;
    while((c = fgetc(fp)) != EOF) {
        parsedInt = c - '0';
        currentLayer->pixels[x][y] = parsedInt;
        y++;

        if (y >= width) {
            y = 0;
            x++;
        }
        if (x >= height) {
            currentLayer->next = makeLayer(width, height);
            currentLayer = currentLayer->next;
            x = 0;
            y = 0;
        }
    }
    fclose(fp);
    return firstLayer;
}

int countDigits(Layer *layer, int digit) {
    int digitCount = 0;
    for(int i = 0; i < layer->height; i++) {
        for(int j = 0; j < layer->width; j++) {
            if (layer->pixels[i][j] == digit) {
                digitCount++;
            }
        }
    }
    return digitCount;
}

Layer *layerWithLeastZeros(Layer *layer) {
    Layer *leastZeros = layer;
    int minZeros = countDigits(layer, 0);

    while (layer != NULL) {
        int zeroCount = countDigits(layer, 0);
        if (zeroCount < minZeros) {
            minZeros = zeroCount;
            leastZeros = layer;
        }
        layer = layer->next;
    }
    return leastZeros;
}

Layer *decode(Layer *layer) {
    Layer *decoded = makeLayer(layer->width, layer->height);
    for(int i = 0; i < decoded->height; i++) {
        for(int j = 0; j < decoded->width; j++) {
            decoded->pixels[i][j] = PIX_TRANS;
        }
    }

    while(layer != NULL) {
        for(int i = 0; i < decoded->height; i++) {
            for(int j = 0; j < decoded->width; j++) {
                if (decoded->pixels[i][j] == PIX_TRANS) {
                    decoded->pixels[i][j] = layer->pixels[i][j];
                }
            }
        }
        layer = layer->next;
    }

    return decoded;

}

void printLayer(Layer *layer) {
    for(int i = 0; i < layer->height; i++) {
        for(int j = 0; j < layer->width; j++)  {
            printf("%d", layer->pixels[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s {width} {height} {input-file}", argv[0]);
        exit(1);
    }
    int width = strtol(argv[1], NULL, 10);
    int height = strtol(argv[2], NULL, 10);
    Layer *layers = parseImage(argv[3], width, height);

    Layer *leastZerosLayer = layerWithLeastZeros(layers);

    int checksum = countDigits(leastZerosLayer, 1) * countDigits(leastZerosLayer, 2);
    printf("Checksum: %d\n", checksum);

    Layer *decoded = decode(layers);
    printLayer(decoded);

} 
