# Proyecto 2, SSOO.

Juego Monster Hunter: Ruz Edition.

Autores:
- Ricardo de la Maza, N° 15639363
- Jean-Paul Balazs, N° 17624282
- Jeremias Molina, N° 15635996
- Pablo Rodríguez, N° 15635961
- Clemente Guasch, N° 1563731J

## Cómo ejecutar

Compilar con el comando ```make``` tanto el servidor como el cliente. Luego, para ejecutar el servidor se debe correr ```./server``` en el directorio /P2/server y para el cliente ```./client``` en el directorio /P2/client.

## Protocolo de comunicación
Los paquetes que se utilizan son 11, y los clasificamos en:
### Enviados por el servidor:
***ID:*** 0  
***Formato payload***: mensaje de bienvenida y solicitud de nombre  
***Descripción:*** se envía este paquete a cada nuevo jugador que se conecta. 

***ID:*** 1  
***Formato payload***: solicitud de seleccionar clase  
***Descripción:*** este paquete se envía a cada jugador que ya definió su nombre.  Se envían las 3 opciones de clases que hay: 1) Cazador, 2) Médico y 3) Hacker.

***ID:*** 2  
***Formato payload***: solicitud de iniciar juego 
***Descripción:*** se envía al jugador líder luego de recibir la clase de este. 

***ID:*** 3  
***Formato payload***: solicitud de elección de monstruo  
***Descripción:*** este paquete se envía al jugador para que elija al monstruo dentro de las 3 opciones disponibles.

***ID:*** 4  
***Formato payload***: solicitud de elección de poder
***Descripción:*** se envía al jugador las opciones de poderes disponibles según su raza, en cada turno.

***ID:*** 5  
***Formato payload***: envío de información 
***Descripción:*** este paquete cumple la función de enviar información al cliente en distintas situaciones y etapas del juego, principalmente cuando no se espera una respuesta luego de enviarlo. Principalmente, canal de comunicación general.

### Enviados por el cliente:
***ID:*** 0  
***Formato payload***: nombre del jugador  
***Descripción:*** se espera el envío de este paquete luego de recibir la bienvenida por parte del servidor. El cliente ingresa su nombre de jugador deseado y es enviado al servidor.

***ID:*** 1  
***Formato payload***: clase del jugador  
***Descripción:*** se envía este paquete luego de que el servidor solicite al cliente que elija su clase. El jugador elige una de las 3 clases disponibles ingresando un 1, 2 o 3 y luego este número se envía al servidor.

***ID:*** 2  
***Formato payload***: señal de confirmación, vacío  
***Descripción:*** se envía este paquete cuando el jugador desea iniciar el juego "apretando Enter", luego de recibir la solicitud del servidor de iniciar el juego.

***ID:*** 3  
***Formato payload***: elección de monstruo  
***Descripción:*** este paquete se envía al servidor para elegir al monstruo dentro de las 3 opciones disponibles.

***ID:*** 4  
***Formato payload***: elección de poder  
***Descripción:*** este paquete contiene el poder elegido por el jugador en cada turno.
