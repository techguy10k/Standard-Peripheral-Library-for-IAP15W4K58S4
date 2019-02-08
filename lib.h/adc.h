#ifndef ADC_H_

#define ADC_H_

#ifndef set_bit
#define set_bit 1
#endif

#ifndef reset_bit 
#define reset_bit 0
#endif


#define adc_accuracy_8bit 8
#define adc_accuracy_10bit 10

#define adc_speed_veryfast 3
#define adc_speed_fast 2
#define adc_speed_slow 1
#define adc_speed_veryslow 0

typedef struct{
                          unsigned char cal;
                          unsigned char channel;
                          unsigned char accuracy;
                          unsigned char speed;
                          unsigned long vcc;
               }adc_cmdtypedef;

unsigned int adc_on(adc_cmdtypedef* adc_cmd);


#endif

