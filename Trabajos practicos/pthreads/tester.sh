!/bin/sh
#PBS -N pt_manati_cpp //nombre tarea
#PBS -l nodes=20:ppn=50 //cantidad de nodos y núcleos
cd $PBS_O_WORKDIR
#Comandos utilizados para ejecutar su programa
CMD BATCH g++ -std=c++11 pt_manati.cpp -o ptman -lpthread
