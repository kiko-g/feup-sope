#include "simpledu.h"

int main(int argc, char *argv[])
{
    char *dirname = ".";
    char flags[argc][MAX_FLAG_LEN];
    DIR *dir = opendir(dirname);
    struct dirent *ent;
    struct stat buf;

    // parse flags
    for (int i = 1; i < argc; ++i)
        strcpy(flags[i - 1], argv[i]);

    printf("FLAGS\n");
    for (int i = 0; i < argc - 1; ++i)
        printf("%s%s%s\n", BLUE_TEXT, flags[i], RESET_TEXT_COLOR);

    int index = pathProvided(flags, (size_t)argc - 1);
    if (index != -1)
        dir = opendir(flags[index]);

    while ((ent = readdir(dir)) != NULL)
    {
        // skip showing current folder and parent folder
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        stat(ent->d_name, &buf);

        // apply different behaviors for files and directories
        if (S_ISREG(buf.st_mode))
            printf("File: %-30s %ld Bytes\n", ent->d_name, buf.st_size);
        else
        {
            printf("Dir:  %-30s (can go deeper)\n", ent->d_name);
            // details(ent->d_name);
        }
    }

    return 0;
}

int pathProvided(char flags[][MAX_FLAG_LEN], size_t len)
{
    printf("\n");
    short index = -1;
    for (size_t i = 0; i < len; ++i)
        for (size_t j = 0; j < strlen(flags[i]); ++j)
            if (flags[i][j] == '/' || flags[i][j] == '~')
                index = i;

    if (index == -1)
    {
        // path not provided
        printf("%sAnalysing current working directory.%s\n", GREEN_TEXT, RESET_TEXT_COLOR);
        return -1;
    }

    if (opendir(flags[index]) == NULL)
    {
        // path provided but invalid
        printf("%sNo valid path to directory provided %s('%s').\n", RED_TEXT, RESET_TEXT_COLOR, flags[index]);
        printf("%sAnalysing current working directory.%s\n", GREEN_TEXT, RESET_TEXT_COLOR);
        return -1;
    }

    else
    {
        // path provided and valid
        printf("%sAnalysing '%s'%s\n", GREEN_TEXT, flags[index], RESET_TEXT_COLOR);
        return index;
    }
}

void details(char dirname[])
{
    DIR *dir = opendir(dirname);
    struct dirent *ent;
    struct stat buf;

    while ((ent = readdir(dir)) != NULL)
    {
        // skip showing current folder and parent folder
        if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
            continue;

        stat(dirname, &buf);

        // apply different behaviors for files and directories
        if (isFile(dirname))
            printf("File: %-30s %ld Bytes\n", dirname, buf.st_size);
        else if (isDirectory(dirname))
        {
            printf("Directory: %-30s (can go deeper)\n", dirname);
            details(dirname);
        }
    }
}

bool isFile(const char *path)
{
    struct stat buf;
    stat(path, &buf);
    if (S_ISREG(buf.st_mode)) // File
        return true;
    return false;
}

bool isDirectory(const char *path)
{
    struct stat buf;
    stat(path, &buf);
    if (S_ISDIR(buf.st_mode)) // Directory
        return true;
    return false;
}

bool isSymbolicLink(const char *path)
{
    struct stat buf;
    stat(path, &buf);
    if (S_ISLNK(buf.st_mode)) // Directory
        return true;
    return false;
}
