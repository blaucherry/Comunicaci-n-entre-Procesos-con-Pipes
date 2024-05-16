# Comunicación entre Procesos con Pipes en C

## Descripción
Este repositorio contiene ejemplos prácticos de cómo establecer y manejar la comunicación entre procesos usando pipes en C, enfocado especialmente en sistemas operativos. Se incluyen funciones clave como `itoa` para convertir enteros a cadenas en varias bases, y métodos para manipular buffers, demostrando cómo se gestionan las operaciones a nivel de bits y memoria en contextos de comunicación interprocesal.

## Motivación
El propósito de este proyecto es proporcionar una referencia educativa para estudiantes de sistemas operativos que deseen entender los mecanismos de bajo nivel de comunicación entre procesos, una parte integral de la gestión de procesos en cualquier sistema operativo moderno.

## Características
- **Conversión de enteros a cadenas:** Implementación de la función `itoa` para diferentes bases.
- **Manipulación de buffers:** Funciones para invertir y intercambiar caracteres en buffers.
- **Comunicación mediante pipes:** Ejemplos de cómo enviar y recibir datos entre procesos padre e hijos.

## Cómo usar
### Requisitos previos
Asegúrate de tener un compilador de C instalado en tu sistema, como GCC.

### Compilación y ejecución
Para compilar el código, puedes utilizar el siguiente comando en la terminal:
```bash
gcc -o programa main.c
