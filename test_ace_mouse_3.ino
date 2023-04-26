/*
* DISPOSICION DE PINES:
* 
* MPU => ARDUINO
* 
* VCC => 5V
* GND => GND
* SCL => A5
* SDA => A4
*
**/
//====================================================================
// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
//====================================================================
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

//====================================================================
// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
//====================================================================
MPU6050 sensor;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int x,y,aux_y,aux_x;
int vely;
bool flag=false;

void setup() {
  Serial.begin(57600);    //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor
  aux_y=0;
  aux_x=0;
  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  
}

void loop() {
  //=======================================================================================================
  // Leer las aceleraciones y velocidades angulares y las guarda en variables ax, ay, az 
  // El "&" es para obtener la direccion de memoria de la variable, no lo que contiene esa direccion
  //=======================================================================================================
  sensor.getAcceleration(&ax, &ay, &az);

  //======================================================================================
  // ESTOS COMENTARIOS SON IMPORTANTES PARA LA MEDICION:
  //
  // formula de conversion de RAW a m/s2
  // los valores raw son en G, es decir mide la aceleracion en base a la gravedad
  // la formula es: x = raw * ( (gravedad*100) / 16384.0 )
  // la gravedad esta multiplicada por 100 para pasarlo a centimetro
  // si se quiere convertir a m/s2 solamente borrar el 100
  //======================================================================================
  
  float ax_m_s2 = ax * ((9.81*100)/16384.0) ;
  float ay_m_s2 = ay * ((9.81*100)/16384.0) ;
  float az_m_s2 = az * ((9.81)/16384.0) ;
  
  //=====================================
  // Mapeo de eje y y eje x
  //=====================================
    y=map(ay,-30000,30000,-74,75);
    x=map(ax, -30000,30000,-74,75);
  //delay(90);
  //Serial.println(y);

  if(y!=0){
    aux_y=y;
    if(aux_y>0){
      flag=true;
      while(y!=0||flag==true){

        Serial.print("d");
        Serial.println(y);
        sensor.getAcceleration(&ax, &ay, &az);
        y=map(ay,-30000,30000,-74,75);
        x=map(ax, -30000,30000,-74,75);
        if(y==0&&x==0){
          flag=false;
        }
      }
    }if(aux_y<0){
      flag=true;
      while(y!=0||flag==true){

        Serial.print("i");
        Serial.println(y);
        sensor.getAcceleration(&ax, &ay, &az);
        y=map(ay,-30000,30000,-74,75);
        x=map(ax, -30000,30000,-74,75);
        if(y==0&&x==0){
          flag=false;
        }
      }
    }
  }/*
  if(x!=0){
    aux_x=x;
    if(aux_x>0){
      flag=true;
      while(x>0||flag==true){
        //delay(90);
        Serial.println("b");
        
        sensor.getAcceleration(&ax, &ay, &az);
        y=map(ay,-30000,30000,-74,75);
        x=map(ax, -30000,30000,-74,75);
        if(y==0&&x==0){
          flag=false;
        }
      }
    }else if(aux_x<0){
      flag=true;
      while(x<0||flag==true){
        //delay(90);
        Serial.println("a");
        
        sensor.getAcceleration(&ax, &ay, &az);
        y=map(ay,-30000,30000,-74,75);
        x=map(ax, -30000,30000,-74,75);
        if(y==0&&x==0){
          flag=false;
        }
      }
    }
  }
  /*
  if(y==0&&x==0){
    flag=false;
    aux_y==0;
  }
  else{
    if(y>0&&flag==false&&y>aux_y){
      Serial.print("arriba ");
      Serial.println(y);
      aux_y=y;
      flag=true;
    }else if(flag==false&&y<0)
      {
        Serial.print("abajo ");
        Serial.println(y);
        flag=true;
      }
    }*/
  }
