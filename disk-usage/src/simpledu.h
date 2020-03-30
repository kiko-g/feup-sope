#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#define MAX_FLAG_LEN 20

#define ALL_FLAG_SHORT          "-a"
#define ALL_FLAG_LONG           "--all"
#define BYTES_FLAG_SHORT        "-b"
#define BYTES_FLAG_LONG         "--bytes"
#define BLOCKSIZE_FLAG_SHORT    "-B="
#define BLOCKSIZE_FLAG_LONG     "--block-size="
#define COUNTLINKS_FLAG_SHORT   "-l"
#define COUNTLINKS_FLAG_LONG    "--count-links"
#define LINK_FLAG_SHORT         "-L"
#define LINK_FLAG_LONG          "--dereference"
#define SEPARATEDIRS_FLAG_SHORT "-S"
#define SEPARATEDIRS_LONG       "--separate-dirs"
#define MAX_DEPTH_FLAG          "--max-depth="


