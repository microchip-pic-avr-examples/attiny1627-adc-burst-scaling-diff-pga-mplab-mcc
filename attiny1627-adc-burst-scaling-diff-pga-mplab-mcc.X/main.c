/*
Copyright (c) [2012-2020] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/

/*
 *
 * How To Use the 12-Bit Differential ADC in Burst Accumulation Mode:
 * Scaling with Differential mode and PGA
 *
 */

#include "mcc_generated_files/system/system.h"

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>

#define TIMEBASE_VALUE              ((uint8_t) ceil(F_CPU*0.000001))
#define ADC_DIFF_MAX_VALUE          (((1 << 12) / 2) - 1)                   /* In differential mode, the max value is 2047 */
#define ADC_DIFF_MAX_VALUE_16BIT    ((uint32_t) ADC_DIFF_MAX_VALUE << 4)    /* In differential mode, the max value for a 16-bit result is 32752 */

/* Defines to easily configure ADC accumulation */
#define ADC_SAMPNUM_CONFIG  ADC_SAMPNUM_ACC256_gc
/* Left shifting (1 << SAMPNUM) results in the number of accumulated samples */
#define ADC_SAMPLES         (1 << ADC_SAMPNUM_CONFIG)

/* Volatile variables to improve debug experience */
static volatile int32_t adc_reading;
static volatile float voltage;
static volatile float current;

int main(void)
{
    SYSTEM_Initialize();

    while(1)
	{
		ADC0.COMMAND |= ADC_START_IMMEDIATE_gc;
		while(!(ADC0.INTFLAGS & ADC_RESRDY_bm)); /* Wait until conversion is done */

		adc_reading = ADC0.RESULT; /* Read 16 bit scaled or left adjusted result */

		/* Calculate the differential voltage, VREF = 1.024V, 16-bit resolution, 16x gain. */
		voltage = (float)((adc_reading * 1.024) / ADC_DIFF_MAX_VALUE_16BIT) / 16;
		//current = voltage / 5;   /* Uncomment this line if measuring across a 5 ohm resistor in series with the power supply */

		_delay_ms(500);
	} 
}