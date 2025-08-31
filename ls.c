#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *entry;
	char *path;
	if (argc > 1)
	{
		path = argv[1];
	} else {
		path = ".";
	}
    dir = opendir(path);
    if (dir == NULL)
	{
        perror("opendir");
        return (1);
    }
    while ((entry = readdir(dir)) != NULL)
	{
        printf("%s ", entry->d_name);
    }
    closedir(dir);
    return 0;
}