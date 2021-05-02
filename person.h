#define MAX_NAME 8
#define MAX_ADDRESS 8

typedef struct person {
    char name[MAX_NAME];
    char address[MAX_ADDRESS];
} Person;

void fill_person_data(Person * person, char * name, char * address);
void print_person(Person * person);
