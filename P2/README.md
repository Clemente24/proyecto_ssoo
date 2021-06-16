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
Los paquetes que se utilizan son 12, y los clasificamos en:
### Enviados por el servidor:
1) ID: 0, formato Payload: 

### Enviados por el cliente:
***ID:*** 0  
***Formato payload***: nombre de jugador  
***Descripción:*** se espera el envío de este paquete luego de recibir la bienvenida por parte del servidor. El cliente ingresa su nombre de jugador deseado y es enviado al servidor.

***ID:*** 1  
***Formato payload***: clase del jugador  
***Descripción:*** se envía este paquete luego de que el servidor solicite al cliente que elija su clase. El jugador elige una de las 3 clases disponibles ingresando un 1, 2 o 3 y luego este número se envía al servidor.

***ID:*** 2  
***Formato payload***: mensaje de confirmación, vacío  
***Descripción:*** se envía este paquete cuando el jugador desea iniciar el juego "apretando Enter", luego de recibir la solicitud del servidor de iniciar el juego.

***ID:*** 3  
***Formato payload***: mensaje de confirmación, vacío  
***Descripción:*** se envía este paquete cuando el jugador desea iniciar el juego "apretando Enter", luego de recibir la solicitud del servidor de iniciar el juego.