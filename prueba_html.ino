#include <Wire.h>
#include <WiFi.h>

//------------------Servidor Web en puerto 80---------------------
WiFiServer server(80);
//---------------------Credenciales de WiFi-----------------------
const char* ssid     = "JP SUAREZQ";
const char* password = "3154397032";
//---------------------VARIABLES GLOBALES-------------------------
int contconexion = 0;
String header; // Variable para guardar el HTTP request
//------------------------CODIGO HTML------------------------------
String paginaInicio = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>JAM Device50</title>"
"<meta http-equiv='Content-Type' content='text/html;charset=UTF-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1'>"
"<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css' integrity='sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T' crossorigin='anonymous'>"
"</head>"
"<body>"

"<div class='container'>"
        
" </div>"

"<nav class='navbar navbar-dark bg-primary'>"
"<a class='navbar-brand' href='#'>"
"<img src='https://cdn-icons-png.flaticon.com/512/34/34643.png' width='100' height='50' class='d-inline-block align-top' alt=''>"
"<span class='text-white'>JAM Device50</span>"
"</a>"
" </nav>"


" <div class='col-md-3 col-sm-3 col-xs-3'>&nbsp;</div>"
"<div class='col-md-3 col-sm-3 col-xs-3'>&nbsp;</div>"
"<div class='col-md-3 col-sm-3 col-xs-3'>&nbsp;</div>"

" <div class='container'>"
"<div class='row justify-content-md-center'>"
" <div class='col-sm-12 col-md-4'>"
"   <div class='card' style='width: 18rem;'>"
"      <img src='https://definicion.de/wp-content/uploads/2008/10/voltaje.png' class='card-img-top' width='500' height='300'>"
"     <div class='card-body'>"
"          <h5 class='card-title'>Voltaje</h5>"
"       <div class='progress'>"
"         <div class='progress-bar progress-bar-striped' role='progressbar' style='width: 50%' aria-valuenow='10' aria-valuemin='0' aria-valuemax='100'></div>"
"         </div>"
"          <div>"
"          <span class='text-black' >50</span>"
"     </div>"
"  </div>"
"   </div>"
"  </div>"
"   <div class='col-sm-12 col-md-4'>"
"       <div class='card' style='width: 18rem;'>"
"   <img src='https://www.thoughtco.com/thmb/KEXYeuqtI767jt1XqkObGqM_Xtw=/1500x0/filters:no_upscale():max_bytes(150000):strip_icc()/GettyImages-144635668-1d9932afb0cd44a2ad33b1f0329d6ec6.jpg' class='card-img-top' width='500' height='300'>"
"    <div class='card-body'>"
"     <h5 class='card-title'>Corriente</h5>"
"     <div class='progress'>"
"          <div class='progress-bar progress-bar-striped bg-danger' role='progressbar' style='width: 70%' aria-valuenow='100' aria-valuemin='0' aria-valuemax='100'></div>"
"       </div>"
"        <div>"
"             <span class='text-black' >70</span>"
"         </div>"
"     </div>"
"   </div>"
" </div>"
"  </div>"

"<script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>"
"<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js' integrity='sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1' crossorigin='anonymous'></script>"
" <script src='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js' integrity='sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM' crossorigin='anonymous'></script>"

"<center>"
"<h3>Datos paneles solares</h3>";

String paginaFin = "</center>"
"</body>"
"</html>";





void setup(void) 
{
  Serial.begin(921600);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }

  uint32_t currentFrequency;
    
  Serial.println("Hello!");

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) {
      //para usar con ip fija
      //IPAddress ip(192,168,1,180); 
      //IPAddress gateway(192,168,1,1); 
      //IPAddress subnet(255,255,255,0); 
      //WiFi.config(ip, gateway, subnet); 
      
      Serial.println("");
      Serial.println("WiFi conectado");
      Serial.println(WiFi.localIP());
      server.begin(); // iniciamos el servidor
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
}

void loop(void) 
{
  WiFiClient client = server.available();   // Escucha a los clientes entrantes

  if (client) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";                //
    while (client.connected()) {            // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del 
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
                     
            // Muestra la página web
            client.println(paginaInicio + String(10) + paginaFin);
            
            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }


  
  delay(2000);
}
