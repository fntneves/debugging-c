#include <stdlib.h>
#include "person.h"

int main(int argc, char * const argv[]) {
    Person * person = (Person *) malloc(sizeof(Person));

    fill_person_data(person, "Gilberto", "Portugal");
    print_person(person);

    return 0;
}
