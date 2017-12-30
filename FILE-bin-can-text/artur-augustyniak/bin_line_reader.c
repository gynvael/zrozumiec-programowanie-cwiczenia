#include<stdio.h>
#include<string.h>
#define BUFF_SIZE 1000
char buff[BUFF_SIZE];

char*  g_line(FILE *f)
{
    memset(buff, 0, BUFF_SIZE);
    long curr_pos = ftell(f);

    static long line_pos = 0;
    fseek(f, line_pos, SEEK_SET);

    char *p = buff;
    unsigned int i;
    char c;
    for(i = 0; i< BUFF_SIZE; i++)
    {
        c = fgetc(f);
        if(EOF == c || '\n' == c)
        {
            break;
        }
        *p++ = c;
    }
    line_pos = ftell(f);
    fseek(f, curr_pos, SEEK_SET);
    return buff;
}


int main(void)
{
    FILE* fp = fopen("text.txt", "rb");
    fseek(fp, 4, SEEK_SET);
    puts(g_line(fp));
    fseek(fp, 12, SEEK_SET);
    puts(g_line(fp));
    printf("pos: %li\n", ftell(fp));
    puts(g_line(fp));
    printf("pos: %li\n", ftell(fp));
    puts(g_line(fp));
    puts(g_line(fp));
    puts(g_line(fp));
    puts(g_line(fp));
    puts(g_line(fp));
    fgets(buff, 19, fp);
    fgets(buff, 19, fp);
    printf("Last: %s", buff);

    fclose(fp);
    return 0;
}