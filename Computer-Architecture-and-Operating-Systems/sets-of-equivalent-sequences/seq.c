
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include "seq.h"

//makra sprawdzające poprawność parametru
#define parCheck(s)                                                 \
    do{                                                             \
        if (s == NULL) {                                            \
            errno = EINVAL;                                         \
            return -1;                                              \
        }                                                           \
                                                                    \
        const char* c = s;                                          \
        while ((*c == '0') || (*c == '1') || (*c == '2')) c++;      \
                                                                    \
        if ((*c != '\0') || (c == s)) {                             \
            errno = EINVAL;                                         \
            return -1;                                              \
        }                                                           \
    }while(0)                                   

#define parCheck2(s)                                                \
    do{                                                             \
        if (s == NULL) {                                            \
            errno = EINVAL;                                         \
            return NULL;                                            \
        }                                                           \
                                                                    \
        const char* c = s;                                          \
        while ((*c == '0') || (*c == '1') || (*c == '2')) c++;      \
                                                                    \
        if ((*c != '\0') || (c == s)) {                             \
            errno = EINVAL;                                         \
            return NULL;                                            \
        }                                                           \
    }while(0)  

#define seriously(p)            \
    do{                         \
        if(p == NULL){          \
            errno = EINVAL;     \
            return -1;          \
        }                       \
    }while(0)

//makro sprawdzające poprawność alokacji pamięci
#define memCheck(p)             \
    do{                         \
        if(p == NULL){          \
            errno = ENOMEM;     \
            return NULL;        \
        }                       \
    }while(0)



typedef struct seq* seq_p;
typedef struct eqClass* classPtr;

struct eqClass{
    char* namePtr;
    int size;
};

struct seq{
    seq_p d[3]; //wskaźniki na dzieci w drzewie 
    classPtr k; //wskaźnik na klasę abstrakcji
};



//konstruktor eqCLass
classPtr new_eqClass () {
    classPtr p = (classPtr) malloc( sizeof(struct eqClass) );
    memCheck(p);

    p -> namePtr = NULL; 
    p -> size = 1; 
    return p;
}

//zwraca wskaźnik na seq_t reprezentuący ciąg
//lub NULL jeśli ciągu nie ma w strukturze
seq_p find_seq (seq_p u, char const *s) {
    if (u == NULL) return NULL;
    if (*s == '\0') return u;

    return find_seq (u -> d [*s - '0'], s + 1);
}

//usuwa z pamięci eqClass wskazywaną przez p
void kill_class (classPtr p) {
    free(p -> namePtr);
    free(p);
}

//usuwa z pamięci seq_t wskazywany przez p 
void kill_seq (seq_p p) {

    if (--p -> k -> size == 0)
        kill_class (p -> k);
    free(p);
}

//uswa seq_t wskazywany przez p 
//i wszystkie jego sufiksy ze zbioru
void help_to_kill (seq_p p) {
    if (p == NULL) return;

    for (int i = 0; i < 3; i++) {
        help_to_kill (p -> d[i]);
    }
    kill_seq(p);
}

//funkcja pomocnicza dla seq_remove
void remove_help (seq_p p, char const *s) {
   if (*(s + 1) == '\0') {
        help_to_kill (p -> d[*s - '0']);
        p -> d[*s - '0'] = NULL;
        return;
   }

   remove_help (p -> d[*s - '0'], s + 1);
}

//funkcja pomocnicza dla seq_add
int help_add (seq_p *p, char const *s) {
    seq_p pp = *p;

    if (pp != NULL) {
        if (*s == '\0')
            return 0; //ciąg jest już w zbiorze - nic nie dodajemy
        else 
            return help_add (&pp -> d [*s - '0'], s + 1);
    }
    else {
        seq_p new_one = seq_new();
        if (new_one == NULL) return -1;

        *p = new_one; 
        //tak zmieniamy też wskaźnik rodzica na wskaźnik na ciąg pod new_one

        if (*s == '\0') return 1; //dodawanie wszystkich prefiksów się udało
        
        //wystąpił błąd alokacji pamięci - cofamy dodanie p 
        if (help_add (&new_one -> d [*s - '0'], s + 1) == -1) {
            kill_seq (new_one);
            *p = NULL;
            return -1;
        }
        else {
            return 1;
        }
        //nie możesz dać nulla zamiast adresu, 
        //bo wtedy jak coś dodasz to nie zmienisz wskaźnika new_one->d; 
        //nawet jeśli pod tym adresem znajduje się NULL
    } 
}

//zwraca długość napisu
int name_length (char * name) {
    if(name == NULL) return 0;
    return strlen(name);
}

//kopiuje napis s w nowe miejsce w pamięci i zwraca wskaźnik na nie
//bądź NULL jeśli alokacja się niepowiodła / s było NULLem
char * cpy_name (char const *s) {
    if(s == NULL) return NULL;
    
    int siz = strlen( s ) + 1; //+1 zarezerwowane na znak końca słowa
    char* new_name = (char*) malloc (sizeof(char) * siz);
    char* new_help = new_name;

    if (new_name == NULL) { 
        free(new_name);
        errno = ENOMEM;
        return NULL; 
    }

    while (*s != '\0') {
        *new_help = *s;
        s ++; new_help ++;
    }
    *new_help = '\0';

    return new_name;
}

//sprawdza czy napisy wskazne przez dwa wskaźniki są sobie równe
bool eq_name(const char *a, char *b) {
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;

    while ( *a != '\0') {
        if(*a != *b) return false;
        a ++;
        b ++;
    }

    if (*b != '\0') return false;
    else return true;
}

//łączy nazwy a i b w jedną, w podanej kolejności, 
//i zwraca wskaźnik na nowy napis
//lub NULL jeśli alokacja nie powiodła się
char * merge_names (char *a, char *b) {
    int siz1 = name_length(a), siz2 = name_length(b);

    char *new_name = (char*) (malloc(sizeof(char) * (siz1 + siz2 + 1)));
    if (new_name == NULL) {
        free(new_name);
        errno = ENOMEM;
        return NULL;
    }

    char *temp_name = new_name;

    for (int i = 0; i < siz1; i++) { *temp_name = *a; temp_name ++; a++; }
    for (int j = 0; j < siz2; j++) { *temp_name = *b; temp_name ++; b++; }
    *temp_name = '\0';

    return new_name;
}

//przechodzi cały zbiór ciągów i zmienia odpowiednim ciągom klasę abstrakcji; 
void change_class (seq_p p, classPtr p1, classPtr p2, classPtr new_class) {
    if (p == NULL) return;

    if ((p -> k == p1) || (p -> k == p2)) p -> k = new_class;

    for (int i = 0; i < 3; i++) {
        change_class (p -> d[i], p1, p2, new_class);
    }
}



seq_t * seq_new (void) {
    seq_p p = (seq_p) malloc( sizeof(seq_t) );
    memCheck(p);

    p -> d[0] = p -> d[1] = p -> d[2] = NULL;
    p -> k = new_eqClass();

    if (p -> k == NULL) {
        //alokacja nowej klasy abstrakcji nie powiodła się
        free(p);
        return NULL;
    }
    else { 
        return p;
    }
};

int seq_add (seq_t *p, char const *s) {
   parCheck(s);
   seriously(p);

   return help_add (&p, s);
}

void seq_delete (seq_t *p) {
    help_to_kill (p);
}

int seq_remove (seq_t *p, char const *s) {
    parCheck(s);
    seriously(p);

    if (find_seq (p,s) == NULL) return 0;

    remove_help(p, s);
    return 1;
}


int seq_set_name (seq_t *p, char const *s, char const *n) {
    parCheck(s);
    seriously(p);

    if ((n == NULL) || (*n == '\0')) { errno = EINVAL; return -1; }

    seq_p z = find_seq (p, s);

    if (z == NULL) {
        return 0;
    }

    else if (eq_name (n, z -> k -> namePtr)) {
        return 0;
    }

    else {
        char* new_name = cpy_name (n);
        if (new_name == NULL) return -1; 
        //alokacja pamięci na nowy napis nie powiodła się

        free (z -> k -> namePtr);
        z -> k -> namePtr = new_name;
        return 1;
    }
}

char const * seq_get_name (seq_t *p, char const *s) {
    parCheck2(s);
    if(p == NULL) { errno = EINVAL; return NULL; }

    seq_p z = find_seq(p, s);
    if( (z == NULL) || (z -> k -> namePtr == NULL) ) {
        errno = 0;
        return NULL;
    }

    return z -> k -> namePtr;
}


int seq_valid (seq_t *p, char const *s) {
    parCheck(s);
    seriously(p);

    if (find_seq (p, s) == NULL) return 0;
    return 1;
}

int seq_equiv (seq_t *p, char const *s1, char const *s2) {
    parCheck(s1);
    parCheck(s2);
    seriously(p);

    seq_p p1 = find_seq (p, s1);
    seq_p p2 = find_seq (p, s2);

    if ( (p1 == NULL) || (p2 == NULL) ) 
        return 0; //co najmniej jeden ciąg nie należy do zbioru ciągów

    if (p1 -> k == p2 -> k) 
        return 0; //ciągi należą już do tej samej klasy abstrakcji


    char* a = p1 -> k -> namePtr;
    char* b = p2 -> k -> namePtr;


    classPtr new_class = new_eqClass();
    if (new_class == NULL) 
        return -1;  //alokacja nowej klasy abstrakcji nie powiodła się

    new_class -> size = p1 -> k -> size + p2 -> k -> size; 

    if (eq_name(a, b))
        new_class -> namePtr = cpy_name (a);
    else
        new_class -> namePtr = merge_names (a, b);

    if ( (new_class -> namePtr == NULL) && !(a == NULL && b == NULL) ) {
        kill_class(new_class);
        return -1; //alokacja nowej nazwy nie powiodła się
    }

    classPtr to_be_killed1 = p1 -> k, to_be_killed2 = p2 -> k;
    change_class (p, p1 -> k, p2 -> k, new_class);

    kill_class (to_be_killed1);
    kill_class (to_be_killed2);

    return 1;
}
