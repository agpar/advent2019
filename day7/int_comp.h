#define SIG_EXIT 0
#define SIG_IO 1
#define STATE_READ 0
#define STATE_WRITE 1

typedef struct Program {
    int *tape;
    int pc;
    int size;
} Program;


typedef struct IO {
    int state;
    int value;
} IO;

Program *loadInput(const char* inputPath);
int runProgram(Program *program, IO *in, IO *out);

