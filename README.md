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

 - El archivo ejecutable quedará guardado en la carpeta bin/. Para ejecutar los tests en modo verbose usar '-v':

```
bin/tdsc -v tests/success/test1.txt
```

 - Para eliminar los archivos de codigo generado y el archivo binario:

```
make clean
```