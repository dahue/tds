# Taller de Diseño de Software
## Repositiorio de la materia Taller de Diseño de Software (2020)

 - Para compilar, ejecutar en la raiz del proyecto:

```
make
```

 - El archivo ejecutable quedará guardado en la carpeta bin/. Para ejecutar los tests en modo verbose usar '-v':

```
bin/tdsc tests/test1.txt # Test exitoso
bin/tdsc -v tests/test2.txt # Test exitoso
bin/tdsc tests/test3.txt # Test que falla
bin/tdsc -v tests/test4.txt # Test que falla
```

 - Para eliminar los archivos de codigo generado y el archivo binario:

```
make clean
```