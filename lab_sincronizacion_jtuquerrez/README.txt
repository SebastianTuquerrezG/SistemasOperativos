Elaborado por Joan Sebastian Tuquerrez Gomez

Planteamiento del problema:

Se deberá desarrollar dos programas en C que implementen una solución
a los problemas del productor-consumidor y los filósofos. presentadas por Tanenbaum [1].
La solución se deberá implementar mediante hilos.

Problema del productor - consumidor: El programa recibirá por línea de
comandos el tamaño del búfer.
Problema de los filósofos: El programa recibirá por línea de comandos la
cantidad de filósofos, mayor a 2.

Referencias

[1] Andrew S. Tanenbaum. Modern Operating Systems. 3rd. Upper Saddle Ri-
ver, NJ, USA: Prentice Hall Press, 2007. ISBN: 9780136006633.

Notes:
    Para ejecutar la solucion al problema del productor consumidor, se ejecuta
        make
    seguido de:
        ./prodcons/prodcons #tamaño del buffer#

    Para ejecutar la solucion al problema de los filósofos, se ejecuta
        make
    seguido de:
        ./philosophers/philosophers #Cantidad de filosofos#