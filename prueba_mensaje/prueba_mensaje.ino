#include <stdio.h>  // printf.
#include <stdint.h> // uint16_t.
#include <stdlib.h> // malloc.
#include <math.h>   // pow.

typedef unsigned int uint; // simplify life. In arduino this *should* be
                           // equivalent to uint16_t.

// Define sizes requires by program, all in bits. SIZEHEAD and SIZEPACKET should
// be divisible by 8 to get malloc byte size.
#define SIZESRC 2
#define SIZEMID 6
#define SIZETYPE 3
#define SIZESIZE 5
#define SIZEHEAD (SIZESRC + SIZEMID + SIZETYPE + SIZESIZE)

#define SIZEPID 8
#define SIZEVAL 16
#define SIZEPACKET (SIZEPID + SIZEVAL)

/* === MESSAGE STRUCTURE ============================================
+-------------------------+-----------------------------------------+
| HEADER                  | PAYLOAD                                 |
|  (32 bits)              |  (24*size bits)                         |
+-----+-----+------+------+--------+--------+--------+--------+-----+
| src | mid | type | size | pid[0] | val[0] | pid[1] | val[1] | ... |
| (2) | (6) | (3)  | (5)  | (8)    | (16)   | (8)    | (16)   | ... |
+-----+-----+------+------+--------+--------+--------+--------+-----+
*/
typedef struct head {
    uint src      : SIZESRC;
    uint mid      : SIZEMID;
    uint type     : SIZETYPE;
    uint size     : SIZESIZE;
} head;
typedef struct packet {
    uint pid : SIZEPID;
    uint     : 0;
    uint val : SIZEVAL; // NOTE: If this fails, change type to uint16_t.
} packet;
typedef struct msg {
    head head;
    packet *payload;
} msg;

// === WRITE MSG ===============================================================
int fill_header(head *h, uint16_t src, uint16_t mid, uint16_t type,
                uint16_t size) {
    if (src  >= (1<<SIZESRC))  return 1;
    if (mid  >= (1<<SIZEMID))  return 2;
    if (type >= (1<<SIZETYPE)) return 3;
    if (size >= (1<<SIZESIZE)) return 4;

    h->src  = src;
    h->mid  = mid;
    h->type = type;
    h->size = size;

    return 0;
}

int fill_payload(packet **pl, uint16_t *pids, uint32_t *vals, uint16_t size) {
    for (int i = 0; i < size; ++i) if (pids[i] >= (1<<SIZEPID)) return 1;
    // overflows for vals should be detected by the compiler due to its size.

    *pl = malloc(sizeof(packet) * size);

    for (int i = 0; i < size; ++i) {
       (*pl)[i].pid = pids[i];
       (*pl)[i].val = vals[i];
    }

    return 0;
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

// === MAIN ====================================================================
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(13, OUTPUT);
}

void loop() {
    msg *m = (msg *) malloc(sizeof(msg));
    // Build a simple header.
    uint16_t src  = 0b10;
    uint16_t mid  = 0b011001;
    uint16_t type = 0b011;
    uint16_t size = 0b00011;
    fill_header(&m->head, src, mid, type, size);


    // Build an array of size packets.
    uint16_t pidarr[size];
    uint32_t valarr[size];
    for (uint16_t i = 0; i < size; ++i) {
        pidarr[i] = 0b00000000 + i;
        valarr[i] = 0x0000     + (1<<i);
    }
    fill_payload(&m->payload, pidarr, valarr, size);

    // Send the message.
    send(m);

    // Leave the system clean after use.
    free(m->payload);
    free(m);
    delay(100); // delay en ms

    digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000); // wait for a second
    digitalWrite(13, LOW); // turn the LED on (HIGH is the voltage level)
    delay(1000); // wait for a second

}
