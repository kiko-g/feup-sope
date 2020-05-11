
#define MAX_LEN 256
#define NUMBER_FLAGS 4
#define NUMBER_SERVER_FLAGS 8
#define T_FLAG "-t"


struct ServerArgs {
    unsigned int nsecs;
    unsigned int nplaces;
    unsigned int nthreads;
    char fifoname[MAX_LEN];
};


