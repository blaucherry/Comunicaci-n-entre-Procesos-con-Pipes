#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define SIZE 512

// Prototipo de funcion
char *itoa(int value, char *buffer, int base);
// Función que cambia el valor de 2 caracteres (Leer:Paso por referencia)
void swap(char *x, char *y) {
  char t = *x;
  *x = *y;
  *y = t;
}
// Función para invertir buffer[i…j]
char *reverse(char *buffer, int i, int j) {
  while (i < j) {
    swap(&buffer[i++], &buffer[j--]);
  }
  return buffer;
}

char *itoa(int value, char *buffer, int base) {
  // entrada inválida
  if (base < 2 || base > 32) {
    return buffer;
  }
  // considera el valor absoluto del número
  int n = abs(value);
  int i = 0;
  while (n) {
    int r = n % base;
    if (r >= 10) {
      buffer[i++] = 65 + (r - 10);
    } else {
      buffer[i++] = 48 + r;
    }
    n = n / base;
  }
  // si el numero es 0
  if (i == 0) {
    buffer[i++] = '0';
  }
  // Si la base es 10 y el valor es negativo, la string resultante
  // va precedido de un signo menos (-)
  // Con cualquier otra base, el valor siempre se considera sin firmar
  if (value < 0 && base == 10) {
    buffer[i++] = '-';
  }
  buffer[i] = '\0'; // string de terminación nula
  // invertir la string y devolverla
  return reverse(buffer, 0, i - 1);
}

int main() {
  int numFilas = 0, numColumnas = 0, flag = 0, cont = 0, a[2], b[2], readbytes,
      indice = 0, ind1 = 0, ind2 = 0;
  double filas;
  pid_t pid;
  char buffer[SIZE], actual, aux[SIZE], aux2[SIZE], cadena[2];
  // Abre 2 pipes
  pipe(a);
  pipe(b);
  // Limpia la consola
  system("clear");

  printf("Examen 2B Procesos\n\n");

  // Ciclo do/while lee el numero de columnas que tendra la matriz, se limita a
  // elegir entre 1 y 26 columnas
  do {
    printf("¿De cuántas columnas será la matriz?\n");
    scanf("%i", &numColumnas);
    if (numColumnas < 1 || numColumnas > 26) {
      printf("El número de columnas proporcionado no es válido, intente de "
             "nuevo...\n\n");
    } else {
      flag = 1;
    }
  } while (flag != 1);

  // Convierte el numero de columnas a double para poder almacenar el residuo en
  // un double
  filas = (double)26 / (double)numColumnas;
  // Funcion ceil recibe un double y lo redondea hacia arriba. Ej 9.3 = 10
  numFilas = (int)ceil(filas);

  printf("Numero de filas= %i\n", numFilas);
  // Declara un arreglo con el numero de filas y columnas solicitado
  char arr[numFilas][numColumnas];
  char letra = 'A';
  // Ciclo for anidado para rellenar la matriz
  for (int i = 0; i < numFilas; i++) {
    for (int j = 0; j < numColumnas; j++) {
      arr[i][j] = letra;
      // Si la letra es z o un espacio se rellenara el resto de la matriz con
      // espacios
      if (letra != 'Z' && letra != ' ') {
        letra++;
      } else {
        letra = ' ';
      }
    }
  }
  // For anidado para imprimir en consola la matriz
  for (int i = 0; i < numFilas; i++) {
    for (int j = 0; j < numColumnas; j++) {
      printf("%c  ", arr[i][j]);
    }
    printf("\n");
  }

  flag = 0;
  printf("\nProceso Padre: ||(pid=%i)||\n", getpid());
  printf("================================\n\n");
  // Condicional que itera 2 veces para trabajar con ambos hijos
	
  while (cont < 2) {
    // A partir de este punto se tiene que controlar las zonas de trabajo de
    // los hijos fork crea el primer hijo Funcion fork retorna -1 en caso de
    // error, 0 para el hijo y el PID del hijo al padre, es decir, un valor
    // mayor a 1
    switch (pid = fork()) {
    // Primer case para manejo de errores
    case -1:
      printf("||Error al crear el proceso||\n\n");
      exit(0);
      break;
    // Case 0 para logica de hijo 1
    case 0:

      if (cont + 1 == 1) {
        printf("Padre: ||(pid=%i)|| Hijo %i: ||(pid=%i)||\n\n", getppid(),
               cont + 1, getpid());
        // TDA FILE para trabajar con archivos
        FILE *archivo;
        // fopen funcion para leer archivos retorna un apuntador al archivo
        // buscado. Como primer argumento toma el nombre del archivo el segundo
        // argumento es el modo de lectura para 'r' se abre el archivo para
        // propositos de lectura Si el archivo existe se retorna la direccion
        // de este a un apuntador, en caso contrario retorna NULL
        if (archivo = fopen("salida.txt", "r")) {
          // Se cierra el archivo
          fclose(archivo);
          printf("\t||El archivo de salida ya existe||\n\n");
          printf("\tHIjo %i: ||(pid=%i)|| --> Borrando archivo...\n\n",
                 cont + 1, getpid());
          // System call rm borra un archivo
          system("rm salida.txt");
          printf("\t||Archivo borrado||\n\n");
          printf("\tHijo %i: ||(pid=%i)|| --> Creando archivo de salida...\n\n",
                 cont + 1, getpid());
          // System call touch crea un archivo en blanco
          system("touch salida.txt");
          printf("\t||Archivo de salida creado con éxito||\n\n");
          // En caso de que el archivo no existe se crea sin necesidad de
          // borrarlo
        } else {
          printf("\t||El archivo de salida no existe||\n\n");
          printf("\tHijo %i: ||(pid=%i)|| --> Creando archivo de salida...\n\n",
                 cont + 1, getpid());
          system("touch salida.txt");
          printf("\t||Archivo de salida creado con éxito||\n\n");
        }
        // Mensaje para indicar que a finalizado las tareas del hijo, mandada
        // por el hijo
        printf("Padre: ||(pid=%i)|| Hijo %i: ||(pid=%i)|| --> TERMINA "
               "EJECUCION\n\n",
               getppid(), cont + 1, getpid());
        // Aumenta variable cont para comenzar a trabajar con el siguiente
        // hijo
        cont++;
        // Para llegar a esta parte se pasa de nuevo por el ciclo while que
        // tiene como condicion la variable cont, es decir, se ha realizado una
        // segunda llamada a la funcion fork por lo que esta seccion del codigo
        // sera ejecutada por el hijo2
      } else {
        printf("Padre: ||(pid=%i)|| Hijo %i: ||(pid=%i)||\n\n", getppid(),
               cont + 1, getpid());
        // Cierra descriptores del pipe [0] para lectura y [1] para escritura
        close(a[0]);
        close(b[1]);
        // strcmp funcion que compara cadenas de caracteres, devuelve cero si
        // son iguales
        while (strcmp("FIN", buffer) != 0) {
          printf(
              "\nHijo %i: ||(pid=%i)| --> Esperando mensaje de mi padre...\n",
              cont + 1, getpid());
          // comando read para leer cadena de caracteres que se recibe del pipe
          // del padre
          read(b[0], buffer, SIZE);
          printf("\nHijo %i: ||(pid=%i)|| --> Lee mensaje del pipe: %s\n",
                 cont + 1, getpid(), buffer);
          flag = 0;
          indice = 0;
          // Una vez recibe la cadena entra en un ciclo while que itera siempre
          // y cuando el valor del caracter actual en la cadena de caracteres sea
          // distinto de \0 que corresponde con el final de la cadena
          while (buffer[indice] != '\0') {
            // la funcion touper recibe un caracter y lo convierte en mayuscula
            // ignorando simbolos especiales
            actual = toupper(buffer[indice]);
            // se introduce el caracter de la iteracion actual en un arrgelo con
            // 1 caracter y un \0 que indica el fin de la cadedla
            cadena[0] = actual;
            cadena[1] = '\0';
            // Ciclo for anidado que recorre toda la matriz
            for (int i = 0; i < numFilas; i++) {
              for (int j = 0; j < numColumnas; j++) {
                // Compara el caracter actual con el caracter del for anidado,
                // ademas la bandera flag debe ser 0 y la cadena de caracteres
                // que recibe el hijo 2 debe ser distinta de FIN
                if (actual == arr[i][j] && flag == 0 &&
                    buffer[indice] != "FIN") {
                  // Se copia un espacio a la variable aux
                  strcpy(aux, "");
                  // se imprime en consola la informacion de la primera letra de
                  // la cadena
                  printf("\t%c inicia en [%i][%i]\n\n", actual, i, j);
                  // strcat concatena la segunda cadena con la primera
                  strcat(aux, cadena);         // cadena
                  strcat(aux, " Inicia en ["); // cadena Inicia en [
                  // Funcion itoa para convertir entero a cadena de caracteres
                  // aux2
                  itoa(i, aux2, 10);

                  strcat(aux, aux2); // cadena Inicia en [i
                  strcat(aux, "]["); // cadena Inicia en [i][
                                     // funcion itoa para convertir entero a
                                     // cadena de caracteres
                  itoa(j, aux2, 10);

                  strcat(aux, aux2); // cadena Inicia en [i][j
                  strcat(aux, "]");  // cadena Inicia en [i][j]
                                    // flag = 1 indica que no es el primer
                                    // caracter de la cadena que se lee
                  flag = 1;
                  // se guardan los indices en variables auxiliares
                  ind1 = i;
                  ind2 = j;
                  // Se manda la cadena aux generada al padre mediante la pipe a
                  write(a[1], aux, SIZE);
                } else {
                  // Si se llega a este punto puede ser por 3 condiciones
                  // El caracter que se lee no es el primero de la cadena
                  // El caracter que se lee pertenece a la cadena FIN mandada
                  // por el padre despues de terminar de leer el archivo

                  // condicional que compara el caracter acutal nuevamente con
                  // las condiciones anteriores pero sin contar la bandera flag
                  if (actual == arr[i][j] && buffer[indice] != "FIN") {
                    // Compara el caracter actual de la matriz con la posicion
                    // guardada en la variable ind1
                    if (i > ind1) {
                      strcpy(aux, "");
                      // imprime mensaje en consola
                      printf("\tPara %c moverse %i hacia abajo\n", actual,
                             i - ind1);
                      // genera cadena para archivo ded salida
                      strcat(aux, "Para ");
                      strcat(aux, cadena);
                      strcat(aux, " moverse ");
                      itoa(i - ind1, aux2, 10);
                      strcat(aux, aux2);
                      strcat(aux, " hacia abajo");
                      // Actualiza la posicion de la fila en ind1
                      ind1 = i;
                      // Manda el mensaje de salida al padre mediante el pipe a
                      write(a[1], aux, SIZE);
                    } else {
                      // compara la fila con el ind1
                      if (i < ind1) {
                        strcpy(aux, "");
                        // imprime mensaje en consela
                        printf("\tPara %c moverse %i hacia arriba\n", actual,
                               ind1 - i);
                        // genera cadena para archivo de salida
                        strcat(aux, "Para ");
                        strcat(aux, cadena);
                        strcat(aux, " moverse ");
                        itoa(ind1 - i, aux2, 10);
                        strcat(aux, aux2);
                        strcat(aux, " hacia arriba");
                        // Actualiza la posicion de la fila en id1
                        ind1 = i;
                        // Manda el mensaje de salida al padre mediante el pipe
                        // a
                        write(a[1], aux, SIZE);
                      } else {
                        // para este caso la fila coincide
                        strcpy(aux, "");
                        // Imprime en consola
                        printf("\tPara %c mismo renglón\n", actual);
                        // genera cadena para archivo de salida
                        strcat(aux, "Para ");
                        strcat(aux, cadena);
                        strcat(aux, " mismo renglón");
                        // ACtualiza la posicion ded la fina en id1
                        ind1 = i;
                        // Manda el mensaje ded salida al padre mediante el pipe
                        // a
                        write(a[1], aux, SIZE);
                      }
                    }
                    // Hace lo mismo con las columnas
                    if (j > ind2) {
                      strcpy(aux, "");
                      printf("\tY moverse %i hacia la derecha\n", j - ind2);
                      strcat(aux, "Y moverse ");
                      itoa(j - ind2, aux2, 10);
                      strcat(aux, aux2);
                      strcat(aux, " hacia la derecha");
                      ind2 = j;
                      write(a[1], aux, SIZE);
                    } else {
                      if (j < ind2) {
                        strcpy(aux, "");
                        printf("\tY moverse %i hacia la izquierda\n", ind2 - j);
                        strcat(aux, "Y moverse ");
                        itoa(ind2 - j, aux2, 10);
                        strcat(aux, aux2);
                        strcat(aux, " hacia la izquierda");
                        ind2 = j;
                        write(a[1], aux, SIZE);
                      } else {
                        strcpy(aux, "");
                        printf("\tMisma columna\n");
                        strcat(aux, "Misma columna");
                        write(a[1], aux, SIZE);
                      }
                    }
                  }
                }
              }
            }
            // Se lee el siguiente caracter
            indice++;
          }
          // Cuando se termina de leer la cadena el hijo 2 manda un mensaje de
          // terminado al padre mediante el pipe a
          write(a[1], "TERMINADO", SIZE);
        }
      }
      exit(0);
      break;
      // Caso default del switch ejecuta tareas del padre
    default:
      // Si cont +1 = 1 el padre espera al proceso hijo a crear el archivo de
      // salida
      if (cont + 1 == 1) {
        // Espera con wait
        wait(NULL);
        printf("Padre: ||(pid=%i)|| --> Llegó mi hijo %i, mi archivo de "
               "salida está listo\n\n",
               getpid(), cont + 1);
        // Si cont es diferente de 1 entonces se ejecutan las actividades del
        // hijo 2
      } else {
        // Cierra descriptores del padre
        close(a[1]);
        close(b[0]);
        // Nuev variable file que apunta al archivo de entrada rb para leer
        // archivo binario
        FILE *file = fopen("nombres.txt", "rb");
        // Si retorna NULL entonces no se pudo abrir el archivo
        if (file == NULL) {
          printf("||Error al abrir el archivo||\n\n");
        } else {
          // En se utilzia un segundo FILE para abrir el archivo de salida en
          // modo w para escritura
          FILE *file2 = fopen("salida.txt", "w");
          if (file2 == NULL) {
            printf("||Error al abrir el archivo||\n\n");
          } else {
            // fgets recibe una cadena decaracteres, un maximo de caracteres a
            // leer y un apuntador a un archivo en este caso para leer una linea
            // del archivo de entrada que devolvera un valor diferente de null si
            // se pudo leer el contenido
            while (fgets(buffer, SIZE, file) != NULL) {
              // Imprime en consola
              printf("Nombre: %s\n", buffer);
              // Imprime en archivo
              fprintf(file2, "Nombre: %s\n", buffer);
              // Manda la cadena de caracteres al hijo 2 por medio del pipe b
              write(b[1], buffer, SIZE);
              while (strcmp("TERMINADO", buffer) != 0) {
                // El padre recibe la cadena aux desde el hijo2 y la guarda en
                // la cadena buffer
                read(a[0], buffer, SIZE);
                // fprintf guarda el texto formateado en un archivo
                fprintf(file2, "\t%s\n", buffer);
              }
            }
            // Manda una cadena fin por el pipe al hijo 2 cuando termina de leer
            // el archivo
            write(b[1], "FIN", SIZE);
            // cierra todo 
            fclose(file);
            fflush(file2);
            fclose(file2);
          }
        }
      }
      // Aumenta el valor de la variable cont para que no coincida con el hijo 1
      // ni el 2 y rompe el ciclo
      cont++;
      break;
    }
    // Espera por el hijo 2
    wait(pid);
    // Condicional que entra cuando los 2 hijos terminaron su ejecucion.
    if (cont + 1 == 3) {
      printf("Padre: ||(pid=%i)|| --> TERMINA EJECUCION\n\n", getpid());
    }
  }

  return 0;
}