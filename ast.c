#include "ast.h"
#include "alloc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUF_SIZE 100

struct node *make_node(char *data, struct node *next)
{
    struct node *t = malloc(sizeof(*t));
    t->data = data;
    t->next = next;
    return t;
}

// constructor of each AST node
Cmd_t Cmd_Seq_new(Cmd_t left, Cmd_t right)
{
    Cmd_Seq cmd;
    NEW(cmd);
    cmd->type = CMD_SEQ;
    cmd->left = left;
    cmd->right = right;
    return (Cmd_t)cmd;
}

/*
 * Exercise 1:
 * In the ast.c, we have given the construction method of leaf nodes(Cmd_t Cmd_Atom_new(struct node *))
 * and a construction method of internal nodes(Cmd_t Cmd_Seq_new(struct Cmd_t *left, struct Cmd_t *right)).
 * Hint: you can refer to the realization of the function Cmd_Seq_new above or data structure defined
 * in the ast.h.
 */
Cmd_t Cmd_Back_new(Cmd_t back)
{
    /*
     * Todo("Exercise 1: try to complete the lost part of this function.")
     */
    Cmd_Back cmd;
    NEW(cmd);
    cmd->type = CMD_BACK;
    cmd->back = back;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Pipe_new(Cmd_t left, Cmd_t right)
{
    /*
     * Todo("Exercise 1: try to complete the lost part of this function.")
     */
    Cmd_Pipe cmd;
    NEW(cmd);
    cmd->type = CMD_PIPE;
    cmd->left = left;
    cmd->right = right;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Redir_new(Cmd_t left, Cmd_t right, int fd)
{
    /*
     * Todo("Exercise 1: try to complete the lost part of this function.")
     */
    Cmd_Redir cmd;
    NEW(cmd);
    cmd->fd = fd;
    cmd->left = left;
    cmd->right = right;
    cmd->type = CMD_REDIR;

    return (Cmd_t)cmd;
}
Cmd_t Cmd_Redir_new_read(Cmd_t left, Cmd_t right, int fd)
{
    /*
     * Todo("Exercise 1: try to complete the lost part of this function.")
     */
    Cmd_Redir_read cmd;
    NEW(cmd);
    cmd->fd = fd;
    cmd->left = left;
    cmd->right = right;
    cmd->type = CMD_REDIR_READ;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Atom_new(struct node *node)
{
    Cmd_Atom cmd;
    NEW(cmd);
    cmd->type = CMD_ATOM;
    cmd->node = node;

    return (Cmd_t)cmd;
}

/*
 * Exercise 2:
 * The function of the void Cmd_print(Cmd_t cmd) is to print out the commands entered by the user to check
 * whether the abstract syntax tree(AST) of the Step 2 is constructed correctly.
 */
void Cmd_print(struct Cmd_t *cmd)
{
    switch (cmd->type)
    {
    case CMD_ATOM:
    {
        struct Cmd_Atom *t = (struct Cmd_Atom *)cmd;
        struct node *node = t->node;
        while (node)
        {
            printf("%s ", node->data);
            node = node->next;
        }
        break;
    }

    case CMD_SEQ:
    {
        // printf("enter\n");
        struct Cmd_Seq *t = (struct Cmd_Seq *)cmd;
        // printf("enter\n");
        Cmd_t left = t->left;
        Cmd_t right = t->right;

        Cmd_print(left);
        printf("; ");
        Cmd_print(right);
        break;
    }

    case CMD_BACK:
    {
        /*
         * Todo("Exercise 2: try to complete the lost part of this function.")
         */
        Cmd_Back t = (Cmd_Back)cmd;
        Cmd_print(t->back);
        printf("& ");
        break;
    }

    case CMD_PIPE:
    {
        /*
         * Todo("Exercise 2: try to complete the lost part of this function.")
         */
        Cmd_Pipe t = (Cmd_Pipe)cmd;
        Cmd_print(t->left);
        printf("| ");
        Cmd_print(t->right);
        break;
    }

    case CMD_REDIR:
    {
        /*
         * Todo("Exercise 2: try to complete the lost part of this function.")
         */
        Cmd_Redir t = (Cmd_Redir)cmd;
        Cmd_print(t->left);
        printf("> ");
        Cmd_print(t->right);
        break;
    }
    case CMD_REDIR_READ:
    {
        /*
         * Todo("Exercise 2: try to complete the lost part of this function.")
         */

        Cmd_Redir_read t = (Cmd_Redir_read)cmd;
        Cmd_print(t->left);
        printf("< ");
        Cmd_print(t->right);
        break;
    }
    default:
        break;
    }
}

/*
 * Exercise 3:
 * Please complete the void Cmd_run(Cmd_t cmd). Regarding redirection commands, you only need to implement
 * output redirection. Commands like this: cat hello.txt > c.txt
 */
void Cmd_run(struct Cmd_t *cmd)
{
    switch (cmd->type)
    {
    case CMD_ATOM:
    {
        char *arg[10] = {0};
        int i = 0;
        struct Cmd_Atom *t = (struct Cmd_Atom *)cmd;
        struct node *node = t->node;
        while (node)
        {
            arg[i] = node->data;
            node = node->next;
            i++;
        }
        char *root = "/bin/";
        char binPath[50];
        strcpy(binPath, root);
        strcat(binPath, arg[0]);
        if (execv(binPath, arg) == -1)
        {
            char *path = "/usr/bin/";
            char usrBinPath[50];
            strcpy(usrBinPath, path);
            strcat(usrBinPath, arg[0]);

            if (execv(usrBinPath, arg) == -1)
            {
                fprintf(stderr, "cannot run command, check your input.\n");
            }
        }
        break;
    }

    case CMD_SEQ:
    {
        struct Cmd_Seq *t = (struct Cmd_Seq *)cmd;
        Cmd_t left = t->left;
        Cmd_t right = t->right;

        if (fork() == 0)
        {
            Cmd_run(left);
        }
        wait(0);
        Cmd_run(right);
        break;
    }

    case CMD_BACK:
    {
        /*
         * Todo("Exercise 3: try to complete the lost part of this function.")
         */
        Cmd_Back t = (Cmd_Back)cmd;
        if (fork() == 0)
        {
            Cmd_run(t->back);
        }
        break;
    }

    case CMD_PIPE:
    {
        /*
         * Todo("Exercise 3: try to complete the lost part of this function.")
         */
        Cmd_Pipe t = (Cmd_Pipe)cmd;
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0)
        {
            dup2(fd[1], 1); // 让fd = 1, 指向管道的写端
            Cmd_run(t->left);
        }
        wait(0);
        dup2(fd[0], 0); // 让fd = 0, 指向管道的读端
        close(fd[0]);
        close(fd[1]);
        Cmd_run(t->right);
        break;
    }
    case CMD_REDIR:
    {
        /*
         * Todo("Exercise 3: try to complete the lost part of this function.")
         */
        // echo 123 > open.txt
        // cat < a.txt
        Cmd_Redir t = (Cmd_Redir)cmd;
        int fd[2];
        pipe(fd);
        if (!fork())
        {
            dup2(fd[1], 1); // 让fd = 1, 指向管道的写端, 这样的左边的命令的结果就是输出到管道中
            Cmd_run(t->left);
        }
        wait(0);
        // 获取文件的路径
        Cmd_Atom file = (Cmd_Atom)(t->right);
        char *filename = file->node->data;
        // 以创建文件, 覆盖, 读写的方式打开, 创建文件的权限是666
        int f_fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0666);
        char buf[BUF_SIZE];
        memset(buf, 0, sizeof(buf));
        int n;
        close(fd[1]);
        while (n = read(fd[0], buf, BUF_SIZE)) // 循环读取管道中的数据
        {
            if (n == -1)
            {
                perror("read error\n");
                return;
            }
            if (n == 0)
                break;
            int tmp = write(f_fd, buf, n); // 写到文件中
            memset(buf, 0, sizeof(buf));
        }
        close(f_fd);
        close(fd[0]);
        break;
    }

        /*
         * challenge: Implement the input redirection.
         */
    case CMD_REDIR_READ:
    {
        // echo < a.txt,
        // 右边是一个文件
        Cmd_Redir_read t = (Cmd_Redir_read)cmd;
        int fd[2];
        pipe(fd);
        Cmd_Atom file = (Cmd_Atom)(t->right);
        char *path = file->node->data;
        int file_fd = open(path, O_RDONLY);
        char buf[BUF_SIZE];
        memset(buf, 0, sizeof(buf));
        int n = read(file_fd, buf, BUF_SIZE);
        write(fd[1], buf, n);
        close(file_fd);
        close(fd[1]); // 这个注意关闭
        // 然后把数据写到, 左边能读的地方
        if (fork() == 0)
        {
            dup2(fd[0], 0); // 让fd = 0, 指向管道的读端
            Cmd_run(t->left);
        }
        wait(0);
        close(fd[0]);
        break;
    }

    default:
        break;
    }

    exit(0);
}
