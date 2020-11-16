# Taller de Diseño de Software
## Repositiorio de la materia Taller de Diseño de Software (2020)

### Requisitos
 - glib
 ```
 sudo apt-get install libglib2.0-dev -y
 ```
 - gcc
 - make
### Compilar y ejecutar
 - Para compilar, ejecutar en la raiz del proyecto:

```
make
```

 - Ejecutar todos los tests:

```
make test
```

 - Para eliminar los archivos de codigo generado y el archivo binario:

```
make clean
```

 - El archivo ejecutable quedará guardado en la carpeta bin/. Para ejecutar el compilador:

```
bin/tdsc -o path_to_output_file.s path_to_source_file.tds
```
Nota: Si no se define un archivo de salida se creará un archivo "o.s" en la carpeta corriente con el código assembler.

