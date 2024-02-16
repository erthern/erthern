#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}
int compare_ct(const void *a, const void *b, char *dirname) {
    char path_a[strlen(dirname) + strlen(*(const char **)a) + 2];
    char path_b[strlen(dirname) + strlen(*(const char **)b) + 2];
    sprintf(path_a, "%s/%s", dirname, *(const char **)a);
    sprintf(path_b, "%s/%s", dirname, *(const char **)b);

    struct stat stat_a, stat_b;
    if (stat(path_a, &stat_a) == -1 || stat(path_b, &stat_b) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    if (stat_a.st_mtime < stat_b.st_mtime) return -1;
    else if (stat_a.st_mtime > stat_b.st_mtime) return 1;
    else return 0;
}
void do_ls(char *dirname, int ca, int cl, int cR, int cr, int ct, int ci, int cs) {
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        fprintf(stderr, "ls1: cannot open %s\n", dirname);
        return;
    }
    struct dirent *dirent1;
    char **dir_names = NULL;
    int count = 0;

    // 读取目录下的所有文件和目录名
    while ((dirent1 = readdir(dir)) != NULL) {
        if (!ca && dirent1->d_name[0] == '.') continue; // 如果不显示隐藏文件，并且当前文件名以'.'开头，则跳过
        dir_names = realloc(dir_names, (count + 1) * sizeof(char *));
        dir_names[count] = strdup(dirent1->d_name);
        count++;
    }
    closedir(dir);
    qsort(dir_names, count, sizeof(char *), compare); // 排序文件和目录名

    if (cr) {
        for (int i = 0; i < count / 2; i++) {
            char *temp = dir_names[i];
            dir_names[i] = dir_names[count - i - 1];
            dir_names[count - i - 1] = temp;
    }
}

    // 输出每个文件和目录
    for (int i = 0; i < count; i++) {
        char *path = malloc(strlen(dirname) + strlen(dir_names[i]) + 2); // +2 用于路径分隔符和字符串结尾符
        if (path == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        sprintf(path, "%s/%s", dirname, dir_names[i]);

        struct stat *file_stat = malloc(sizeof(struct stat));
        if (file_stat == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        if (stat(path, file_stat) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        // 如果显示长格式，则输出更多详细信息
        if (cl) {
        printf((S_ISDIR(file_stat->st_mode)) ? "d" : "-");//有哪些权限输出哪些
        printf((file_stat->st_mode & S_IRUSR) ? "r" : "-");
        printf((file_stat->st_mode & S_IWUSR) ? "w" : "-");
        printf((file_stat->st_mode & S_IXUSR) ? "x" : "-");
        printf((file_stat->st_mode & S_IRGRP) ? "r" : "-");
        printf((file_stat->st_mode & S_IWGRP) ? "w" : "-");
        printf((file_stat->st_mode & S_IXGRP) ? "x" : "-");
        printf((file_stat->st_mode & S_IROTH) ? "r" : "-");
        printf((file_stat->st_mode & S_IWOTH) ? "w" : "-");
        printf((file_stat->st_mode & S_IXOTH) ? "x" : "-");

        struct passwd *pwd = getpwuid(file_stat->st_uid);
        struct group *grp = getgrgid(file_stat->st_gid);
        printf(" %-8s %-8s", pwd->pw_name, grp->gr_name);

        // 考虑目录的大小
        if (S_ISDIR(file_stat->st_mode)) {
            printf(" %-10s", "");
        } else {
            printf(" %-10ld", (long)file_stat->st_size);
        }

        // 格式化并打印修改时间
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&file_stat->st_mtime));
        printf(" %s", time_str);
        }
        // 如果显示 inode 号码，则输出 inode
        if (ci) {
            printf("%ld ", (long)file_stat->st_ino);
        }
        if(ct){;}
        // 如果显示大小，则输出文件大小（仅对普通文件有效）
        if (cs && S_ISREG(file_stat->st_mode)) {
            printf("%-10ld ", (long)file_stat->st_blocks);
        }
        printf("%s\n", dir_names[i]);
        // 如果显示递归，且当前项是目录，则递归进入该目录
        if (cR && S_ISDIR(file_stat->st_mode) && strcmp(dir_names[i], ".") != 0 && strcmp(dir_names[i], "..") != 0) {
            printf("\n%s:\n", path);
            do_ls(path, ca, cl, cR, cr, ct, ci, cs);
        }
        free(file_stat);
        free(path);
        free(dir_names[i]);
    }
    free(dir_names);
}

int main(int argc, char *argv[]) {
    int ca = 0;
    int cl = 0;
    int cR = 0;
    int cr = 0;
    int ct = 0;
    int ci = 0;
    int cs = 0;
    int c_ = 0;

    // 处理命令行参数
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strchr(argv[i], 'a') != NULL) ca = 1;
            if (strchr(argv[i], 'l') != NULL) cl = 1;
            if (strchr(argv[i], 'R') != NULL) cR = 1;
            if (strchr(argv[i], 'r') != NULL) cr = 1;
            if (strchr(argv[i], 't') != NULL) ct = 1;
            if (strchr(argv[i], 'i') != NULL) ci = 1;
            if (strchr(argv[i], 's') != NULL) cs = 1;
            c_++;
        }
    }

    // 如果没有指定目录，默认使用当前目录
    if (c_ == argc-1) {
        do_ls(".", ca, cl, cR, cr, ct, ci, cs);
    }
    // else if(argc == 2&&argv[1][0] == '-') {
    //     do_ls(".", ca, cl, cR, cr, ct, ci, cs);
    // }
    else {
        // 处理命令行中的每个目录
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                printf("%s:\n", argv[i]);
                do_ls(argv[i], ca, cl, cR, cr, ct, ci, cs);
            }
        }
    }

    return 0;
}
