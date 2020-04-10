#include <limits.h>
#define MAX_FLAG_LEN 256
#define ALL_FLAG_SHORT          "-a"
#define ALL_FLAG_LONG           "--all"
#define BYTES_FLAG_SHORT        "-b"
#define BYTES_FLAG_LONG         "--bytes"
#define BLOCKSIZE_FLAG_SHORT    "-B"
#define BLOCKSIZE_FLAG_LONG     "--block-size"
#define COUNTLINKS_FLAG_SHORT   "-l"
#define COUNTLINKS_FLAG_LONG    "--count-links"
#define LINK_FLAG_SHORT         "-L"
#define LINK_FLAG_LONG          "--dereference"
#define SEPARATEDIRS_FLAG_SHORT "-S"
#define SEPARATEDIRS_LONG       "--separate-dirs"
#define MAX_DEPTH_FLAG          "--max-depth"



#define ARGUMENTS (char*[13]){ALL_FLAG_SHORT, ALL_FLAG_LONG  , BYTES_FLAG_SHORT  , BYTES_FLAG_LONG   , BLOCKSIZE_FLAG_SHORT , BLOCKSIZE_FLAG_LONG , COUNTLINKS_FLAG_SHORT , COUNTLINKS_FLAG_LONG, LINK_FLAG_SHORT , LINK_FLAG_LONG , SEPARATEDIRS_FLAG_SHORT, SEPARATEDIRS_LONG , MAX_DEPTH_FLAG}

struct Arguments {
    bool all;
    bool bytes;
    bool block_size_flag;
    int block_size;
    bool countLinks;
    bool deference;
    bool separateDirs;
    int max_depth;
    bool max_depth_flag;
    char path[MAX_FLAG_LEN];
    bool is_dir;
};
