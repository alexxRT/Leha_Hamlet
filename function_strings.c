#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <malloc/malloc.h>


#define DEBUG_STAT 0


int COUNTER = 0;

const void* ERR_PTR = (const void*)0x78;
const char ERR_BYTE = 0x87;


#define CALLOC( num, type )  ( (type*) calloc ((num), sizeof (type)) )
#define SQUARE(  x  )        ( (x)*(x) )
#define   FREE( ptr )        { memset ( (ptr), ERR_BYTE, malloc_size( ptr ) ); free (ptr); *(const void**) &(ptr) = ERR_PTR; }


typedef struct _string {
    int len;
    char* string;
} str;

typedef struct _file {
    char* buff;
    int file_size;
} file_to_buf;

void my_puts (char* str);

char* my_strchr (char* str, int code);

int my_strlen (char* str);

char* my_strcpy (char* buf, char* str);

char* my_strncpy (char* buf, char* str, int num);

char* my_strcat (char* dest, char* str);

char* my_strncat (char* dest, char* str, int num);

char* my_fgets (char* buf, int num, FILE* file);

char* my_strdup (char* str);

char* my_getline(char* buf, FILE* file);

int get_size (FILE* file);

const char* str_prepare (const char* const str);

int str_cmp_begin (const void* str_param1, const void* str_param2);

int str_cmp_end (const void* str_param1, const void* str_param2);

int creat_massive (char* buf, str* struct_array);

int min (int a, int b);

void info_print (const str* str, FILE* file);

int str_prepare_end (const str* str_struct);

void massiv_fprint (const str struct_str[], size_t size, FILE* file);

void massiv_print (const str struct_str[], size_t size, const char* const param);

file_to_buf* file_read (const char file_name[], const char mode[]);

str* initialization (file_to_buf* buf, int* num_of_str);

void mem_swope (void* buf1, void* buf2, size_t mut_size);

int single_iteration (void* base, void* swope, size_t __nel, size_t __width, int (*cmp)(const void*, const void*));

void buble_sort (void* __base, size_t __nel, size_t __width, int (*cmp)(const void*, const void*));

void inside_Hoare_sort (void* base, void* extra_buf, size_t nel, size_t width, int (*cmp)(const void*, const void*));

void inside_Lomuto_sort (void* base, size_t nel, size_t width, int (*cmp)(const void*, const void*));

void custm_qsort (void* __base, size_t __nel, size_t __width, int (*cmp)(const void*, const void*));



int main()
{
    printf ("\n----------------------------------------------------------------------\n\n");

    file_to_buf* buffer = file_read ("Hamlet.txt", "r");
    FILE* new_file = fopen ("Strings.txt", "w");

    int num_of_str = 0;
    
    str* struct_array = initialization (buffer, &num_of_str);
    
    printf ("\nmy qsort started:\n");

    double start = clock ();

    custm_qsort (struct_array, num_of_str, sizeof(struct_array[0]), &str_cmp_begin);

    double end = clock ();

    printf ("it took [%lf\n]", (end - start)/(CLOCKS_PER_SEC));
    massiv_fprint (struct_array, num_of_str, new_file);
    
    custm_qsort (struct_array, num_of_str, sizeof(struct_array[0]), &str_cmp_end);
    massiv_fprint (struct_array, num_of_str, new_file);

    printf ("\nmy qsort ended\n\n");
    //printf ("counter: [%d]\n", COUNTER);

    fclose (new_file);
    FREE (buffer -> buff);
    FREE (buffer);
    FREE (struct_array);

    printf ("\n----------------------------------------------------------------------\n\n");

    return 0;
}

char* my_getline(char* buf, FILE* file){
    assert (file != NULL);
    assert (buf != NULL);
    assert (file != NULL);
    char ch;
    int i = 0;

    while ((ch = fgetc(file)) != '\n' && ch != EOF)
    {
        buf[i] = ch;
        i++;
    }

    if (ch == '\n')
        buf[i] = '\n';
        buf[i+1] = '\0';
    if (ch == EOF)
        buf[i] = '\0';

    return buf;
}

char* my_strdup (char* str) 
{
    assert (str != NULL);

    int len = my_strlen(str);
    char* new_str = CALLOC (len + 1, char);

    assert (new_str != NULL);

    my_strcat (new_str, str);

    return new_str;
}

char* my_fgets (char* buf, int num, FILE* file)
{
    assert (num >= 0);
    assert (file != NULL);
    assert (buf != NULL);
    char ch;
    int i = 0;

    while (i < num) 
    {
        ch = fgetc (file);
        if (ch != EOF && ch != '\n')
            buf [i] = ch;
        i++; 
    }

    buf [i] = '\0';

    return buf;
}

char* my_strncat (char* dest, char* str, int num) 
{
    assert (dest != NULL);
    assert (str != NULL);
    assert (num >= 0);

    int i = 0;
    int len = my_strlen (dest);

    while (i < num)
    {
        if (str[i] != '\0') 
            dest [len + i] = str [i];
        
        else 
            dest [len + i] = '\0';
        i++;
    } 
    return dest;
}

char* my_strcat (char* dest, char* str)
{
    assert (dest != NULL);
    assert (str != NULL);

    int len = my_strlen (dest);
    int i = 0;

    while (str [i] != '\0') 
    {
        dest [len + i] = str [i];
        i++;
    }
    dest[i + len] = '\0';
    return dest;
}

char* my_strncpy (char* buf, char* str, int num)
{
    assert (str != NULL);
    assert (num >= 0);

    int i = 0;
    while (i < num) 
    {
        if (str[i] != '\0') 
            buf[i] = str[i];
        else 
            buf[i] = '\0';
        i++;
    }

    return buf;
}
char* my_strcpy (char* buf, char* str) 
{
    assert (str != NULL);
    int i = 0;

    while (str[i] != '\0') {
        buf[i] = str[i];
        i ++;    
    } 
    buf[i] = '\0';

    return buf;
}

int my_strlen (char* str) 
{
    assert (str != NULL);
    int i = 0;

    while (str[i] != '\0') {
         i++;
    }
    return i;
} 

char* my_strchr (char* str, int code)
{
    assert (str != NULL);

    int i = 0;
    while (str[i] != '\0') 
    {
        int symb = str[i];

        if (symb == code) {
            assert (*(str + i) == code);
            return str + i;
        }  
        i++;
    }

    return NULL;
}

void my_puts (char* str) 
{
    assert (str != NULL);
    int i = 0;

    while (str[i] != '\0') {
        putchar (str[i]);
        i++; 
    }
    printf ("\n");
}

int get_size (FILE* file) 
{
    assert (file != NULL);
    int size = 0;
    int current_pos = ftell (file);

    fseek (file, 0, SEEK_END);
    size = ftell (file);
    fseek (file, 0, current_pos);

    return size;
}

const char* str_prepare_begin (const char* const str)
{
    assert (str != NULL);
    int i = 0;

    for (; str[i] != '\0'; i++)
    {
        if (isalpha(str[i])) {
    
            assert (isalpha(str[i]));
            return str + i;
        }
    }
    
    return str + i;
}

int str_prepare_end (const str* str_struct) 
{
    int i = str_struct -> len - 1;
    for (; i >= 0; i --)
    {
        if (isalpha(str_struct -> string[i]))
        {
            return i;
        }
    }

    return i;
}

int str_cmp_begin (const void* str_param1, const void* str_param2)
{
    const str* str1 = (const str*) str_param1;
    const str* str2 = (const str*) str_param2;

    #if DEBUG_STAT == 1
    {
    printf ("\n\nFIRST STRING\n");
    info_print (str1);
    printf ("\n\nSECOND STRING\n");
    info_print (str2);
    }
    #endif

    assert (str1 != NULL);
    assert (str2 != NULL);

    const char* s1 = str1 -> string;
    const char* s2 = str2 -> string;

    s1 = str_prepare_begin (s1); assert (str1 != NULL);
    s2 = str_prepare_begin (s2); assert (str2 != NULL);

    if (s1[0] == '\0' || s2[0] == '\0')
    {
        int diff = s1[0] - s2[0];

        return -diff;
    }

    for (int i = 0; s1[i] != '\0' && s2[i] != '\0'; i ++)
    {
        if (s1 [i] < s2 [i])
        {
            return -1;
        }
        else if (s1 [i] > s2 [i])
        {
            return 1;
        }
    }

    return 0;
}

int str_cmp_end (const void* str_param1, const void* str_param2)
{

    const str* str1 = (const str*) str_param1;
    const str* str2 = (const str*) str_param2;

    assert (str1 != NULL);
    assert (str2 != NULL);

    const char* s1 = str1 -> string; assert (s1 != NULL);
    const char* s2 = str2 -> string; assert (s2 != NULL);

    #if DEBUG_STAT == 1
    {
        info_print (str1);
        info_print (str2);
    }
    #endif

    //to do another prepare

    int begin1 = str_prepare_end (str1);
    int begin2 = str_prepare_end (str2);

    if (s1[begin1] == '\0' || s2[begin2] == '\0')
    {
        int diff = s1[begin1] - s2[begin2];
        return -diff;
    }
    // этот блок оставим

    
    int len = min (begin1, begin2);

    while (len >= 0)
    {
        if (s1 [begin1] < s2 [begin2])
        {
            return -1;
        }
        else if (s1 [begin1] > s2 [begin2])
        {
            return 1;
        }

        len --;
        begin1 --;
        begin2 --;
    }

    return 0;
}


int creat_massive (char* buf, str* struct_array) 
{
    assert (buf != NULL);
    assert (struct_array != NULL);

    int elem_num = 0;
    struct_array[0].string = buf;

    for (int i = 0; buf[i] != '\0'; i++)
    {
        if (buf [i] == '\n' || buf[i] == '\r') 
        {
            buf [i] = '\0';

            elem_num ++;
            struct_array[elem_num].string = buf + i + 1;
            struct_array[elem_num - 1].len = struct_array[elem_num].string - struct_array[elem_num - 1].string - 1;

            #if DEBUG_STAT == 1
            {
            info_print (struct_array + elem_num - 1);
            }
            #endif
        }
    }
    return elem_num;
}

int min (int a, int b) {
    return b < a ? b : a;
}

void massiv_fprint (const str struct_str[], size_t size, FILE* file)
{
    for (int i = 0; i < size; i ++)
    {   
        if (struct_str[i].string[0] != '\0')
            fprintf (file, "%d  [%s]\n", i, struct_str[i].string);
    }
}

void massiv_print (const str struct_str[], size_t size, const char* const param)
{
    printf ("\n%s:\n\n", param);

    for (int i = 0; i < size; i ++)
    {
        info_print (struct_str + i, stderr);
    }

    printf ("%s ended\n\n", param);
} 

void info_print (const str* str, FILE* file)
{
    fprintf (file, "  struct address - [%p]\n", str);

    if (!str) 
    {
        return;
    }

    fprintf (file, "  string address - [%p] string len - [%d] ", str -> string, str -> len);
    fprintf (file, "  string itself [%s]\n", str -> string);
    fprintf (file, "\n");
}

file_to_buf* file_read (const char file_name[], const char mode[])
{
    FILE* file = fopen (file_name, mode);

    assert (file != NULL);

    file_to_buf* elem = CALLOC (1, file_to_buf);

    elem -> file_size = get_size(file);

    char* buffer = CALLOC (elem -> file_size + 1, char);
    elem -> buff = buffer;

    assert (buffer != NULL);

    fread (buffer, sizeof(char), elem -> file_size, file);
    fclose (file);

    return elem;
}

str* initialization (file_to_buf* buf, int* num_of_str)
{
    str* struct_array = CALLOC (buf -> file_size, str);
    assert (struct_array != NULL);

    #if DEBUG_STAT == 1 
    {
        printf ("calloc struct returned %p\n", struct_array);
    }
    #endif

    *num_of_str = creat_massive (buf -> buff, struct_array);

    #if DEBUG_STAT == 1
    {
        printf ("massiv created\n");
    }
    #endif

    struct_array = realloc (struct_array, sizeof(struct_array[0]) * (*num_of_str));

    #if DEBUG_STAT == 1
    {
        massiv_print (struct_array, *num_of_str, "after realloc");
    }
    #endif

    return struct_array;

}

void mem_swope (void* buf1, void* buf2, size_t mut_size)
{
    assert (buf1 != NULL);
    assert (buf2 != NULL);

    short const int ll_bytes = sizeof (long long int); // 8 bytes
    short const int l_bytes  = sizeof (int); // 4 bytes
    short const int s_bytes  = sizeof (short int); // 2 bytes
    short const int c_bytes  = sizeof (char); // 1 byte

    long long int *buf1_ll = (long long int *)(buf1);
    long long int *buf2_ll = (long long int *)(buf2);
    int pos = 0;

    while (mut_size >= ll_bytes)
    {
        long long int temp = 0;
        temp = buf1_ll[pos];
        buf1_ll[pos] = buf2_ll[pos];
        buf2_ll [pos] = temp;

        pos ++;
        mut_size -= ll_bytes;
    }

    long int *buf1_l = (long int *)(buf1);
    long int *buf2_l = (long int *)(buf2);

    while (mut_size >= l_bytes)
    {
        long int temp = 0;
        temp = buf1_l[pos];
        buf1_l [pos] = buf2_l [pos];
        buf2_l [pos] = temp;

        pos ++;
        mut_size -= l_bytes;
    }

    short int *buf1_s = (short int*)buf1_l;
    short int *buf2_s = (short int*)buf2_l;

    while (mut_size >= s_bytes)
    {
        short int temp = 0;
        temp = buf1_s[pos];
        buf1_s [pos] = buf2_s [pos];
        buf2_s [pos] = temp;

        pos ++;
        mut_size -= s_bytes;
    }

    char *buf1_c = (char*)buf1_s;
    char *buf2_c = (char*)buf2_s;

    while (mut_size >= s_bytes)
    {
        char temp = 0;
        temp = buf1_c[pos];
        buf1_c [pos] = buf2_c [pos];
        buf2_c [pos] = temp;

        pos ++;
        mut_size -= c_bytes;
    }
}

int single_iteration (void* base, void* swope, size_t __nel, size_t __width, int (*cmp)(const void*, const void*))
{
    int swope_counter = 0;

    for (int i = 0; i < __nel - 1; i ++)
    {

        #if DEBUG_STAT == 1
        {
        printf ("\n\nBEFORE SWOPE:\n");

        printf ("\n\n[%d] elem\n", i);
        info_print ((str*)(base + __width * i), stderr);

        printf ("\n\n[%d] elem\n", i + 1);
        info_print ((str*)(base + __width * (i + 1)), stderr);
        }
        #endif

        int result = cmp (base + __width*i, base + __width*(i + 1));
        
        if (result > 0) 
        {

            mem_swope (base + __width*(i + 1), base + i*__width, __width);
    
            #if DEBUG_STAT == 1
            {
            printf ("\n\nAFTER SWOPE:\n");

            printf ("\n\n[%d] elem\n", i);
            info_print ((str*)(base + __width * i), stderr);

            printf ("\n\n[%d] elem\n", i + 1);
            info_print ((str*)(base + __width * (i + 1)), stderr);
            }
            #endif

            swope_counter ++;

        }    
    }

    return swope_counter;

}

void buble_sort (void* __base, size_t __nel, size_t __width, int (*cmp)(const void*, const void*)) 
{
    void* swope = CALLOC (__width, void);
    assert (swope != NULL); 

    while (single_iteration(__base, swope, __nel, __width, cmp))
        ; 

    FREE (swope);

}

void inside_Hoare_sort (void* base, void* extra_buf, size_t nel, size_t width, int (*cmp)(const void*, const void*))
{
    //COUNTER ++;
    if (nel <= 1) return;

    if (nel == 2) 
    {
        if (cmp (base, base + width) > 0)
        {
            mem_swope (base, base + width, width);
        }
        return;
    } 

    if (nel == 3)
    {
        void* max_elem = NULL;

        if (cmp (base, base + width) > 0) max_elem = base;

        else max_elem = base + width;

        if (cmp (max_elem, base + 2 * width) > 0) mem_swope (max_elem, base + 2 * width, width);

        if (cmp (base, base + width) > 0) mem_swope (base, base + width, width);

        return;
    }

    assert (base != NULL);
    assert (extra_buf != NULL);

    srand (time(NULL));
    size_t rand_index = rand() % (nel - 1);

    assert (rand_index < nel);
    memcpy (extra_buf, base + (rand_index * width), width);

    int left = 0;
    int right = nel - 1;

    do 
    {

    while (cmp (base + (left * width) , extra_buf) < 0) left  ++;
    while (cmp (base + (right * width), extra_buf) > 0) right --;
    
        if (left <= right)
        {
            mem_swope (base + (left * width), base + (right * width), width);

            left ++;
            right --;
        }

    }
    while (left <= right);


    inside_Hoare_sort (base               , extra_buf, right + 1      , width, cmp);

    if (left - right == 1)
        inside_Hoare_sort (base + left * width, extra_buf, nel - right - 1, width, cmp);
    else 
        inside_Hoare_sort (base + left * width, extra_buf, nel - right - 2, width, cmp);

}

void inside_Lomuto_sort (void* base, size_t nel, size_t width, int (*cmp)(const void*, const void*))
{
    if (nel <= 1) return;

    assert (base != NULL);

    int position = 0;
    void* base_elem = base + (nel - 1) * width;


    for (int i = 0; i < nel - 1; i ++)
    {
        if (cmp (base + width * i, base_elem) < 0)
        {
            mem_swope (base + width * position, base + width * i, width);

            position ++;
        }
    }

    mem_swope (base + width * (nel - 1), base + width * position , width);
  
    inside_Lomuto_sort (base                         , position          , width, cmp);
    inside_Lomuto_sort (base + width * (position + 1), nel - position - 1, width, cmp);
}

void custm_qsort (void* __base, size_t __nel, size_t __width, int (*cmp)(const void*, const void*))
{

    void* extra_buf = CALLOC (__width, void);
    assert (extra_buf != NULL);

    inside_Hoare_sort  (__base, extra_buf, __nel, __width, cmp);
   //inside_Lomuto_sort (__base, __nel, __width, cmp);
    
    FREE (extra_buf);
}
