Proyecto dedicado al desarrollo de estaciones autónomas, conectadas a un servidor (Servidor.c), que atenderá las peticiones de los clientes, para administrar la información almacenada por las estaciones.

Ficheros:

#Servidor.c
  Codigo ejecutado por el servidor que crearà dos procesos para escuchar conexiones entrantes y en caso de aceptarlas crear un proceso (Estación o Cliente).

#EstacionESP32.cpp
  Codigo dedicado al microcontrolador de la estacion de control.

#ArduinoEstacion.ino
  Misma funcionalidad que la estacionEsp32, pero implementandolo con un microcontrolador arduino y un modulo de bluetooth.
    
