/*
+------------------------------------+-----------------------------------------+
| HEADER                             | PAYLOAD                                 |
|  (32 bits)                         |  (40*size bits)                         |
+-----+-----+------+------+----------+--------+--------+--------+--------+-----+
| src | mid | type | size | checksum | pid[0] | val[0] | pid[1] | val[1] | ... |
| (2) | (6) | (3)  | (5)  | (16)     | (8)    | (32)   | (8)    | (32)   | ... |
+-----+-----+------+------+----------+--------+--------+--------+--------+-----+
*/
/*
typedef struct head {
    unsigned int src      :  2;
    unsigned int mid      :  6;
    unsigned int type     :  3;
    unsigned int size     :  5;
    unsigned int checksum : 16;
} head;
typedef struct packet {
    unsigned int pid :  8;
    unsigned int     :  0;
    unsigned int val : 32;
} packet;*/
//es para que el packet se estructure así:

//0xpppp pppp 0000 0000 0000 0000 0000 0000
//0xvvvv vvvv vvvv vvvv vvvv vvvv vvvv vvvv

//en vez de

//0xpppp pppp vvvv vvvv vvvv vvvv vvvv vvvv
//0xvvvv vvvv 0000 0000 0000 0000 0000 0000

//en la memoria

/*
typedef struct msg {
    head header;
    packet *payload;
} msg;*/



struct msg 
{
    byte src;
    byte mid;
    byte type;
    byte size;
    word checksum;
};

#define sensor 0xC0
/*
int send(msg *m){
   byte datos[4];
   datos[0]=m->src+m->mid;
   datos[1]=m->type<<5+m->size;
   datos[2]=(m->checksum&0xFF00)>>8;
   datos[3]=m->checksum&0x00FF;
 
   Serial.write(datos,4); //envía una cadena de bytes, se le indica el largo de la cadena
   return 0;
}
*/
struct msg mensaje;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    pinMode(13, OUTPUT);
  mensaje.src=sensor;
  mensaje.mid=0;
  mensaje.type=0;
  mensaje.size=0;
  mensaje.checksum=0;
  
}

int send(msg *m){
    const int size=sizeof(m);
    //byte datos[size];
    union {
    msg* val;
    unsigned char byte[size];
    } data;
    data.val=m;
    Serial.write(data.byte,size); //envía una cadena de bytes, se le indica el largo de la cadena
    return 0;
}



void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)  
 delay(1000); // wait for a second  
digitalWrite(13, LOW); // turn the LED on (HIGH is the voltage level)    
  delay(1000); // wait for a second  
  send(&mensaje);
  
  delay(100); // delay en ms

}
