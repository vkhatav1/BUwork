#include <stdio.h>
#include "lab5.h"
#include <string.h>
#include <stdlib.h>

int main()
{
    char first_name[20],last_name[20],buffer[10];
    char *first_name_ptr,*last_name_ptr;
    unsigned int size;
    first_name_ptr=first_name;
    last_name_ptr=last_name;
    printf("Enter your first name :\n");
    scanf("%s",first_name);
    printf("Enter your Last name :\n");
    scanf("%s",last_name);
    jedi(first_name_ptr,last_name_ptr,buffer);
    printf("The jedi name of %s %s is : %s\n",first_name,last_name,buffer);
    file_read();

    printf("Enter the required size for the heap\n");
    scanf("%d",&size);

    unsigned int *x=allocate(size);

    //unsigned int *y=deallocate(x,size);

    return 0;
}

void jedi(char *first_name1, char *last_name1,char buffer[])
{
    int i,j;
    for (i = 0; i < 3; ++i)
    {
        if(last_name1[i]=='\0')
            break;
        buffer[i]=last_name1[i];
    }
    for (j = 0; j < 2; ++j)
    {
        if(first_name1[j]=='\0')
            break;
        buffer[i] = first_name1[j];
        ++i;
    }
}

void jedi2(struct names n1)
{
    int x,j;
    char last_name[20],first_name[20],buffer[20];
    memset(last_name,0, sizeof(last_name));
    memset(first_name,0, sizeof(first_name));
    memset(buffer,0, sizeof(buffer));
    char *jedi_name;
    strcpy(last_name,n.last_name);
    strcpy(first_name,n.first_name);
    for (x = 0; x < 3; ++x)
    {
        if(last_name[x]=='\0')
            break;
        buffer[x]=last_name[x];
    }
    for (j = 0; j < 2; ++j)
    {
        if(first_name[j]=='\0')
            break;
        buffer[x] = first_name[j];
        ++x;
    }
    jedi_name=buffer;
    n.jedi_name=jedi_name;
}
void file_read(void)
{

    char string[25],first_name[20],last_name[20],jedi_name[10];
    char *first_name_ptr,*last_name_ptr;
    memset(string, 0, sizeof(string));

    // Append new name in the file
    printf("Enter your first and last name :\n");
    scanf("%s %s",first_name,last_name);
    printf("F: %s L:%s\n",first_name,last_name);

    FILE *fptr2 = fopen("/Users/khatav/CLionProjects/lab5/names.txt", "a");
    fprintf(fptr2, "%s", "\n");
    fprintf(fptr2, "%s", first_name);
    fprintf(fptr2, "%s"," ");
    fprintf(fptr2, "%s", last_name);

    fclose(fptr2);

    FILE *fptr1 = fopen("/Users/khatav/CLionProjects/lab5/names.txt", "r");

    // fgets() reads one line at a time and store it in the array
    while ((fgets(string, sizeof(string), fptr1))!= NULL)
    {
        char *str1, *str2;
        str1 = strtok(string, " ");
        str2 = strtok(NULL, " ");
        if(str1==NULL)
            break;
        else if(str2==NULL)
            break;
        strcpy(first_name,str1);
        strcpy(last_name,str2);
        first_name_ptr=first_name;
        last_name_ptr=last_name;
        jedi(first_name_ptr,last_name_ptr,jedi_name);
        printf("F:%s \tL:%s \t Jedi:%s \n", str1, str2,jedi_name);
        allocate2(first_name,last_name);
    }
    fclose(fptr1);
}

void * allocate(unsigned int size)
{
    unsigned int *ptr[size];
    for (int i = 0; i < size; ++i)
    {
        ptr[i]=(unsigned int)malloc(sizeof(unsigned int));
        if(ptr[i]==NULL)
        {
            printf("!!!ERROR!!!\n");
            printf("The pointer is not pointing to the heap!!!\n");
            exit(0);
        }
        heap_usage++;
    }
    int total_mem=heap_usage* sizeof(unsigned int);
    printf("Total memory allocated %d\n",total_mem);

    return ptr;

}

void * allocate2(char *str1,char *str2)
{
    char first_name[20],last_name[20],*ptr1,*ptr2,*a[2];
    strcpy(first_name,str1);
    strcpy(last_name,str2);
     ptr1=(char *)malloc(sizeof(first_name));
    ptr1=first_name;
     ptr2=(char *)malloc(sizeof(last_name));
    ptr2=last_name;
    n.first_name=ptr1;
    n.last_name=ptr2;

    jedi2(n);
    printf("Name from the structure\n");
    printf("F:%s \tL:%s \t Jedi:%s \n",n.first_name,n.last_name,n.jedi_name);
   //deallocate2(ptr1);
  // ptr1=NULL;
  // deallocate2(ptr2);
   // ptr2=NULL;
}

void * deallocate(void *x, int size)
{
    //memset(x,0,sizeof(unsigned int)*size);
    void * ptr[size];
    for (int i = 0; i < size; ++i)
    {
        free(ptr+i);
        heap_usage--;
    }
    int total_mem=heap_usage* sizeof(unsigned int);
    printf("Total memory allocated %d\n",total_mem);

    return x;
}

void * deallocate2(void *x)
{
    //memset(x,0,sizeof(unsigned int)*size);
    free(x);
}