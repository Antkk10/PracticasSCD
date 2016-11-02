
import monitor.* ;

import java.util.Random;

// Clase que gestiona el monitor.
class Barberia extends AbstractMonitor
{
	private Condition barbero = makeCondition();
	private Condition silla = makeCondition();
	private Condition sala_espera = makeCondition();
	private volatile int clientes = 0;
	public Barberia(){}
	// invocado por los clientes para cortarse el pelo
	public void cortarPelo(int num_thr){
		enter();
		System.out.println("Cliente "+ num_thr + " entra en la barberia.");
		// Comprobamos si hay clientes
		if(this.clientes!=0){
			// Si los hay sumamos un cliente más en la sala de espera y entra en la cola de bloqueados
			clientes++;
			System.out.println("Cliente " + num_thr +" entra en sala de espera.");
			sala_espera.await();
		}
		else{
			// Si al entrar no hay clientes suma uno y despierta al barbero (esté dormido o no).
			// En el caso de que el barbero no esté dormido (bloqueado) el signal se pierde.
			clientes++;
			System.out.println("Cliente "+ num_thr + " despierta al barbero.");
			barbero.signal();
		}

		
		System.out.println("Cliente "+ num_thr + " se sienta en la silla.");
		// El cliente se bloquea en la silla
		silla.await();
		System.out.println("Cliente " + num_thr +" sale de la barberia.");
		leave();
	}

	//Invocado por el barbero para esperar (si procede) a un nuevo cliente y sentarlo para el corte
	public void siguienteCliente(){
		enter();
			// El barbero comprueba si hay clientes
			if(clientes!=0){
				System.out.println("Barbero llama a un cliente.");
				// Despierta al primer cliente de la cola de la sala de espera
				sala_espera.signal();
				
			}
			else{
				System.out.println("Clientes actuales: " + this.clientes + " Barbero se pone a dormir.");
				// No hay clientes y se acuesta (bloquea).
				barbero.await();
				System.out.println("El barbero se despierta y se va a poner a pelar");
			}
		
		leave();
	}

	//Invocado por el barbero para indicar que ha terminado de cortar el pelo
	public void finCliente(){
		enter();
		// Quita un cliente.
		clientes--;
		
		System.out.println("Barbero termino de cortar el pelo a cliente.");
		// Levanta al cliente de la silla.
		silla.signal();
		leave();
	}
}

// Clase auxiliar que es invocada por las hebras para que "se bloqueen" un tiempo aleatorio.
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


// Plantillas de las hebras cliente.
class Cliente implements Runnable
{
	public Thread thr;
	private Barberia barberia;
	private int n_cliente;
	public Cliente(Barberia barberia, int n_cliente){
		this.barberia=barberia;
		this.n_cliente=n_cliente;
		thr = new Thread(this, "cliente "+n_cliente);
	}

	public void run(){
		while(true){
			barberia.cortarPelo(n_cliente); // El cliente espera (si procede) y se corta el pelo
			aux.dormir_max(2000);

		}
	}
}
// Plantilla de la hebra barbero
class Barbero implements Runnable
{
	public Thread thr;
	private int barbero;
	private Barberia barberia;

	public Barbero(Barberia barberia, int barbero){
		this.barbero=barbero;
		this.barberia=barberia;
		thr = new Thread(this, "barbero "+barbero);
	}

	public void run(){
		while(true){
			barberia.siguienteCliente();
			aux.dormir_max(2500); // El barbero est´a cortando el pelo
			barberia.finCliente();
		}
	}
}

class Principal3{

	public static void main(String[] args){
		try{
			Barberia barberia = new Barberia();

			Barbero barbero = new Barbero(barberia,0);


			Cliente cliente1 = new Cliente(barberia,0);
			Cliente cliente2 = new Cliente(barberia,1);
			Cliente cliente3 = new Cliente(barberia,2);

			// Iniciamos las hebras
			barbero.thr.start();
			cliente1.thr.start();
			cliente2.thr.start();
			cliente3.thr.start();

			
			barbero.thr.join();
			cliente1.thr.join();
			cliente2.thr.join();
			cliente3.thr.join();


		}catch (InterruptedException e){
			System.err.println("Ha ocurrido una excepcion en main.");
		}
	}
}
