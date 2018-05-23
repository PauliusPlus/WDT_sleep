
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>


const byte LED = 13;
volatile int sleep_cycles_remaining;



ISR(WDT_vect)                   // watchdog interrupt
  {
      digitalWrite (LED, HIGH);  // LED on
      sleep_cycles_remaining--;
  }



void configure_wdt(void)
{
 
  cli();                                           // disable interrupts for changing the registers
  // reset WDT and clear various flags
  MCUSR = 0;     
  // allow changes
  WDTCSR = bit (WDCE) | bit (WDE);
  // set interrupt mode and an interval 
  WDTCSR = bit (WDIE) | bit (WDE) | bit (WDP2) | bit (WDP1) | bit (WDP0);    // set WDIE, and 8 seconds delay
  sei();                                            // re-enable interrupts 
  wdt_reset();
  Serial.println(F(" WDT = 1s "));
  delay(5);

}  //configure_wdt


void wait_serial_end(void)
{
          // empty output buffer
        Serial.flush ();
        // wait for transmit buffer to empty
        while ((UCSR0A & _BV (TXC0)) == 0){}

        digitalWrite (LED, LOW);
}  //wait_serial_end




void sleep(int ncycles)                              // Put the Arduino to deep sleep.
{  
  sleep_cycles_remaining = ncycles;                  // defines how many cycles should sleep

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);                // Set sleep to full power down.
 
  power_adc_disable();                                // Turn off the ADC while asleep.
 
  while (sleep_cycles_remaining > 0)                  // while some cycles left, sleep!
    {
      configure_wdt();

      Serial.print(F("sleep_cycles_remaining = "));
      Serial.println(sleep_cycles_remaining);
      wait_serial_end();

      sleep_mode();                                   // now goes to Sleep and waits for the interrupt
      
      ///////ZZZZzzzzzzzzzz.....
      
      sleep_disable();                                // When awake, disable sleep mode
    }
 
  power_all_enable();                                 // put everything on again
  wdt_reset();                                        // !!! after wake up WDT is in System Reset Mode
  
} //sleep


void setup () {
    pinMode (LED, OUTPUT);
digitalWrite (LED, LOW);
   Serial.begin(115200);
   Serial.println();
   Serial.println();
  Serial.println(F("RF24/examples/GettingStarted_HandlingData"));
  
} //setup



void loop()
{

 sleep(5);

 while(1);
  
}  //loop

