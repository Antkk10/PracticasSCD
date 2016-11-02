
import monitor.*;

import java.util.Random;

// Clase para que la hebra que la use espere un tiempo aleatorio.
class aux{

	static Random genAlea = new Random();
	static void dormir_max(int milisecsMax){
	try{
		Thread.sleep(genAlea.nextInt(milisecsMax));
	   }
	catch( InterruptedException e){
		System.err.println("sleep interrumpido en 'aux.dormir_max()'");
	   }
	}
}


class Buffer extends AbstractMonitor{
	private int numSlots;
	private volatile int  cont = 0;
	private volatile double[] buffer;
	private Condition producir = makeCondition();
	private Condition consumir = makeCondition();
	public Buffer (int p_numSlots){

		numSlots=p_numSlots;
		buffer = new double[numSlots];
	}

	// Usado por el productor para depositar el producto(valor).
	public void depositar(double valor){
		enter();
		// Cambio while por if
		// Comprueba si el vector está lleno.
		if(cont == numSlots)
			// El productor se bloquea
			producir.await();
		// Depositamos el valor en el buffer
		buffer[cont] = valor;

		cont++;
		// Desbloquea al productor si está bloqueado
		consumir.signal();
		leave();
	}

	// Usado por el consumidor para extraer el producto.
	public double extraer(){
		enter();
		double valor;
		// Cambio while por if
		// Comprueba si hay algo en vector
		if(cont == 0)
			// Si no hay nada en el vector se bloquea
			consumir.await();
		// Las dos siguientes lineas obtienen el valor del vector
		cont--;
		valor = buffer[cont];
		// Desbloquea al productor en el caso de que esté bloqueado.
		producir.signal();
		leave();
		return valor;
	}
}

// Clase donde se crea la hebra consumidor
class Consumidor implements Runnable{
	private Buffer bb;
	private int veces;
	private int numC;
	Thread thr;
	public Consumidor(Buffer pbb, int pveces, int pnumC){
		bb = pbb;
		veces = pveces;
		numC =pnumC;
		// Creación de la hebra
		thr = new Thread(this, "consumidor "+this.numC);

	}

	// Función que usa la hebra
	public void run(){
		try{
			for(int i=0;i<veces;i++){
				double item = bb.extraer();
				System.out.println(thr.getName()+", consumiendo "+item);
				aux.dormir_max(1);

			}
		}
		catch(Exception e){
			System.err.println("Excepción en consumidor: " + e);
		}
	}
}

// Clase que utiliza la hebra productor
class Productor implements Runnable{
	private Buffer bb;
	private int veces;
	private int numP;
	Thread thr;
	public Productor(Buffer pbb, int pveces, int pnumP){
		bb=pbb;
		veces=pveces;
		numP=pnumP;
		// Creación de la hebra
		thr = new Thread(this, "productor "+this.numP);
	}

	public void run(){
		try{
			double item= 100*numP;
			for(int i=0;i<veces;i++){
				System.out.println(thr.getName()+", produciendo " + item);
				bb.depositar(item++);
			}
		}
		catch(Exception e){
			System.err.println("Excepcion en productor: " + e);
		}
	}
}

class Principal{

	public static void main( String[] args){
	/*try{
		Buffer buffer = new Buffer(10);
		// Se producen y consumen 50 numeros.
		int veces = 50;
		// Creamos las dos hebras
		Productor productor = new Productor(buffer,veces,1);
		Consumidor consumidor = new Consumidor(buffer,veces,1);

		// Pongo ejecutables las hebras
		productor.thr.start();
		consumidor.thr.start();
		// Esperamos a la finalizacion de las hebras
		productor.thr.join();
		consumidor.thr.join();
		}catch (InterruptedException e) 
    	{ System.out.println ("ha ocurrido una excepcion.");}
	}*/
  
    if ( args.length != 5 ) 
    {  
      System.err.println("Uso: ncons nprod tambuf niterp niterc");
      return ;
    }

    // leer parametros, crear vectores y buffer intermedio
    Consumidor[] cons      = new Consumidor[Integer.parseInt(args[0])] ;
	  Productor[]  prod      = new Productor[Integer.parseInt(args[1])] ;
	  Buffer       buffer    = new Buffer(Integer.parseInt(args[2]));
	  int          iter_cons = Integer.parseInt(args[3]);
	  int          iter_prod = Integer.parseInt(args[4]);
	  
    if ( cons.length*iter_cons != prod.length*iter_prod )
	  { 
      System.err.println("no coinciden número de items a producir con a cosumir");
      return ;
    }    

	  // crear hebras
	  for(int i = 0; i < cons.length; i++) 
	    cons[i] = new Consumidor(buffer,iter_cons,i) ;
	  for(int i = 0; i < prod.length; i++)
	    prod[i] = new Productor(buffer,iter_prod,i) ;

	  // poner en marcha las hebras
	  for(int i = 0; i < prod.length; i++) 
      prod[i].thr.start();
	  for(int i = 0; i < cons.length; i++) 
      cons[i].thr.start();
  }
}
