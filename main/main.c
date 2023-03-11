#include <stdio.h>
#include <string.h>

// The registers store different types:
typedef struct Empleado_struct
{
    int id;
    char firstName[20];
    char lastName[20];
    float sueldoAnual;

} Empleado;

// Procedure 1
void updateEmpleado(Empleado *p_profesor)
{
    p_profesor->id = 152;
    strcpy(p_profesor->firstName, "John");
    strcpy(p_profesor->lastName, "Doe");
    p_profesor->sueldoAnual = 80000.48;
}

// Procedure 2
void exclamIt(char *phrase)
{
    strcat(phrase, "!");
}

void app_main(void)
{
    // Case 2:
    char phrase[20] = {"Hello world"};
    exclamIt(phrase); //
    printf("function output: %s\n", phrase);

    // Case 1:
    Empleado profesor;
    updateEmpleado(&profesor); //
    printf("profesor: %s %s de id %d tiene un sueldo anual de %.2f\n", profesor.firstName, profesor.lastName, profesor.id, profesor.sueldoAnual);
}