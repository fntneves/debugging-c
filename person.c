#include <stdio.h>
#include <string.h>
#include "person.h"

void fill_person_data(Person * person, char * name, char * address) {
    strncpy(person->name, name, MAX_NAME);
    strncpy(person->address, address, MAX_ADDRESS);

    // The stpncpy() and strncpy() functions copy at most len characters from src into dst.  If src is less than len characters long, the remainder of dst is filled with `\0' characters.  Otherwise, dst is not terminated.
    person->name[MAX_NAME_BUFFER - 1] = '\0';
    person->address[MAX_ADDRESS_BUFFER - 1] = '\0';
}

void print_person(Person * person) {
    printf("person.name: %s\n", person->name);
    printf("person.address: %s\n", person->address);
}
