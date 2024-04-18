#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Función para imprimir el prompt
void type_prompt() {
    printf("$ "); // Indicador básico de prompt
}

// Función para leer el comando y sus parámetros desde la entrada estándar
void read_command(char *command, char *parameters[]) {
    char input[100]; // Variable para almacenar la entrada del usuario
    fgets(input, sizeof(input), stdin); // Lee una línea de entrada del usuario
    sscanf(input, "%s", command); // Extrae el comando de la entrada
    parameters[0] = command; // El primer parámetro es el comando
    char *token = strtok(input, " \n"); // Tokeniza la entrada por espacios y nueva línea
    int i = 1;
    while (token != NULL && i < 20) {
        parameters[i++] = token; // Almacena los parámetros en el arreglo
        token = strtok(NULL, " \n"); // Continúa tokenizando la entrada
    }
    parameters[i] = NULL; // El último elemento debe ser un puntero nulo
}

// Función principal
int main() {
    char cmd[100], command[100], *parameters[21]; // Arreglo para almacenar el comando y sus parámetros

    // Bucle principal de la minishell
    while(1) {
        type_prompt(); // Imprime el prompt
        read_command(command, parameters); // Lee el comando y sus parámetros
        if (strcmp(command, "exit") == 0) { // Si el comando es "exit", sale del bucle
            break;
        }
        pid_t pid = fork(); // Crea un nuevo proceso hijo
        if (pid < 0) { // Maneja errores al crear el proceso hijo
            // Error al crear el proceso hijo
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // Este es el proceso hijo
            execvp(command, parameters); // Ejecuta el comando en el proceso hijo
            // Si execvp() devuelve, hubo un error
            perror("execvp");
            return 1;
        } else {
            // Este es el proceso padre
            wait(NULL); // Espera a que el proceso hijo termine
        }
    }
    return 0; // Sale del programa
}

