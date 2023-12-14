#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 256

void transCadena(char *cad) {
    while (*cad) {
        *cad = toupper((char)*cad);
        cad++;
    }
}

int main(int argc, char *argv[]) {
    int n;
    int pipe1[2], pipe2[2];

    printf("Ingrese el número de procesos a crear: ");
    scanf("%d", &n);
    getchar();

    for (int i = 0; i < n; ++i) {
        
        pid_t pid_p2, pid_p3; //pid de las variables 

        char leerBuffer[BUFFER_SIZE]; 
        char escribirBuffer[BUFFER_SIZE];
	
	// Crear el primer pipe
	pipe(pipe1);
	pipe(pipe2);	

        // Crear el proceso intermediario
        pid_p2 = fork();

        if (pid_p2 == 0) {
            // Código del intermediario

            // Leer la cadena del proceso original
            read(pipe1[0], leerBuffer, BUFFER_SIZE);	
            printf("Parte 2 intermediario %d lee: %s\n", i+1, leerBuffer);

            // Pasar la cadena a mayúsculas
            transCadena(leerBuffer);

            // Enviar la cadena al proceso de conversión a mayúsculas
            write(pipe2[1], leerBuffer, strlen(leerBuffer) + 1);
            
            exit(EXIT_SUCCESS);
        } else {
            // Código del proceso original

            // Pedir al usuario que ingrese una cadena
            printf("Ingrese una cadena\n");
            printf("Proceso %d : ", i+1);
            fgets(leerBuffer,BUFFER_SIZE, stdin);
            int lon = strlen(leerBuffer);
            leerBuffer[lon-1]=0;

            // Enviar la cadena al intermediario
            write(pipe1[1], leerBuffer, strlen(leerBuffer) + 1);

            // Leer la cadena convertida a mayúsculas
            read(pipe2[0], escribirBuffer, BUFFER_SIZE);
            printf("Proceso %d lee: %s\n", i+1, escribirBuffer);
        }
    }

    return 0;
}
