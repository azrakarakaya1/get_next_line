#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

void run_test(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    printf("=== Reading %s ===\n", filename);
    
    char *line;
    while((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    printf("\n=== End of %s ===\n\n", filename);
}

int main()
{
    // Test 1: Normal dosya
    run_test("file1.txt");

    // Test 2: Tek satır
    run_test("file2.txt");

    // Test 3: Boş dosya
    run_test("file3.txt");

    // Test 4: Sadece \n içeren dosya
    run_test("file4.txt");

    // Test 5: Çok uzun satır
    run_test("file5.txt");

    // Test 6: Birden fazla fd ile aynı anda okuma
    int fd1 = open("file1.txt", O_RDONLY);
    int fd2 = open("file2.txt", O_RDONLY);
    printf("==== Multi FD Test ===\n");
    char *l1 = get_next_line(fd1);
    char *l2 = get_next_line(fd2);
    printf("FD1: %s\n", l1);
    printf("FD2: %s\n", l2);
    free(l1);
    free(l2);
    close(fd1);
    close(fd2);
    printf("\n=== End Multi FD Test ===\n\n");

    // Test 7: Geçersiz FD
    printf("=== Invalid FD Test ===\n");
    char *invalid = get_next_line(-1);
    if (invalid == NULL)
        printf("OK: Returned NULL for invalid fd\n");
    else
    {
        printf("ERROR: Did not return NULL\n");
        free(invalid);
    }
    printf("=== End Invalid FD Test ===\n");

    return 0;
}