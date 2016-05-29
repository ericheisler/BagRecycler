/*
 * Induction heater controller for Recyclebot
 * ATtiny85
 * by shlonkin
 * 
 * Procedure to avoid measuring while heater is on(for noise):
 * Turns off heater, waits 100ms, measures temp, turns on for switchInterval if needed
 * 
 * This code is in the public domain
 */

#define heaterPin 4
#define thermPin 3
#define switchInterval 1000

// A table for temperature values
// I realize this is completely unnecessary because there is only one
// value that matters, 180C = 777 = 3.8V(5V supply) = 3166ohm thermistor resistance
// But I will retain it for possible future use.
// these are the ADC values and the index is temperature-1 
// 1 to 200 degrees = 0 to 199 index
const uint16_t table[] PROGMEM = {20,21,21,22,23,24,25,26,27,28,29,30,
31,32,34,35,36,37,39,40,42,43,45,46,48,50,51,53,55,57,59,61,63,65,67,
70,72,74,77,79,82,85,87,90,93,96,99,102,105,108,111,115,118,122,125,
129,133,136,140,144,148,152,157,161,165,170,174,179,183,188,193,198,
203,208,213,218,223,228,234,239,244,250,256,261,267,273,278,284,290,
296,302,308,314,320,327,333,339,345,352,358,364,371,377,383,390,396,
403,409,415,422,428,435,441,447,454,460,467,473,479,485,492,498,504,
510,517,523,529,535,541,547,553,559,564,570,576,582,587,593,598,604,
609,615,620,625,631,636,641,646,651,656,661,666,670,675,680,684,689,
693,698,702,706,710,715,719,723,727,731,735,738,742,746,749,753,757,
760,763,767,770,773,777,780,783,786,789,792,795,798,801,803,806,809,
811,814,816,819,821,824,826,829,831};

void setup() {
  pinMode(heaterPin, OUTPUT);
  pinMode(thermPin, INPUT);
}

void loop() {
  //turn off
  digitalWrite(heaterPin, LOW);

  // wait 100ms for things to settle down
  delay(100);

  // measure
  int temp = getTemp();

  // turn on if needed
  if(temp < 180){
    digitalWrite(heaterPin, HIGH);
  }

  // let it go for switchInterval
  delay(switchInterval);
}

int getTemp(){
  // Average 8 reads
  int val = analogRead(thermPin);
  val += analogRead(thermPin);
  val += analogRead(thermPin);
  val += analogRead(thermPin);
  val += analogRead(thermPin);
  val += analogRead(thermPin);
  val += analogRead(thermPin);
  val += analogRead(thermPin);
  val /= 8;

  // look up the temperature in the table (temp is table index+1)
  if(val > pgm_read_word_near(table+199)){
    return 200;
  }
  if(val > 661){
    for(int i=150; i<200; i++){
      if(pgm_read_word_near(table+i) >= val){
        return i+1;
      }
    }
  }else if(val > 364){
    for(int i=100; i<200; i++){
      if(pgm_read_word_near(table+i) >= val){
        return i+1;
      }
    }
  }else if(val > 111){
    for(int i=50; i<200; i++){
      if(pgm_read_word_near(table+i) >= val){
        return i+1;
      }
    }
  }else{
    for(int i=0; i<200; i++){
      if(pgm_read_word_near(table+i) >= val){
        return i+1;
      }
    }
  }
}

