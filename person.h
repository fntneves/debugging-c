typedef struct person {
    char name[8];
    char address[8];
} Person;

void fill_person_data(Person * person, char * name, char * address);
void print_person(Person * person);
