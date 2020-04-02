#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#define MAX_FLAG_LEN 50
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
#define RED_TEXT                "\033[0;31m"
#define BLUE_TEXT               "\033[0;34m"
#define GREEN_TEXT              "\033[0;32m"
#define RESET_TEXT_COLOR        "\033[0m"



/**
 * @brief checks if a path was provided through command line 
 * and sets the working directory to the correct path
 * @param flags parsed flags array
 * @param len length of the flags array
 * @return positive index if successfully read a path
 */
int pathProvided(char flags[][MAX_FLAG_LEN], size_t len);

void details(char dirname[]);

bool isFile(const char *path);

bool isDirectory(const char *path);

bool isSymbolLink(const char *path);