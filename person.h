#define MAX_NAME 8
#define MAX_ADDRESS 8
#define MAX_NAME_BUFFER 9
#define MAX_ADDRESS_BUFFER 9

typedef struct person {
    char name[MAX_NAME_BUFFER];
    char address[MAX_ADDRESS_BUFFER];
} Person;

void fill_person_data(Person * person, char * name, char * address);
void print_person(Person * person);
