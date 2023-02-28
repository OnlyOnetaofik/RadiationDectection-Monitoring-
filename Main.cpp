#include <PCD8544.h>
#include <SPI.h>
#define integratingTime 15000  //Logging period in milliseconds
#define oneMinute 60000  //One minute
 
unsigned long counts = 0;     //variable for GM Tube events
unsigned long events = 0;
unsigned long cpm = 0;        //variable for CPM
unsigned int multiplier;  //variable for calculation CPM in this sketch
unsigned long previousMillis;  //variable for time measurement
double avgCounts = 0;
double avgCPM = 0;
double avgUSV = 0;
double sumCPM = 0;
double sumUSV = 0;
double uSv = 0;
double dose = 0;
 
const int ledPin =  9;
 
static PCD8544 lcd;
 
void tube_impulse() //subprocedure for capturing events from GM board
{      
  counts++;
  events++;
}
 
void setup() {
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);
  counts = 0;
  cpm = 0;
  multiplier = oneMinute / integratingTime;      //calculating multiplier, depend on your log period
  Serial.begin(9600);
  attachInterrupt(0, tube_impulse, FALLING); //define external interrupts
  pinMode(ledPin, OUTPUT);
  Serial.println("GM Tube - Send 's' to start...");
}
void loop()
{
withserialPort();
}
void withserialPort()
  {
             
  if (Serial.available() > 0) {
      lcd.setCursor(0, 4); //row 5
      lcd.print("Counts: ");
      lcd.print(events);  
   
    char cin = Serial.read();
    if(cin = 's'){
     while(Serial.read() != 'N') {
 
                 lcd.setCursor(0, 4); //row 5
                 lcd.print("Counts: ");
                 lcd.print(events);  
               
                unsigned long currentMillis = millis();
               
                if(currentMillis - previousMillis > integratingTime){
                 
                  avgCounts++;
                  previousMillis = currentMillis;
                  cpm = counts * multiplier; //cpm * 60/15 = cpm * 4
 
                  uSv =( cpm  / 151.0);
               
                  sumCPM = (double)cpm + sumCPM;
                  sumUSV = uSv + sumUSV;
             
                  avgCPM = sumCPM / avgCounts;
                  avgUSV = sumUSV / avgCounts;                

                //print cpm on serial
                Serial.print("Time: ");
                Serial.print((currentMillis)/1000);
                Serial.print(" | ");
                Serial.print("\t");
               
                Serial.print("CPM: ");
                Serial.print(cpm);
                Serial.print(" | ");
                Serial.print("\t");

                Serial.print("AvgCPM: ");
                Serial.print(avgCPM);

                Serial.print(" | ");
                Serial.print("\t");

                Serial.print("uSv/h: ");
                Serial.print(uSv);
                Serial.print(" | ");
                Serial.print("\t");

                Serial.print("Avg_uSv/h: ");
                Serial.print(avgUSV);
                Serial.print(" | ");
                Serial.print("\t");
                Serial.print("\n"); 

                 //print on LCD
                 lcd.setCursor(0, 0); //row 1
                 lcd.print("S CPM   uSv/hr"); //first row: information
                 lcd.setCursor(0, 1); //row 2
                 lcd.print("N ");
                 lcd.print(cpm);
                 lcd.print("    ");
                 lcd.print(uSv,4);
                 
                 lcd.setCursor(0, 2); //row 3
                 lcd.print("A ");
                 lcd.print(avgCPM,1);
                 lcd.print("  ");
                 lcd.print(avgUSV,4);
             
                                 
                 counts = 0;
              //-------------------------------------
