

#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include <sys/stat.h>


int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}
void do_ls(char dirname[]) {
    DIR* dir1=opendir(dirname);       // 记录opendir()后的返回值
    struct dirent* dirent1; // 记录readdir()后的返回值
    char** dir2=NULL;
    int count = 0;
    if (dir1 == NULL) {
        fprintf(stderr, "ls1: cannot open %s\n", dirname);//打开失败
    }
    else {
        while ((dirent1 = readdir(dir1)) != NULL) {//此处不能提前，因为每一次提取都是一个新的目录项
            if(dirent1->d_name[0] =='.') continue;
            dir2 = realloc(dir2, (count + 1) * sizeof(char *));
            dir2[count] = strdup(dirent1->d_name);
            count++;
        }
        closedir(dir1);
        qsort(dir2, count, sizeof(char *), compare);
        for (int i = 0; i < count; i++) {
        printf("%s\n", dir2[i]);
        free(dir2[i]);
        }
        free(dir2);
    }
}
void do_ls_a(char dirname[]) {
    DIR* dir1=opendir(dirname);       // 记录opendir()后的返回值
    struct dirent* dirent1; // 记录readdir()后的返回值
    char** dir2=NULL;
    int count = 0;
    if (dir1 == NULL) {
        fprintf(stderr, "ls1: cannot open %s\n", dirname);//打开失败
    }
    else {
        while ((dirent1 = readdir(dir1)) != NULL) {//此处不能提前，因为每一次提取都是一个新的目录项
            //if(dirent1->d_name[0] =='.') continue;
            dir2 = realloc(dir2, (count + 1) * sizeof(char *));
            dir2[count] = strdup(dirent1->d_name);
            count++;
        }
        closedir(dir1);
        qsort(dir2, count, sizeof(char *), compare);
        for (int i = 0; i < count; i++) {
        printf("%s\n", dir2[i]);
        free(dir2[i]);
        }
        free(dir2);
    }
}
void do_ls_s(char *dirname){DIR *dir = opendir(dirname); // 打开目录
    if (dir == NULL) {
        fprintf(stderr, "ls1: cannot open %s\n", dirname);
        return;
    }

    struct dirent *dirent1;
    char **dir_names = NULL;
    int count = 0;

    // 读取目录下的所有文件和目录名
    while ((dirent1 = readdir(dir)) != NULL) {
        dir_names = realloc(dir_names, (count + 1) * sizeof(char *));
        dir_names[count] = strdup(dirent1->d_name);
        count++;
    }
    closedir(dir);

    // 排序文件和目录名
    qsort(dir_names, count, sizeof(char *), compare);

    // 输出每个文件和目录的大小
    for (int i = 0; i < count; i++) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirname, dir_names[i]);

        struct stat file_stat;
        if (stat(path, &file_stat) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        // 如果是普通文件，则输出大小
        if (S_ISREG(file_stat.st_mode)) {
            printf("%-10ld %s\n", (long)file_stat.st_blocks, dir_names[i]);
        }

        free(dir_names[i]);
    }
    free(dir_names);
}
int main(int argc, char* argv[]) {
    int n=argc;
    int i,t;
    char *r0=NULL;
    char *ra=NULL;
    char *rl=NULL;
    char *rR=NULL;
    char *rr=NULL;
    char *rt=NULL;
    char *ri=NULL;
    char *rs=NULL;
    if (argc == 1) {
        do_ls(".");
    }
    else {
        while (--argc) {
            ++argv;
            r0 = strchr(*argv,'-');
            if(r0 != NULL)
            {
                ra = strchr(*argv,'a');
                rl = strchr(*argv,'l');
                rR = strchr(*argv,'R');
                rr = strchr(*argv,'r');
                rt = strchr(*argv,'t');
                ri = strchr(*argv,'i');
                rs = strchr(*argv,'s');
            }
        argc=n;
        if(r0 == NULL) {
            while(--argc) {
                printf("%s:\n", *(argv));
                do_ls(*argv);
                argv++;
            }
        }
        else {
            if(argc == 2) do_ls_a(".");
            else {
                argc-=2;
                while(--argc>0) {
                printf("%s:\n", *(++argv));
                if(ra != NULL) do_ls_a(*(argv));
                }
            }
        }
        }
    }
    return 0;
}


