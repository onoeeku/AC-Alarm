#include <TridentTD_LineNotify.h>

#define SSID        "xxxx"      // บรรทัดที่ 11 ให้ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
#define PASSWORD    "xxx"     // บรรทัดที่ 12 ใส่ รหัส Wifi
#define LINE_TOKEN  "xxxxxx"   // บรรทัดที่ 13 ใส่ รหัส TOKEN ที่ได้มาจากข้างบน

// AC Check
#define ADC_PIN A0
#define SAMPLING 300
#define VOFFSET  512
#define AMPLITUDE 411.00
#define REAL_VAC  233.5
int adc_max,adc_min;
int adc_vpp;


// switch setup
#define D1 5
#define D2 4 
#define button1 D1 // switch input Active Low
#define button2 D2 // switch input Active Low
#define pressed LOW
void setup() {
  
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  Serial.begin(115200); Serial.println();
  Serial.println(LINE.getVersion());

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);
}

void loop() {

   read_VAC();
    float V = map(adc_vpp, 0, AMPLITUDE, 0, REAL_VAC * 100.00) / 100.00;
  String data;
  data = "Volt = "+String(V,1)+(" VAC"); 
    // AC Alarm
  if(V >= 200) {
    LINE.notify ("ไฟฟ้า ติด");
    delay(500);
  }

   if(V <= 0) {
    LINE.notify ("ไฟฟ้า ดับ");
    delay(500);
  }

  // Switch Alarm
  bool ReadSwitch = digitalRead(button1);
  if(ReadSwitch == pressed)
    {
    LINE.notify ("อุณหภูมิ เกินกำหนด");
    delay(500);
  }  
  bool ReadSwitch1 = digitalRead(button2);
  if(ReadSwitch == pressed)
    {
    LINE.notify ("ความชื่น เกินกำหนด");
    delay(500);
  }  
}
void read_VAC() {
  int cnt;
  
  adc_max = 0;
  adc_min = 1024;
  
  for(cnt = 0; cnt < SAMPLING; cnt++) {
    int adc = analogRead(ADC_PIN);
    
    if(adc > adc_max) {
      adc_max = adc;
      }
  }
}
