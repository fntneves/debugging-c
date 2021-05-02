#include "person.h"

int main(int argc, char * const argv[]) {
    Person * person;

    fill_person_data(person, "Gilberto", "Portugal");
    print_person(person);

    return 0;
}
