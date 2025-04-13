void filtro(Filter2 *F, volatile float in, volatile float *out)
{
  int a = 0.8;
  *out = a*F->out_1 + (1-a)*in;
   F->out_1 = *out;
}

void filtro2(Filter3 *F, volatile float in, volatile float *out)
{
  int a = 0.4;
  int c = 0.6;
  F->l = a * in + (1 - a) * (F->l + F->b) ;
  *out = F->l;
  F->b =  c * (F->l - F->l_1) + (1 - c) * F->b ;
  F->l_1 = F->l;
}

typedef struct {
  float out_1;
  } Filter2;

typedef struct {
  float l;
  float b;
  float l_1;
  } Filter3;
