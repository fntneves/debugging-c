#include <stdio.h>
#include <string.h>
#include "person.h"

void fill_person_data(Person * person, char * name, char * address) {
    strncpy(person->name, name, 8);
    strncpy(person->address, address, 8);
}

void print_person(Person * person) {
    printf("person.name: %s\n", person->name);
    printf("person.address: %s\n", person->address);
}
