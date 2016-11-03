
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>
#include <unistd.h>

using namespace std;


void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);
void Camarero( int id, int nprocesos);

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=11)
   {
      
      cout<<"El numero de procesos debe ser 11" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }

   if(rank != 10){
	   if ((rank%2) == 0)  
	      Filosofo(rank,size); // Los pares son Filosofos 
	   else 
	      Tenedor(rank,size);  // Los impares son Tenedores 
	}
	else
		Camarero(rank,size); // Camarero es el proceso 10
   MPI_Finalize( );
   return 0;
}  
// ---------------------------------------------------------------------

void Filosofo( int id, int nprocesos )
{
   int izq = (id+1) % (nprocesos-1); // Cambios nprocesos - 1
   int der = ((id+nprocesos-1)-1) % (nprocesos-1); // Cambios nprocesos - 1
   int camarero=10; 
   int peticion;
   int tenedor;
   MPI_Status status;
   
   while(1)
   {
      
	      cout << "Filósofo " << id << " solicita sentarse " << endl << flush;
	      MPI_Ssend(&peticion,1,MPI_INT,camarero,0,MPI_COMM_WORLD); // Usamos 0 para solicitar sentarse
	      cout << "Filósofo " <<id << " se sienta " << endl << flush;

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

	      MPI_Ssend(&peticion,1,MPI_INT,camarero,1,MPI_COMM_WORLD); // Usamos tag = 1 para solicitar levantarse de la silla
	      cout << "Filósofo " << id << " se levanta de la silla " << endl;
	   
	      
	      // Piensa (espera bloqueada aleatorio del proceso)
	      cout << "Filosofo " << id << " PENSANDO" << endl << flush;
	      
	      // espera bloqueado durante un intervalo de tiempo aleatorio 
	      // (entre una décima de segundo y un segundo)
	      usleep( 1000U * (100U+(rand()%900U)) );
	  
 }
}
// ---------------------------------------------------------------------

void Camarero(int id, int nprocesos){
	int peticion;
	MPI_Status status;
	int sentados=0;

	while(true){

		
		/*if(sentados==4){ // Espera a que quiera levantarse algún filósofo
			MPI_Recv(&peticion,1,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD, &status);
			sentados--;
		}
		else{*/


			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &status);

			if(status.MPI_TAG == 0 && sentados < 4){ 
				MPI_Recv(&peticion,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD, &status);
				sentados++;
			}
			else{
				MPI_Recv(&peticion,1,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD, &status);
				sentados--;
			}
		//}
	}
}
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