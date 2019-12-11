typedef struct Layer {
    int width;
    int height;
    int **pixels;
    struct Layer *next;
} Layer;
    
