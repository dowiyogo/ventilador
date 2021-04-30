# CCTVal IMV 2.0 Communication Protocol v0.4
Protocol designed to manage communication for the different components inside the CCTVal Invasive Mechanical Ventilator.
These three components are:
* **Coordinator**: Acts as the coordinator between the UI and the ventilator.
Hosts the User Interface.
Hosted in a Raspberry Pi, written in Python<sup>1</sup>.
Communication is handled via the Python serial library.
* **Ventilator Manager**: Manager of the ventilator.
Sets the relevant parameters.
Hosted in an Arduino Mega, written in C.

Communication is done via two USB connections: one between the Coordinator and the Blender Manager, and one between the Coordinator and the Sensors Manager.

```
+-------------+
| Coordinator |
+------+------+
       |
       |
+------+------+
| Ventilator  |
|   Manager   |
+-------------+
```

--------------------------------------------------------------------------------
### Message Description
Messages are separated in two, a fixed-size header and a variable-size payload or body.
For the purposes of this file, messages are described as a `struct` named `msg`<sup>2</sup>.

```
+-------------------------+-----------------------------------------+
| HEADER                  | PAYLOAD                                 |
|  (16 bits)              |  (24*size bits)                         |
+-----+-----+------+------+--------+--------+--------+--------+-----+
| src | mid | type | size | pid[0] | val[0] | pid[1] | val[1] | ... |
| (2) | (6) | (3)  | (5)  | (8)    | (16)   | (8)    | (16)   | ... |
+-----+-----+------+------+--------+--------+--------+--------+-----+
```

#### Header
The header is built by five fixed-size components:
* `src` (2 bits): Device that emitted the message.
Can be:
    * `00`: The coordinator.
    * `01`: *unused*.
    * `10`: The ventilator manager.
    * `11`: *unused*.
* `mid` (6 bits): Identifier of the message.
Used by the receiver to check if the message has already been sent.
A message should have a permanent MID, and the emitter's MID counter is updated by one every time a message is sent (so that the next message has the same MID plus one).
* `type`<sup>3</sup> (3 bits): Type of the message, explaining the receiver how to interpret it.
Can be:
    * `000`: ACK (no payload).
    * `001`: NAK (no payload).
    * `010`: REQ<sup>4</sup>.
    * `011`: *unused*.
    * `100`: Measurements.
    * `101`: Alarms.
    * `110`: Control parameters.
    * `111`: Configuration parameters.
* `size` (5 bits): Size of the payload.
Describes the number of identifier-value pairs in the payload, so the total size of the payload is `24*size` bits.

#### Payload
Then, the payload is built by an array of identifier-value pairs:
* `pid` (8 bits): Identifier of the parameter sent.
Each `type`:`pid` combination is unique and is described in the list at the end of this file.
* `val` (16 bits): Value of the parameter.
The value can either be a 16-bit `int` or a `float`, as is described in the list at the end of the file.

#### Functions<sup>5</sup>
The list of functions used to manipulate the messages follows:
* `int open(socket *s)`: Open socket `s` for sending or receiving messages.
Returns `0` if successful, an error code otherwise (as defined in the error code list by the end of the file).
* `int close(socket *s)`: Close socket `s`.
Returns `0` if successful, an error code otherwise.
* `int send(msg *m, socket *s)`: Send message `m` to socket `s`.
Returns `0` if successful, an error code otherwise.
* `int send_ack(byte mid, socket *s)`: Sends an `ACK` to socket `s`.
The `ACK` has the same `mid` as the identifier of the message that it's acknowledging.
* `int send_nak(byte mid, socket *s)`: Sends a `NAK` to socket `s`.
The `NAK` has the same `mid` as the identifier of the message that it's acknowledging.
* `int recv(msg *m, socket *s)`: Receive a message from socket `s` and write it to `m`, which would usually be an empty `struct` `msg`.
Returns `0` if successful, an error code otherwise.
If the method is active for an extended period of time and no message is received, a communication error is assumed and a **Communication Failure** alarm is thrown.
* `int assert_mid(byte mid)`: Assert that a message with the same `mid` has been seen recently.
Returns `0` if the message hasn't been seen, `-1` otherwise.
Implementation-wise, its recommended that the file containing the function stores a queue of a predefined `N` number of `mid`s as a global variable, popping the oldest one when a new one is added.
* `int assert_payload(byte type, char* pid_arr, char* val_arr)`: Assert that all the `pid`s and `val`s received makes sense (depending on the `type`).
* `int raise_error(int err)`: Raises error `err`, allowing the program to respond to the issue appropriately.

--------------------------------------------------------------------------------
### Communication Steps
Communication can be initiated either by the coordinator or any of the managers.
The coordinator sends the managers control and configuration parameters, while the managers send measurements and active alarms flags.
Additionally, all components send responses to answer if a message was received correctly.
All these messages are structured in the same manner.

Communication simply involves two messages, one with the "payload", and a response, which can either be acknowledgment (ACK) or negative acknowledgment (NAK).
This leads to various scenarios:
* **Scenario 1**:
    * Actor 1 sends message.
    * Message is never received.
    * Scenario loops until message is received.
    * If message is not received after N attempts, a **Communication Failure** alarm is thrown.
```
Actor 1      Actor 2
   |            |
   +--- MSG --x |
   |            |
```

* **Scenario 2**:
    * Actor 1 sends message.
    * Message is received by Actor 2.
    * Actor 2 asserts message ID and sees that it was already properly received.
    * Actor 2 sends an ACK, telling Actor 1 that the message was received correctly.
```
Actor 1      Actor 2
   |            |
   +--- MSG --->|
   |            |
   |           MID
   |         INVALID
   |            |
   |<--- ACK ---+
   |            |
```

* **Scenario 3**:
    * Actor 1 sends message.
    * Message is received by Actor 2.
    * Actor 2 asserts message ID and sees that it hasn't been received.
    * As a final check, actor 2 asserts message payload and sees that it doesn't make sense.
    * Actor 2 sends a NAK, asking Actor 1 to send the message again.
    * Scenario loops until message is received.
    * If message comes up malformed every time after N attempts, a **Multiple Malformed Messages** alarm is thrown.
```
Actor 1      Actor 2
   |            |
   +--- MSG --->|
   |            |
   |           MID
   |          VALID
   |            |
   |         PAYLOAD
   |         INVALID
   |            |
   |<--- NAK ---+
   |            |
```

* **Scenario 4**:
    * Actor 1 sends message.
    * Message is received by Actor 2.
    * Actor 2 asserts message ID and sees that it hasn't been received.
    * Actor 2 asserts message payload and sees that it makes sense, marking the message ID as received and processing it accordingly.
    * Actor 2 sends a ACK.
    * Actor 1 receives the ACK, and the conversation ends.
```
Actor 1      Actor 2
   |            |
   +--- MSG --->|
   |            |
   |           MID
   |          VALID
   |            |
   |         PAYLOAD
   |          VALID
   |            |
   |<--- ACK ---+
   |            |
```

**NOTE**: If at any point Actor 2 sends an ACK or NAK and it is never received, then Actor 1 just sends the message again until it receives a response.
If the message hasn't been received properly, Actor 2 just processes the message again as if nothing has happened.
If it was already received and processed, the message ID has already been marked as read, so Actor 2 sends an ACK without processing the message, as is seen in Scenario 2.

--------------------------------------------------------------------------------
### State Machines
```
  ___________________________                ____________________________________
 /          SENDER           \              /              RECEIVER              \
|                             |            |                                      |
|                 open()      |            |                        open()        |
|                   |         |            |                          |           |
|     +-------------+         |            |  +---+-------------------+           |
|     |             |         |            |  |   |                   |           |
|    NAK &          v         |            |  |   |                   v           |
|   nfail < N     send()      |            |  | send_nak()          recv()        |
|     |             |         |            |  |   ^                   |           |
|     |             v         |            |  |   |                   v           |
|     +---------- recv()      |            |  |   +--failure----- assert_mid()    |
|     |             |         |            |  |   |                   |           |
|    NAK &         ACK        |            |  |   |                success        |
|   nfail >= N                |            |  |   |                   |           |
|     |                       |            |  |   |                   v           |
|     |                       |            |  |   +--failure-- assert_payload()   |
|     +------> raise_error()  |            |  |                       |           |
|                             |            |  |                    success        |
 \___________________________/             |  |                       |           |
                                           |  |                       v           |
                                           |  +------------------ send_ack()      |
                                           |                                      |
                                            \____________________________________/
```

--------------------------------------------------------------------------------
### Emergency States
* **Communication Failure**: A communication failure between two actors is detected.
This is communicated to the user via two methods (depending if the actor is the coordinator or a manager):
    * Coordinator: A message is printed on screen, and an alarm is thrown via the standard alarm methodology.
    * Manager: The communication error light is turned on in the ventilator itself.
* **Multiple Malformed Messages**: A continuous string of malformed messages are received, meaning that the sender is having some sort of issue.
This is communicated to the user with the same method as the communication failure emergency state.

--------------------------------------------------------------------------------
### `type`:`pid` Combinations
**TODO: Finish table with René**

| type |       pid | message/measurement/alarm/etc |
|-----:|----------:|-------------------------------|
| 000  |         - | ACK                           |
| 001  |         - | NAK                           |
| 010  |         - | REQ<sup>4</sup>               |
| 011  |         - | *unused*                      |
|      |           |                               |
| 100  | 0000 0000 | Net flow                      |
| 100  | 0000 0001 | Net volume                    |
| 100  | 0000 0010 | Tidal volume                  |
| 100  | 0000 0011 | Patient pressure              |
| 100  | 0000 0100 | presion_via_aerea             |
| 100  | 0000 0101 | PEEP                          |
| 100  | 0000 0110 | Respiratory frequency         |
| 100  | 0000 0111 | Maximum pressure              |
|      |           |                               |
| 101  | 0100 0000 |                               |
|      |           |                               |
| 110  | 1000 0000 |                               |
|      |           |                               |
| 111  | 1100 0000 |                               |

--------------------------------------------------------------------------------
### Footnotes
<sup>1</sup> The entire protocol was designed with C or a similar language in mind.

<sup>2</sup> Implementation-wise there might be a difference between the `struct` `msg` and the message sent through the socket.
This will be studied further as the protocol is implemented.

<sup>3</sup> This can probably be substituted completely by `pid`.
A future protocol could contemplate the possibility of eliminating this component, thus allowing payloads to contain measurements, messages, alarms, etc at the same time.

<sup>4</sup> The REQ type of message acts as a way to ask the receiving device the current state of one or more parameters by adding their `pid`s to the message.
The message assigns a `val` to these `pid`s, but it's just to keep consistency and doesn't have any meaning.

<sup>5</sup> More complex functions (like `send_req()` or `send_configparam()`) could be added in the future, but are left out for this version of the protocol to maintain simplicity.
