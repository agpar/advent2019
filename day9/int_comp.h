#define SIG_EXIT 0
#define SIG_IO 1
#define STATE_READ 0
#define STATE_WRITE 1
#define TAPE_LEN 10000

typedef long long t_data;

typedef struct Program {
    t_data *tape;
    int pc;
    int size;
    int rb;
} Program;


typedef struct IO {
    int state;
    t_data value;
} IO;

// Load an input file into a Program.
Program *loadInput(const char* inputPath);

// Run the program with control over IO. When IO is necessary but the
// given IO struct is not in a correct state, return SIG_IO. Useful for
// piping together multiple programs.
int runProgram(Program *program, IO *in, IO *out);

// Run the program, using stdin and stdout as IO sources.
int runStdProgram(Program *program);
