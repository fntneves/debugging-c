#include <stdio.h>
#include <string.h>
#include "person.h"

void fill_person_data(Person * person, char * name, char * address) {
    strncpy(person->name, name, MAX_NAME);
    strncpy(person->address, address, MAX_ADDRESS);
}

void print_person(Person * person) {
    printf("person.name: %s\n", person->name);
    printf("person.address: %s\n", person->address);
}
