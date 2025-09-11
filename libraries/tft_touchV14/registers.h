//			Last Update: 13/08/15

#ifndef _REGISTERS_H_
#define _REGISTERS_H_

#define SET(x,y)		(x|=(1<<y))
#define CLR(x,y)		(x&=(~(1<<y)))
#define CHK(x,y)	(x & (1<<y))
#define TOG(x,y)	(x^=(1<<y))

typedef struct{ unsigned char b0:1;
	unsigned char b1:1;
	unsigned char b2:1;
	unsigned char b3:1;
	unsigned char b4:1;
	unsigned char b5:1;
	unsigned char b6:1;
unsigned char b7:1; } bits;

//UDR0
#define _UDR0 UDR0

//UBBR0
#define _UBBR0 UBBR0

//UCSR0C
#define _UCSR0C (* (volatile bits *) &UCSR0C)
#define _UMSEL01	_UCSR0C.b7
#define _UMSEL00	_UCSR0C.b6
#define _UPM01			_UCSR0C.b5
#define _UPM00			_UCSR0C.b4
#define _USBS0			_UCSR0C.b3
#define _UCSZ01		_UCSR0C.b2
#define _UCSZ00		_UCSR0C.b1
#define _UCPOL0		_UCSR0C.b0

//UCSR0B
#define _UCSR0B (* (volatile bits *) &UCSR0B)
#define _RXCIE0		_UCSR0B.b7
#define _TXCIE0		_UCSR0B.b6
#define _UDRIE0		_UCSR0B.b5
#define _RXEN0			_UCSR0B.b4
#define _TXEN0			_UCSR0B.b3
#define _UCSZ02		_UCSR0B.b2
#define _RXB80			_UCSR0B.b1
#define _TXB80			_UCSR0B.b0

//UCSR0A
#define _UCSR0A (* (volatile bits *) &UCSR0A)
#define _RXC0		_UCSR0A.b7
#define _TXC0		_UCSR0A.b6
#define _UDRE0		_UCSR0A.b5
#define _FE0			_UCSR0A.b4
#define _DOR0		_UCSR0A.b3
#define _UPE0		_UCSR0A.b2
#define _U2X0		_UCSR0A.b1
#define _MPCM0	_UCSR0A.b0

//TWAMR
#define _TWAMR	TWAMR

//TWCR
#define _TWCR (* (volatile bits *) &TWCR)
#define _TWINT		_TWCR.b7
#define _TWEA		_TWCR.b6
#define _TWSTA	_TWCR.b5
#define _TWSTO	_TWCR.b4
#define _TWWC		_TWCR.b3
#define _TWEN		_TWCR.b2
#define _TWIE		_TWCR.b0

//TWDR
#define _TWDR	TWDR

//TWAR
#define _TWAR (* (volatile bits *) &TWAR)
#define _TWGCE _TWAR.b0

//TWSR
#define _TWSR (* (volatile bits *) &TWSR)
#define _TWPS1 _TWSR.b1
#define _TWPS0 _TWSR.b0

//TWBR
#define _TWBR TWBR

//ASSR
#define _ASSR (* (volatile bits *) &ASSR)
#define _EXCLK		_ASSR.b6
#define _AS2				_ASSR.b5
#define _TCN2UB		_ASSR.b4
#define _OCR2AUB	_ASSR.b3
#define _OCR2BUB	_ASSR.b2
#define _TCR2AUB	_ASSR.b1
#define _TCR2BUB	_ASSR.b0

//OCR2B
#define _OCR2B OCR2B

//OCR2A
#define _OCR2A OCR2A

//TCNT2
#define _TCNT2 TCNT2

//TCCR2B
#define _TCCR2B (* (volatile bits *) &TCCR2B)
#define _FOC2A		_TCCR2B.b7
#define _FOC2B			_TCCR2B.b6
#define _WGM22		_TCCR2B.b3
#define _CS22			_TCCR2B.b2
#define _CS21			_TCCR2B.b1
#define _CS20			_TCCR2B.b0

//TCCR2A
#define _TCCR2A (* (volatile bits *) &TCCR2A)
#define _COM2A1		_TCCR2A.b7
#define _COM2A0		_TCCR2A.b6
#define _COM2B1		_TCCR2A.b5
#define _COM2B0		_TCCR2A.b4
#define _WGM21		_TCCR2A.b1
#define _WGM20		_TCCR2A.b0

#define _OCR1BH	OCR1BH
#define _OCR1BL	OCR1BL
#define _OCR1AH	OCR1AH
#define _OCR1AL	OCR1AL
#define _ICR1H		ICR1H
#define _ICR1L		ICR1L
#define _TCNT1H	TCNT1H
#define _TCNT1L	TCNT1L

#define _TCCR1C (* (volatile bits *) &TCCR1C)
#define _FOC1A	_TCCR1C.b7
#define _FOC1B		_TCCR1C.b6

#define _TCCR1B (* (volatile bits *) &TCCR1B)
#define _ICNC1		_TCCR1B.b7
#define _ICES1		 _TCCR1B.b6
#define _WGM13	_TCCR1B.b4
#define _WGM12	_TCCR1B.b3
#define _CS12		_TCCR1B.b2
#define _CS11		_TCCR1B.b1
#define _CS10		_TCCR1B.b0

#define _TCCR1A (* (volatile bits *) &TCCR1A)
#define _COM1A1		_TCCR1A.b7
#define _COM1A0		 _TCCR1A.b6
#define _COM1B1		_TCCR1A.b5
#define _COM1B0		_TCCR1A.b4
#define _WGM11		_TCCR1A.b1
#define _WGM10		_TCCR1A.b0


//OCR0B
#define _OCR0B OCR0B

//OCR0A
#define _OCR0A OCR0A

//TCNT0
#define _TCNT0 TCNT0

//TCCR0B
#define _TCCR0B (* (volatile bits *) &TCCR0B)
#define _FOC0A		_TCCR2B.b7
#define _FOC0B			_TCCR2B.b6
#define _WGM02		_TCCR2B.b3
#define _CS02			_TCCR2B.b2
#define _CS01			_TCCR2B.b1
#define _CS00			_TCCR2B.b0

//TCCR0A
#define _TCCR0A (* (volatile bits *) &TCCR0A)
#define _COM0A1		_TCCR0A.b7
#define _COM0A0		_TCCR0A.b6
#define _COM0B1		_TCCR0A.b5
#define _COM0B0		_TCCR0A.b4
#define _WGM01		_TCCR0A.b1
#define _WGM00		_TCCR0A.b0



#define _DIDR1 (* (volatile bits *) &DIDR1)
#define _AIN1D		_DIDR1.b1
#define _AIN0D		_DIDR1.b0

#define _DIDR0	DIDR0

//ADMUX
#define _ADMUX (* (volatile bits *) &ADMUX)
#define _REFS1		_ADMUX.b7
#define _REFS0		_ADMUX.b6
#define _ADLAR	_ADMUX.b5
#define _MUX3		_ADMUX.b3
#define _MUX2		_ADMUX.b2
#define _MUX1		_ADMUX.b1
#define _MUX0		_ADMUX.b0

//ADCSRB
#define _ADCSRB (* (volatile bits *) &ADCSRB)
#define _ACME		_ADCSRB.b6
#define _ADTS2		_ADCSRB.b2
#define _ADTS1		_ADCSRB.b1
#define _ADTS0		_ADCSRB.b0

//ADCSRA
#define _ADCSRA (* (volatile bits *) &ADCSRA)
#define _ADEN		_ADCSRA.b7
#define _ADSC		_ADCSRA.b6
#define _ADATE		_ADCSRA.b5
#define _ADIF		_ADCSRA.b4
#define _ADIE		_ADCSRA.b3
#define _ADPS2		_ADCSRA.b2
#define _ADPS1		_ADCSRA.b1
#define _ADPS0		_ADCSRA.b0

//ADC
#define _ADC		ADC

//TIMSK2
#define _TIMSK2 (* (volatile bits *) &TIMSK2)
#define _OCIE2B	_TIMSK2.b2
#define _OCIE2A	_TIMSK2.b1
#define _TOIE2		_TIMSK2.b0

//TIMSK1
#define _TIMSK1 (* (volatile bits *) &TIMSK1)
#define _ICIE1		_TIMSK1.b5
#define _OCIE1B	_TIMSK1.b2
#define _OCIE1A	_TIMSK1.b1
#define _TOIE1		_TIMSK1.b0

//TIMSK0
#define _TIMSK0 (* (volatile bits *) &TIMSK0)
#define _OCIE0B	_TIMSK0.b2
#define _OCIE0A	_TIMSK0.b1
#define _TOIE0		_TIMSK0.b0

#define _PCMSK2 (* (volatile bits *) &PCMSK2)
#define _PCINT23	_PCMSK2.b7
#define _PCINT22	_PCMSK2.b6
#define _PCINT21	_PCMSK2.b5
#define _PCINT20	_PCMSK2.b4
#define _PCINT19	_PCMSK2.b3
#define _PCINT18	_PCMSK2.b2
#define _PCINT17	_PCMSK2.b1
#define _PCINT16	_PCMSK2.b0

#define _PCMSK1 (* (volatile bits *) &PCMSK1)
#define _PCINT14	_PCMSK1.b6
#define _PCINT13	_PCMSK1.b5
#define _PCINT12	_PCMSK1.b4
#define _PCINT11	_PCMSK1.b3
#define _PCINT10	_PCMSK1.b2
#define _PCINT09	_PCMSK1.b1
#define _PCINT08	_PCMSK1.b0

#define _PCMSK0 (* (volatile bits *) &PCMSK0)
#define _PCINT07	_PCMSK0.b7
#define _PCINT06	_PCMSK0.b6
#define _PCINT05	_PCMSK0.b5
#define _PCINT04	_PCMSK0.b4
#define _PCINT03	_PCMSK0.b3
#define _PCINT02	_PCMSK0.b2
#define _PCINT01	_PCMSK0.b1
#define _PCINT00	_PCMSK0.b0

//EICRA
#define _EICRA (* (volatile bits *) &EICRA)
#define _ISC11		_EICRA.b3
#define _ISC10		_EICRA.b2
#define _ISC01		_EICRA.b1
#define _ISC00		_EICRA.b0

//PCICR
#define _PCICR (* (volatile bits *) &PCICR)
#define _PCIE2		_PCICR.b2
#define _PCIE1		_PCICR.b1
#define _PCIE0		_PCICR.b0

//OSCCAL
#define _OSCCAL OSCCAL

//PRR
#define _PRR (* (volatile bits *) &PRR)
#define _PRTWI			_PRR.b7
#define _PRTIM2		_PRR.b6
#define _PRTIM0		_PRR.b5
#define _PRTIM1		_PRR.b3
#define _PRSPI			_PRR.b2
#define _PRUSART0	_PRR.b1
#define _PRADC		_PRR.b0

//CLKPR
#define _CLKPR (* (volatile bits *) &CLKPR)
#define _CLKPS3		_CLKPR.b3
#define _CLKPS2		_CLKPR.b2
#define _CLKPS1		_CLKPR.b1
#define _CLKPS0		_CLKPR.b0

//WDTCSR
#define _WDTCSR (* (volatile bits *) &WDTCSR)
#define _WDIF		_WDTCSR.b7
#define _WDIE		_WDTCSR.b6
#define _WDP3		_WDTCSR.b5
#define _WDCE		_WDTCSR.b4
#define _WDE		_WDTCSR.b3
#define _WDP2		_WDTCSR.b2
#define _WDP1		_WDTCSR.b1
#define _WDP0		_WDTCSR.b0

//SREG
#define _SREG (* (volatile bits *) &SREG)
#define _I		_SREG.b7
#define _T		_SREG.b6
#define _H		_SREG.b5
#define _S		_SREG.b4
#define _V		_SREG.b3
#define _N		_SREG.b2
#define _Z		_SREG.b1
#define _C		_SREG.b0

//SPH
#define _SPH	SPH

//SPL
#define _SPL	SPL



//SPCR
#define _SPCR (* (volatile bits *) &SPCR)
#define _SPIE			_SPCR.b7
#define _SPE			_SPCR.b6
#define _DORD		_SPCR.b5
#define _MSTR		_SPCR.b4
#define _CPOL		_SPCR.b3
#define _CPHA		_SPCR.b2
#define _SPR1		_SPCR.b1
#define _SPR0		_SPCR.b0

//SPSR
#define _SPSR (* (volatile bits *) &SPSR)
#define _SPIF			_SPSR.b7
#define _WCOL		_SPSR.b6
#define _SPI2X		_SPSR.b0

//SPMCSR
#define _SPMCSR (* (volatile bits *) &SPMCSR)
#define _SPMIE				_SPMCSR.b7
#define _RWWSB			_SPMCSR.b6
#define _RWWSRE		_SPMCSR.b4
#define _BLBSET			_SPMCSR.b3
#define _PGWRT			_SPMCSR.b2
#define _PGERS				_SPMCSR.b1
#define _SELFPRGEN	_SPMCSR.b0

//MCUCR
#define _MCUCR (* (volatile bits *) &MCUCR)
#define _BODS			_MCUCR.b6
#define _BODSE		_MCUCR.b5
#define _PUD				_MCUCR.b4
#define _IVSEL			_MCUCR.b1
#define _IVCE			_MCUCR.b0

//MCUSR
#define _MCUSR (* (volatile bits *) &MCUSR)
#define _WDRF		_MCUSR.b3
#define _BORF		_MCUSR.b2
#define _EXTRF		_MCUSR.b1
#define _PORF		_MCUSR.b0

//SMCR
#define _SMCR (* (volatile bits *) &SMCR)
#define _SM2		_SMCR.b3
#define _SM1		_SMCR.b2
#define _SM0		_SMCR.b1
#define _SE		_SMCR.b0

//ACSR
#define _ACSR (* (volatile bits *) &ACSR)
#define _ACD			_ACSR.b7
#define _ACBG		_ACSR.b6
#define _ACO			_ACSR.b5
#define _ACI			_ACSR.b4
#define _ACIE		_ACSR.b3
#define _ACIC		_ACSR.b2
#define _ACIS1		_ACSR.b1
#define _ACIS0		_ACSR.b0

//GPIOR2	
#define _GPIOR2	GPIOR2
//GPIOR1
#define _GPIOR1	GPIOR1
//GPIOR0
#define _GPIOR0	GPIOR0

//GTCCR
#define _GTCCR (* (volatile bits *) &GTCCR)
#define _TSM					_GTCCR.b7
#define _PSRASY			_GTCCR.b1
#define _PSRSYNC		_GTCCR.b0

//PCIFR
#define _PCIFR (* (volatile bits *) &PCIFR)
#define _PCIF2		_PCIFR.b2
#define _PCIF1		_PCIFR.b1
#define _PCIF0		_PCIFR.b0

//TIFR2
#define _TIFR2 (* (volatile bits *) &TIFR2)
#define _OCF2B		_TIFR2.b2
#define _OCF2A		_TIFR2.b1
#define _TOV2		_TIFR2.b0

//TIFR1
#define _TIFR1 (* (volatile bits *) &TIFR1)
#define _ICF1			_TIFR1.b5
#define _OCF1B		_TIFR1.b2
#define _OCF1A	_TIFR1.b1
#define _TOV1		_TIFR1.b0

//TIFR0
#define _TIFR0 (* (volatile bits *) &TIFR0)
#define _OCF0B		_TIFR0.b2
#define _OCF0A	_TIFR0.b1
#define _TOV0		_TIFR0.b0

//EEARH
#define _EEARH		EEARH

//EEARL
#define _EEARL		EEARL

//EEDR
#define _EEDR		EEDR

//EECR
#define _EECR (* (volatile bits *) &EECR)
#define _EEPM1		_EECR.b5
#define _EEPM0		_EECR.b4
#define _EERIE		_EECR.b3
#define _EEMPE		_EECR.b2
#define _EEPE		_EECR.b1
#define _EERE		_EECR.b0

//EIFR
#define _EIFR (* (volatile bits *) &EIFR)
#define _INTF1		_EIFR.b1
#define _INTF0		_EIFR.b0

//EIMSK
#define _EIMSK (* (volatile bits *) &EIMSK)
#define _INT1		_EIMSK.b1
#define _INT0		_EIMSK.b0

#define POD PORTD
#define PID PIND
#define PDD DDRD

#define POC PORTC
#define PIC PINC
#define PDC DDRC

#define POB PORTB
#define PIB PINB
#define PDB DDRB

//PORTD
#define _POD (* (volatile bits *) &PORTD)
#define POD7		_POD.b7
#define POD6		_POD.b6
#define POD5		_POD.b5
#define POD4		_POD.b4
#define POD3		_POD.b3
#define POD2		_POD.b2
#define POD1		_POD.b1
#define POD0		_POD.b0

//DDRD
#define _PDD (* (volatile bits *) &DDRD)
#define PDD7		_PDD.b7
#define PDD6		_PDD.b6
#define PDD5		_PDD.b5
#define PDD4		_PDD.b4
#define PDD3		_PDD.b3
#define PDD2		_PDD.b2
#define PDD1		_PDD.b1
#define PDD0		_PDD.b0

//PIND
#define _PID (* (volatile bits *) &PIND)
#define PID7	_PID.b7
#define PID6	_PID.b6
#define PID5	_PID.b5
#define PID4	_PID.b4
#define PID3	_PID.b3
#define PID2	_PID.b2
#define PID1	_PID.b1
#define PID0	_PID.b0

//PORTC
#define _POC (* (volatile bits *) &PORTC)
#define POC6		_POC.b6
#define POC5		_POC.b5
#define POC4		_POC.b4
#define POC3		_POC.b3
#define POC2		_POC.b2
#define POC1		_POC.b1
#define POC0		_POC.b0

//DDRC
#define _PDC (* (volatile bits *) &DDRC)
#define PDC6	_PDC.b6
#define PDC5	_PDC.b5
#define PDC4	_PDC.b4
#define PDC3	_PDC.b3
#define PDC2	_PDC.b2
#define PDC1	_PDC.b1
#define PDC0	_PDC.b0

//PINC
#define _PIC (* (volatile bits *) &PINC)
#define PIC6	_PIC.b6
#define PIC5	_PIC.b5
#define PIC4	_PIC.b4
#define PIC3	_PIC.b3
#define PIC2	_PIC.b2
#define PIC1	_PIC.b1
#define PIC0	_PIC.b0

// PORTB
#define _POB (* (volatile bits *) &PORTB)
#define POB7 _POB.b7
#define POB6 _POB.b6
#define POB5 _POB.b5
#define POB4 _POB.b4
#define POB3 _POB.b3
#define POB2 _POB.b2
#define POB1 _POB.b1
#define POB0 _POB.b0

//DDRB
#define _PDB (* (volatile bits *) &DDRB)
#define PDB7		_PDB.b7
#define PDB6		_PDB.b6
#define PDB5		_PDB.b5
#define PDB4		_PDB.b4
#define PDB3		_PDB.b3
#define PDB2		_PDB.b2
#define PDB1		_PDB.b1
#define PDB0		_PDB.b0

//PINB
#define _PIB (* (volatile bits *) &PINB)
#define PIB7	_PIB.b7
#define PIB6	_PIB.b6
#define PIB5	_PIB.b5
#define PIB4	_PIB.b4
#define PIB3	_PIB.b3
#define PIB2	_PIB.b2
#define PIB1	_PIB.b1
#define PIB0	_PIB.b0

#endif
