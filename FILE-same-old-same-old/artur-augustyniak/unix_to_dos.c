#include<stdio.h>
#include<fcntl.h>

int main(void) {

    char c;
    int source = open("text.txt", O_RDONLY, 0);
    int dest = open("text_dos.txt", O_WRONLY | O_CREAT | O_TRUNC, 0700);

    while (read(source, &c, 1) == 1)
    {
        if('\n' != c)
        {
            write(dest, &c, 1);
        }
        else
        {
            write(dest, "\r\n", 2);
        }
    }

    close(source);
    close(dest);
    return 0;
}