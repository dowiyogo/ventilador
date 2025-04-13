#define PIN_SF1 A2
#define PIN_SF2 A0

float gain_Sf1 = 50.202;
float offset_Sf1 = -49.557;
float gain_Sf2 = 50.218;
float offset_Sf2 = -49.612;

volatile float sf1_adc;
volatile float sf2_adc;

void setup() {


  //  // configuraciones timer1 (control)
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for F increments
  OCR1A = CONTROL_100HZ;// = (16*10^6) / (F*8) - 1 (must be <65536) 1999 para 1 KHz
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bits for 8 prescaler
  TCCR1B |= (1 << CS12) ;
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  /* FAST SAMPLING
  //  // configuraciones timer2 (sampling)
  TCCR2A = 0;             // control timer/counter 2A a 0
  TCCR2B = 0;             // control timer/counter 2B a 0
  TCNT2  = 0;             // contador a 0
  OCR2A  = SAMPLING_250HZ;           // = frec.op/prescaler/frec.deseada = (16*10^6)/256/500-1 (prescaler 8 bits)
  TCCR2A |= (1 << WGM22); // modo CTC (Clear Timer at Compare)
  TCCR2B |= (1 << CS22) | ( 1 << CS21 );  // prescaler a 256
  TIMSK2 |= (1 << OCIE2A);// habilitado interrupcion por comparacion en timer2
  */

  //  // configuraciones timer3 (comunicacion)
  TCCR3A = 0;             // control timer/counter 3A a 0
  TCCR3B = 0;             // control timer/counter 3B a 0
  TCNT3  = 0;             // contador a 0
  OCR3A  = COMM_10HZ;           // = frec.op/prescaler/frec.deseada = (16*10^6)/1024/500-1 (prescaler 8 bits)
  TCCR3B |= (1 << WGM32); // modo CTC (Clear Timer at Compare)
  TCCR3B |= (1 << CS32) | ( 0 << CS21 ) | ( 1 << CS30 );  // prescaler a 1024
  TIMSK3 |= (1 << OCIE3A);// habilitado interrupcion por comparacion en timer3

  // SLOW SAMPLING  
  //  // configuraciones timer5 (sampling)
  TCCR5A = 0;             // control timer/counter 5A a 0
  TCCR5B = 0;             // control timer/counter 5B a 0
  TCNT5  = 0;             // contador a 0
  OCR5A  = SAMPLING_1KHZ;           // = frec.op/prescaler/frec.deseada = (16*10^6)/256/500-1 (prescaler 8 bits)
  TCCR5B |= (1 << WGM52); // modo CTC (Clear Timer at Compare)
  TCCR5B |= (1 << CS52) | ( 0 << CS51 ) | ( 0 << CS50 );  // prescaler a 256
  TIMSK5 |= (1 << OCIE5A);// habilitado interrupcion por comparacion en timer5  
  
  // Configuración ADC
  ADCSRA |= (0<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); // Prescaler ADC = 8 (153KHz)
  
  sei();//allow interrupts

  ///////////////////////////////////////////////////////





}



  float ADC_converter(int adc_val, float gain_v, float offset_v)
{
  float output_v = offset_v + gain_v * (adc_val * 5.0 / 1024.0);
  return output_v;
}


  
void loop()
{

  sf1_adc = ADC_converter((int)analogRead(PIN_SF1), gain_Sf1, offset_Sf1);
  sf2_adc = ADC_converter((int)analogRead(PIN_SF2), gain_Sf2, offset_Sf2);
}