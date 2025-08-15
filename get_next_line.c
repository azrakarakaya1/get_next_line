#include "get_next_line.h"

static char *join_free(char *s1, char *s2)
{
    char *joined = ft_strjoin(s1, s2);
    if (s1)
        free(s1);
    return (joined);
}

static int get_forward_bytes(char *src)
{
    int i;

    i = 0;
    while (src[i] && src[i] != '\n')
        i++;
    return (i);
}

static char *get_line(char *src)
{
    char *line;
    int i;
    int j;

    if (!src || !src[0])
        return (NULL);
    i = get_forward_bytes(src);
    line = ft_calloc(i + (src[i] == '\n') + 1, sizeof(char));
    if (!line)
        return (NULL);
    j = 0;
    while (j < i)
    {
        line[j] = src[j];
        j++;
    }
    if (src[i] == '\n')
        line[j] = '\n';
    return (line);
}

static char *get_src(char *src)
{
    char *new_src;
    int i;
    int j;

    i = get_forward_bytes(src);
    if (!src[i])
    {
        free(src);
        return (NULL);
    }
    i++;    //skip \n
    new_src = ft_calloc(ft_strlen(src) - i + 1, sizeof(char));
    if (!new_src)
        return (NULL);
    j = 0;
    while (src[i])
        new_src[j++] = src[i++];
    free(src);
    return (new_src);
}

/*
    Okuma sırasında read()
        0 döndürürse -> EOF
        Bufferda kalan karakterler varsa -> Son satır döndürülmeli, \n olmayabilir
        Buffer boşsa ve read() 0 döndürürse -> Fonksiyon NULL döndürür, okunacak satır yok
*/

static char *read_forward(int fd, char *src)
{
    char *buf;
    int bytes_readed;

    if (!src)
        src = ft_strdup("");
    buf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
    bytes_readed = 1;
    if (!buf)
        return (NULL);
    while (!ft_strchr(src, '\n') && bytes_readed > 0)
    {
        bytes_readed = read(fd, buf, BUFFER_SIZE);
        if (bytes_readed < 0)
        {
            free(buf);
            return (NULL);
        }
        buf[bytes_readed] = '\0';
        src = join_free(src, buf);
    }
    free(buf);
    return (src);
}

/*
    - Kalan veri kontrol edilir -> Daha önce okunmuş ama satır tamamlanmamış veri var mı? Varsa o kullanılır.
    - Dosyadan okuma yapar
        * Bufferda \n yoksa -> Dosyadan BUFFER_SIZE kadar veri okunur, önceki buffera eklenir ve bu \n veya EOF a kadar devam eder.
    - Satır çıkarılır -> Bufferdaki ilk satır (eklenen, \n dahil) alınır ve return değeri olur.
    - Kalan veri saklanır
        * Satırdan sonra bufferda kalan karakterler static olarak saklanır, sonraki get_next_line(fd) çağrısında dosyadan devam edilir.
    - Sonuç -> Fonksiyon bir satır döndürür, dosya bittiğinde veya hata oluştuğunda NULL döner. 
*/

char *get_next_line(int fd)
{
    static char *buffer_src;
    char        *line;
    char        tmp;

    if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &tmp, 0) < 0)
        return (NULL);
    buffer_src = read_forward(fd, buffer_src);
    if (!buffer_src)
        return (NULL);
    line = get_line(buffer_src);
    buffer_src = get_src(buffer_src);
    return (line);
}