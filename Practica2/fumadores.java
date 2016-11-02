

import monitor.* ;


import java.util.Random;

// Monitor que gestiona los semáforos de las hebras.
class Estanco extends AbstractMonitor
{
	// ingrediente a -1 significa que no el mostrador esta vacio
	private volatile int ingrediente_disponible=-1;
	private Condition estanquero = makeCondition();
	private Condition[] fumadores;

	public Estanco(){
		fumadores = new Condition[3];
		for(int i=0;i<3;i++)
			fumadores[i]= makeCondition();
	}

	// Usado por los fumadores para obtener su ingrediente.
	public void obtenerIngrediente(int miIngrediente){
		enter();
		// El cliente comprueba si su producto está disponible
		if(miIngrediente!=this.ingrediente_disponible)
			// Se bloquea el cliente para esperar su producto
			fumadores[miIngrediente].await();
		
		System.out.println("Gracias por el ingrediente " + miIngrediente + ", comienzo a fumar.");
		// Pongo el ingrediente disponible a -1 que significa que no hay ingrediente
		this.ingrediente_disponible=-1;
		// Libero al estanquero
		estanquero.signal();
		leave();
	}

	// Usado por el estanquero para poner el ingrediente.
	public void ponerIngrediente(int ingrediente){
		enter();
		// El barbero pone el ingrediente en el mostrador.
		this.ingrediente_disponible=ingrediente;
        System.out.println("Ya tienes el producto " + this.ingrediente_disponible + " disponible. ");
        
        // Liberamos al fumador que corresponda.
        fumadores[ingrediente].signal();
		leave();
	}

	// El estanquero se bloquea y espera a que se recoja el ingrediente.
	public void esperarRecogidaIngrediente(){
		enter();
		// Si en el mostrador hay producto, se bloquea el barbero.
		if(this.ingrediente_disponible!=-1)
			estanquero.await();
		leave();
	}
	

}

// Clase auxiliar para que la hebra que la use espere un tiempo aleatorio.
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

// Clase donde se ejecutan las hebras.
class Fumador implements Runnable{
	int miIngrediente;
	public Thread thr;
	// instancia donde está el monitor.
	private Estanco estanco; 

	public Fumador(int p_miIngrediente, Estanco estanco){
		this.miIngrediente = p_miIngrediente;
		this.estanco=estanco;
		thr = new Thread(this,"Fumador " + this.miIngrediente);
	}
	// Donde se ejecuta la hebra.
	public void run(){
		while(true){
			estanco.obtenerIngrediente(miIngrediente);
			aux.dormir_max(2000);
			System.out.println("Soy el fumador " + this.miIngrediente + ", termine de fumar ");

		}
	}

}

// Clase donde se ejecuta la hebra estanquero.
class Estanquero implements Runnable{
	public Thread thr;
	// Objeto donde se utiliza el monitor
	private Estanco estanco;

	public Estanquero(Estanco estanco){
		// Creación de la hebra.
		thr = new Thread(this);
		this.estanco=estanco;
	}

	// Donde se ejecuta la hebra.
	public void run(){
		int ingrediente;
		while(true){
			ingrediente = (int) (Math.random() * 3.0);
			estanco.ponerIngrediente(ingrediente);
			estanco.esperarRecogidaIngrediente();
		}
	}
}

class Principal2{

	public static void main( String[] args){
		try 
    	{
			Estanco estanco = new Estanco();

			// Creamos las hebras
			Fumador fumador1 = new Fumador(0,estanco);
			Fumador fumador2 = new Fumador(1,estanco);
			Fumador fumador3 = new Fumador(2,estanco);
			Estanquero estanquero = new Estanquero(estanco);

			// Iniciamos cada hebra
			fumador1.thr.start();
			fumador2.thr.start();
			fumador3.thr.start();
			estanquero.thr.start();

			// Espera a que terminen las hebras.
			fumador1.thr.join();
			fumador2.thr.join();
			fumador3.thr.join();
			estanquero.thr.join();			
		}catch (InterruptedException e) 
    	{ System.err.println ("ha ocurrido una excepción.");}
	}
}
