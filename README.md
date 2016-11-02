# PracticasSCD

Prácticas de la asignatura SCD impartidas en el primer cuatrimestre del segundo curso.

Este repositorio contiene la práctica 2 y 3. La práctica 1 está en este [repositorio](https://github.com/Antkk10/Pr-ctica-1-Sistemas-Concurrentes-y-Distribuidos)

### Práctica 2 ###
#### Problema 1 ####
Obtener una versión de la clase Buffer, que se desarrolló en una sección anterior para múltiples productores y consumidores, usando las clases vistas del paquete monitor.
Documentación para el portafolio
Los alumnos redactarán un documento donde se responda de forma razonada a cada uno de los siguientes puntos:
1. Describe los cambios que has realizado sobre la clase Buffer vista anteriormente, indicando qué objetos condición has usado y el propósito de cada uno.
2. Incluye el código fuente completo de la solución adoptada.
3. Incluye un listado de la salida del programa (para 2 productores

#### Problema 2 ####

Escribir un programa Java con hebras para el problema del barbero durmiente. La barbería se implementa usando un monitor para la sincronización entre la hebra del barbero y las hebras de clientes. Dicho monitor tiene tres procedimientos exportados, que se describen aqu ́ı:
  Los clientes llaman a cortarPelo para obtener servicio del barbero, despertándolo o esperando a que termine con el cliente
  anterior.
El barbero llama a siguienteCliente para esperar la llegada de
  un nuevo cliente y servirlo.
Cuando el barbero termina de pelar al cliente actual llama a finCliente, indicándole que puede salir de la barbería y esperando a que lo haga para pasar al siguiente cliente.
