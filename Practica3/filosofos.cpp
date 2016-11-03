
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>
#include <unistd.h>

using namespace std;


void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=10)
   {
      
      cout<<"El numero de procesos debe ser 10" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }
   
   if ((rank%2) == 0)  
      Filosofo(rank,size); // Los pares son Filosofos 
   else 
      Tenedor(rank,size);  // Los impares son Tenedores 
   
   MPI_Finalize( );
   return 0;
}  
// ---------------------------------------------------------------------

void Filosofo( int id, int nprocesos )
{
   int izq = (id+1) % nprocesos;
   int der = ((id+nprocesos)-1) % nprocesos;   
   int peticion;
   int tenedor;
   MPI_Status status;
   
   while(1)
   {
      
      if(id==0){
	      cout << "Filosofo "<<id<< " solicita tenedor izq ..." << izq << endl << flush;
	      MPI_Ssend(&peticion,1,MPI_INT,izq,0,MPI_COMM_WORLD);
	      
	      
	      cout << "Filosofo "<<id<< " coge tenedor izquierdo... " << izq << endl << flush;

	      cout << "Filosofo " << id << " solicita tenedor derecho. " << der << endl << flush;
	      MPI_Ssend(&peticion,1,MPI_INT,der,0,MPI_COMM_WORLD);
	      
	      
	      cout <<"Filosofo "<<id<< " coge tenedor der ..." << der << endl << flush;
	      
	  
	      cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
	      sleep((rand() % 3)+1);  //comiendo
	      
	      MPI_Ssend(&peticion,1,MPI_INT,izq,0,MPI_COMM_WORLD);
	      cout <<"Filosofo "<<id<< " suelta tenedor izq ..." << izq << endl << flush;

	      MPI_Ssend(&peticion,1,MPI_INT,der,0,MPI_COMM_WORLD);
	      cout <<"Filosofo "<<id<< " suelta tenedor der ..." << der << endl << flush;
	   
	      
	      // Piensa (espera bloqueada aleatorio del proceso)
	      cout << "Filosofo " << id << " PENSANDO" << endl << flush;
	      
	      // espera bloqueado durante un intervalo de tiempo aleatorio 
	      // (entre una décima de segundo y un segundo)
	      usleep( 1000U * (100U+(rand()%900U)) );
	  }
	  else{
	      cout << "Filosofo "<<id<< " solicita tenedor der ..." << der << endl << flush;
	      MPI_Ssend(&peticion,1,MPI_INT,der,0,MPI_COMM_WORLD);
	      
	      
	      cout << "Filosofo "<<id<< " coge tenedor derecho... " << der << endl << flush;

	      cout << "Filosofo " << id << " solicita tenedor izquierdo. " << izq << endl << flush;
	      MPI_Ssend(&peticion,1,MPI_INT,izq,0,MPI_COMM_WORLD);
	      
	      
	      cout <<"Filosofo "<<id<< " coge tenedor izq ..." << izq << endl << flush;
	      
	  
	      cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
	      sleep((rand() % 3)+1);  //comiendo
	      
	      MPI_Ssend(&peticion,1,MPI_INT,der,0,MPI_COMM_WORLD);
	      cout <<"Filosofo "<<id<< " suelta tenedor der ..." << der << endl << flush;

	      MPI_Ssend(&peticion,1,MPI_INT,izq,0,MPI_COMM_WORLD);
	      cout <<"Filosofo "<<id<< " suelta tenedor izq ..." << izq << endl << flush;
	   
	      
	      // Piensa (espera bloqueada aleatorio del proceso)
	      cout << "Filosofo " << id << " PENSANDO" << endl << flush;
	      
	      // espera bloqueado durante un intervalo de tiempo aleatorio 
	      // (entre una décima de segundo y un segundo)
	      usleep( 1000U * (100U+(rand()%900U)) );
	  }
 }
}
// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos)
{
  int buf; 
  MPI_Status status; 
  int Filo;
  
  while( true )
  {
    
    
    MPI_Recv(&buf,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD, &status);   
    Filo = status.MPI_SOURCE;
    cout << "Ten. " << id << " recibe petic. de " << Filo << endl << flush;
    
    MPI_Recv(&buf,1,MPI_INT,Filo,0,MPI_COMM_WORLD, &status);
    cout << "Ten. " << id << " recibe liberac. de " << Filo << endl << flush; 
  }
}
// ---------------------------------------------------------------------