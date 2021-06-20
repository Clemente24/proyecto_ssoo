# Proyecto 2, SSOO.

Juego Monster Hunter: Ruz Edition.

Autores:

- Ricardo de la Maza, N° 15639363
- Jean-Paul Balazs, N° 17624282
- Jeremias Molina, N° 15635996
- Pablo Rodríguez, N° 15635961
- Clemente Guasch, N° 1563731J

## Cómo ejecutar

Compilar con el comando `make` tanto el servidor como el cliente. Luego, para ejecutar el servidor se debe correr `./server -p <port> -i <ip>` en el directorio /P2/server y para el cliente `./client -p <port> -i <ip>` en el directorio /P2/client.

## Protocolo de comunicación

Los paquetes que se utilizan son 11, y los clasificamos en:

### Enviados por el servidor:

**_ID:_** 0  
**_Formato payload_**: mensaje de bienvenida y solicitud de nombre  
**_Descripción:_** se envía este paquete a cada nuevo jugador que se conecta.

**_ID:_** 1  
**_Formato payload_**: solicitud de seleccionar clase  
**_Descripción:_** este paquete se envía a cada jugador que ya definió su nombre. Se envían las 3 opciones de clases que hay: 1) Cazador, 2) Médico y 3) Hacker.

**_ID:_** 2  
**_Formato payload_**: solicitud de iniciar juego
**_Descripción:_** se envía al jugador líder luego de recibir la clase de este.

**_ID:_** 3  
**_Formato payload_**: solicitud de elección de monstruo  
**_Descripción:_** este paquete se envía al jugador para que elija al monstruo dentro de las 3 opciones disponibles.

**_ID:_** 4  
**_Formato payload_**: solicitud de elección de poder
**_Descripción:_** se envía al jugador las opciones de poderes disponibles según su raza, en cada turno.

**_ID:_** 5  
**_Formato payload_**: envío de información
**_Descripción:_** este paquete cumple la función de enviar información al cliente en distintas situaciones y etapas del juego, principalmente cuando no se espera una respuesta luego de enviarlo. Principalmente, canal de comunicación general.

**_ID:_** 6
**_Formato payload_**: solicitud de seguir jugando
**_Descripción:_** se envía al jugador las opciones de seguir jugando o no.

**_ID:_** 7
**_Formato payload_**: instruccion cerrar cliente
**_Descripción:_** se envia la notificación al cliente para que finalice correctamente

**_ID:_** 8
**_Formato payload_**: solicitud de elección jugador objetivo
**_Descripción:_** se envía al jugador las opciones de selección de un jugador para ser afectado por los ataques "Curar" (Medico) y "Inyeccion SQL" (Hacker)

### Enviados por el cliente:

**_ID:_** 0  
**_Formato payload_**: nombre del jugador  
**_Descripción:_** se espera el envío de este paquete luego de recibir la bienvenida por parte del servidor. El cliente ingresa su nombre de jugador deseado y es enviado al servidor.

**_ID:_** 1  
**_Formato payload_**: clase del jugador  
**_Descripción:_** se envía este paquete luego de que el servidor solicite al cliente que elija su clase. El jugador elige una de las 3 clases disponibles ingresando un 1, 2 o 3 y luego este número se envía al servidor.

**_ID:_** 2  
**_Formato payload_**: señal de confirmación, vacío  
**_Descripción:_** se envía este paquete cuando el jugador desea iniciar el juego "apretando Enter", luego de recibir la solicitud del servidor de iniciar el juego.

**_ID:_** 3  
**_Formato payload_**: elección de monstruo  
**_Descripción:_** este paquete se envía al servidor para elegir al monstruo dentro de las 3 opciones disponibles.

**_ID:_** 4  
**_Formato payload_**: elección de poder  
**_Descripción:_** este paquete contiene el poder elegido por el jugador en cada turno.

**_ID:_** 5
**_Formato payload_**: elección de jugador objetivo  
**_Descripción:_** este paquete contiene el jugador objetivo.

**_ID:_** 6
**_Formato payload_**: elección de seguir jugando
**_Descripción:_** este paquete contiene la respuesta sobre continuar jugando.

## Deciciones de diseño

- Sangrado se acumula por el resto de la batalla
- Sangrado en jugadores hace 100 por stack, debido a que tienen menos vida que un monstruo
- El veneno de espina venenosa afecta por 2 turnos ademas del primer turno en donde es infectado, haciendo la suma 3 turnos.
- El daño por status se hace despues de que todos atacan (incluido el monstruo)
- Cuando el monstruo utiliza distraer, pierde su turno.
- Cuando se elige el ataque "Curar" o "Inyeccion SQL" y no hay jugadores activos para ser elegidos, el jugador pierde el turno.
