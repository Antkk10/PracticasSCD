#include <mpi.h>
#include <iostream>
#include <math.h>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"


#define tag_Productor    0
#define Buffer       5
#define tag_Consumidor   2
#define ITERS       20
#define TAM          5

using namespace std;

// ---------------------------------------------------------------------

// Cada productor va a generar un producto distinto al de los demás productores.
// El primer productor producirá el número 0,5,10,15... el segundo 1,6,11,16...
// el tercero 2,7,12,17,.... el cuarto 3,8,13,18,23.... el quinto 4,9,14,....
void productor()
{
   int value, rank;

   MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
   
   for ( unsigned int i=0; i < ITERS/5 ; i++ )
   { 
      value = i+(rank*100) ;
      cout << "Productor " << rank << " produce valor " << value << endl << flush ;
      
      // espera bloqueado durante un intervalo de tiempo aleatorio 
      // (entre una décima de segundo y un segundo)
      usleep( 1000U * (100U+(rand()%900U)) );
      
      // enviar 'value'
      MPI_Send( &value, 1, MPI_INT, Buffer, tag_Productor, MPI_COMM_WORLD );
   }
}
// ---------------------------------------------------------------------

void buffer()
{
   int        value[TAM] , 
              peticion , 
              pos  =  0,
              rama ;
   MPI_Status status ;
   
   for( unsigned int i=0 ; i < ITERS*2 ; i++ )
   {  
      if ( pos==0 )      // el consumidor no puede consumir
         rama = 0 ;        
      else if (pos==TAM) // el productor no puede producir
         rama = 1 ;           
      else               // ambas guardas son ciertas
      {
         // leer 'status' del siguiente mensaje (esperando si no hay)
         MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
         
         // calcular la rama en función del origen del mensaje
         if ( status.MPI_TAG  == tag_Productor ) 
            rama = 0 ; 
         else 
            rama = 1 ;
      }
      switch(rama)
      {
         case 0:
            MPI_Recv( &value[pos], 1, MPI_INT, MPI_ANY_SOURCE, tag_Productor, MPI_COMM_WORLD, &status );
            cout << "Buffer recibe " << value[pos] << " de Productor " << status.MPI_SOURCE << endl << flush;  
            pos++;
            break;
         case 1:
            MPI_Recv( &peticion, 1, MPI_INT, MPI_ANY_SOURCE, tag_Consumidor, MPI_COMM_WORLD, &status );
            cout << "Buffer envía " << value[pos-1] << " a Consumidor " << status.MPI_SOURCE << " " << endl << flush;
            MPI_Send( &value[pos-1], 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
              
            pos--;
            break;
      }     
   }
}   
   
// ---------------------------------------------------------------------

void consumidor()
{
   int         value, 
               peticion = 1 ; 
   float       raiz ;
   MPI_Status  status ;
   int rank;

   MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
 
   for (unsigned int i=0;i<ITERS/4;i++)
   {
      MPI_Send( &peticion, 1, MPI_INT, Buffer, tag_Consumidor, MPI_COMM_WORLD ); 
      MPI_Recv ( &value, 1,    MPI_INT, Buffer, 0, MPI_COMM_WORLD,&status );
      cout << "Consumidor " << rank << " recibe valor " << value << " de Buffer " << endl << flush ;
      
      // espera bloqueado durante un intervalo de tiempo aleatorio 
      // (entre una décima de segundo y un segundo)
      usleep( 1000U * (100U+(rand()%900U)) );
      
      raiz = sqrt(value) ;
   }
}
// ---------------------------------------------------------------------

int main(int argc, char *argv[]) 
{
   int rank,size; 
   
   // inicializar MPI, leer identif. de proceso y número de procesos
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   // inicializa la semilla aleatoria:
   srand ( time(NULL) );
   
   // comprobar el número de procesos con el que el programa 
   // ha sido puesto en marcha (debe ser 10)
   if ( size != 10 ) 
   {
      cout<< "El numero de procesos debe ser 10 "<<endl;
      return 0;
   } 
   
   // verificar el identificador de proceso (rank), y ejecutar la
   // operación apropiada a dicho identificador
   // Para los procesos de 0 a 4 ejecutar los productores
   if ( rank < 5 ) 
      productor();
   // Para el proceso 5 ejecutará el buffer
   else if ( rank == 5 ) 
      buffer();
   // Para los procesos de 6 a 9 los consumidores.
   else 
      consumidor();
   
   // al terminar el proceso, finalizar MPI
   MPI_Finalize( );
   return 0;
}
