
void jedi(char *first_name, char *last_name,char buffer[]);
void file_read(void);
void * allocate(unsigned int size);
void * deallocate(void *, int size);
void * allocate2(char *str1,char *str2);
void * deallocate2(void *x);
unsigned int heap_usage = 0;

struct names
{
    char * first_name;
    char * last_name;
    char * jedi_name;
};

struct names n;