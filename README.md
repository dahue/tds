# Taller de Diseño de Software
## Repositiorio de la materia Taller de Diseño de Software (2020)

### Requisitos
 - gcc
 - glib
 ```
sudo apt-get install libglib2.0-dev -y 
```
 - make
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
bin/tdsc -v tests/test1.txt # Test exitoso
bin/tdsc -v tests/test2.txt # Test exitoso
bin/tdsc -v tests/test3.txt # Test que falla (no disponible)
bin/tdsc -v tests/test4.txt # Test que falla (no disponible)
```

 - Para eliminar los archivos de codigo generado y el archivo binario:

```
make clean
```