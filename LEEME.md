LEEME - Práctica MDD - Metaheurísticas

Estructura del proyecto:

- build/: Carpeta donde se generan automáticamente los archivos binarios y ejecutables tras la compilación.
- common/: Librerías y utilidades comunes necesarias para el desarrollo de la práctica.
- datos/: Archivos de entrada correspondientes a las instancias del problema MDD.
- inc/: Ficheros de cabecera (.h) que definen estructuras de datos y algoritmos.
- src/: Código fuente (.cpp) donde se implementan los algoritmos: Genético, memético, Búsqueda Local.
- Makefile: Script para automatizar la compilación y ejecución del proyecto usando CMake.
- CMakeLists.txt: Archivo de configuración para CMake.
- LICENSE: Archivo de licencia (opcional).

Compilación:

Desde el directorio 'software/', ejecutar:

    make

Esto crea el directorio 'build/', configura el proyecto con CMake y compila el ejecutable llamado 'main'.

Ejecución:

Desde el directorio 'software/', ejecutar:

    ./build/main [semilla]

Donde:
- [semilla] es un parámetro opcional que permite introducir una semilla pseudoaleatoria personalizada.
- Si no se especifica, se usa por defecto la semilla 42.

Ejemplos:

    ./build/main          # Usa semilla por defecto (42)
    ./build/main 123      # Usa semilla 123

Limpieza:

Para eliminar los archivos generados durante la compilación, ejecutar:

    make clean

Requisitos:

- CMake 3.10 o superior
- Compilador compatible con C++11 o superior
