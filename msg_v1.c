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
#define SIZECHECKSUM 16
#define SIZEHEAD (SIZESRC + SIZEMID + SIZETYPE + SIZESIZE + SIZECHECKSUM)

#define SIZEPID 8
#define SIZEVAL 32
#define SIZEPACKET (SIZEPID + SIZEVAL)

/* === MESSAGE STRUCTURE =======================================================
+------------------------------------+-----------------------------------------+
| HEADER                             | PAYLOAD                                 |
|  (32 bits)                         |  (40*size bits)                         |
+-----+-----+------+------+----------+--------+--------+--------+--------+-----+
| src | mid | type | size | checksum | pid[0] | val[0] | pid[1] | val[1] | ... |
| (2) | (6) | (3)  | (5)  | (16)     | (8)    | (32)   | (8)    | (32)   | ... |
+-----+-----+------+------+----------+--------+--------+--------+--------+-----+
*/
typedef struct head {
    uint src      : SIZESRC;
    uint mid      : SIZEMID;
    uint type     : SIZETYPE;
    uint size     : SIZESIZE;
    uint checksum : SIZECHECKSUM;
} head;
typedef struct packet {
    uint pid : SIZEPID;
    uint     : 0;
    uint val : SIZEVAL;
} packet;
typedef struct msg {
    head head;
    packet *payload;
} msg;

// === PRINT MSG ===============================================================
int print_binary16(uint16_t n, int size) {
    uint16_t i;
    for (i = 1 << (size-1); i>0; i = i/2) (n & i) ? printf("1") : printf("0");
    printf("  "); // Just for convenience.
    return 0;
}

int print_binary32(uint32_t n, int size) {
    uint32_t i;
    for (i = 1 << (size-1); i>0; i = i/2) (n & i) ? printf("1") : printf("0");
    printf("  "); // Just for convenience.
    return 0;
}

int print_header(head *h) {
    // Prints a message header.
    printf("header: ");
    print_binary16((uint16_t) h->src,      SIZESRC);
    print_binary16((uint16_t) h->mid,      SIZEMID);
    print_binary16((uint16_t) h->type,     SIZETYPE);
    print_binary16((uint16_t) h->size,     SIZESIZE);
    print_binary16((uint16_t) h->checksum, SIZECHECKSUM);
    printf("\n        src mid     type size   checksum\n");
    return 0;
}

int print_packet(packet *p, uint16_t i) {
    // Prints a packet.
    printf("packet[%2hu]: ", i);
    print_binary16((uint16_t) p->pid, SIZEPID);
    print_binary32((uint32_t) p->val, SIZEVAL);
    printf("\n            pid       val\n");
    return 0;
}

int print_msg(msg *m) {
    print_header(&m->head);
    for (int i = 0; i < m->head.size; ++i) {
        print_packet(&m->payload[i], i);
    }
    return 0;
}

// === GENERAL UTILITIES =======================================================
uint16_t calc_checksum(msg *m) {
    // Modulo operations are technically not needed, but are added just in case
    // the compiler gets angry with overflows. Remove if convenient.
    uint16_t csum = m->head.src;
    csum = (csum + m->head.mid)  % (1<<SIZECHECKSUM);
    csum = (csum + m->head.type) % (1<<SIZECHECKSUM);
    csum = (csum + m->head.size) % (1<<SIZECHECKSUM);
    for (int i = 0; i < m->head.size; ++i) {
        csum = (csum + m->payload[i].pid) % (1<<SIZECHECKSUM);
        csum = (csum + m->payload[i].val) % (1<<SIZECHECKSUM);
    }

    return csum;
}

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

int fill_checksum(msg *m) {
    m->head.checksum = calc_checksum(m);
    return 0;
}

int send(msg *m){
    int size=sizeof(m);
    //byte datos[size];
    union {
    msg val;
    unsigned char byte[size];
    } data;
    data.val=m;
    Serial.write(data.byte,size); //envía una cadena de bytes, se le indica el largo de la cadena
    return 0;
}

// === MAIN ====================================================================
int main(int argc, char* argv[]) {
    msg *m = (msg *) malloc(sizeof(msg));

    // Build a simple header.
    uint16_t src  = 0b10;
    uint16_t mid  = 0b011001;
    uint16_t type = 0b011;
    uint16_t size = 0b00011;
    switch (fill_header(&m->head, src, mid, type, size)) {
        case 0:
            break;
        case 1:
            printf("src is too large! Exiting...\n");
            exit(1);
        case 2:
            printf("mid is too large! Exiting...\n");
            exit(1);
        case 3:
            printf("type is too large! Exiting...\n");
            exit(1);
        case 4:
            printf("size is too large! Exiting...\n");
            exit(1);
        default:
            printf("Unexpected case! Unknown error detected. Exiting...\n");
            exit(1);
    }

    // Build an array of size packets.
    uint16_t pidarr[size];
    uint32_t valarr[size];
    for (uint16_t i = 0; i < size; ++i) {
        pidarr[i] = 0b00000000 + i;
        valarr[i] = 0x0000     + (1<<i);
    }
    switch (fill_payload(&m->payload, pidarr, valarr, size)) {
        case 0:
            break;
        case 1:
            printf("a pid is too large! Exiting...\n");
            exit(1);
        default:
            printf("Unexpected case! Unknown error detected. Exiting...\n");
            exit(1);
    }

    // Generate the checksum.
    fill_checksum(m);

    // Print the message.
    print_msg(m);

    // Leave the system clean after use.
    free(m->payload);
    free(m);



}
