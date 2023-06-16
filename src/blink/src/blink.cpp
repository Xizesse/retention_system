    #include "/home/odroid/Desktop/gpio_test/wiringPi/wiringPi/wiringPi.h"
     
    int main(void)
    {
        wiringPiSetup();
     
        pinMode(0, OUTPUT);
        
        
        for (;;)
        {
            
            digitalWrite(0, HIGH); 
            
            delay(1000);
            digitalWrite(0, LOW); 
            delay(1000);
        }
        digitalWrite(0, LOW); 
        return 0;
       
    }


