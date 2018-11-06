!/bin/sh
#PBS -N pt_manati_cpp //nombre tarea
#PBS -l nodes=20:ppn=50 //cantidad de nodos y núcleos
cd $/home/rlazo
#Comandos utilizados para ejecutar su programa
/usr/include/mpiCC matrix.cpp -o mat
/usr/include/mpiexec -n 21 ./mat
