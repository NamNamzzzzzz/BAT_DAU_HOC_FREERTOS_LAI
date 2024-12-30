//Cai nay danh cho cam bien muc nuoc
#define PW_PIN 19
#define SS_PIN 15

//Cai nay danh cho HC-SR04 
#define trig 5
#define echo 18
#define SOUND_SPEED 0.034

//Cac bien danh cho HC-SR04
int duration;
float distance_cm;
float distance_inc;

QueueHandle_t my_queue;
typedef struct {
  int ID;
  float value;
} sensor;

sensor WTR;
sensor H04;

void sensor1(void *pvParameters);
void sensor2(void *pvParameters); 
void consumer (void *pvParameters);

void setup() {
  Serial.begin(115200);
  pinMode(PW_PIN, OUTPUT);
  digitalWrite(PW_PIN, LOW);
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 

  my_queue = xQueueCreate(5, sizeof(sensor));
  if(my_queue == NULL){
    Serial.println("Queue full or falure !!");
    while(1);
  }
  xTaskCreate(sensor1, "Water SS", 2048, NULL, 1, NULL);
  xTaskCreate(sensor2, "HC-SR04", 4096, NULL, 1, NULL);
  xTaskCreate(consumer, "Nhan Data", 2048, NULL, 1, NULL);
}

float HC_SR04(){
  duration = pulseIn(echo, HIGH);
  distance_cm = duration * SOUND_SPEED/2.0;
  delay(100);
  return distance_cm;
}

float readSensor() {
    digitalWrite(PW_PIN, HIGH);    
    delay(100);                 
    int val = analogRead(SS_PIN);        
    digitalWrite(PW_PIN, LOW);        
    return val;                 
}

void sensor1(void *pvParameters){
  while(1){
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  H04.ID = 111;
  H04.value = HC_SR04();
  if(xQueueSend(my_queue, &H04, pdMS_TO_TICKS(1000)) == pdTRUE){
    //Serial.print("Khoang cach la: ");
    //Serial.println(H04.value);
  }
  else Serial.println("Khong in duoc !!");
  vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void sensor2(void *pvParameters){
  while(1){
    WTR.ID = 222;
    WTR.value = readSensor();
    if(xQueueSend(my_queue, &WTR, pdMS_TO_TICKS(1000)) == pdTRUE){
      //Serial.print("WTR la: ");
      //Serial.println(WTR.value);
    }
    else Serial.println("Khong in duoc !!!");
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void consumer(void *pvParameters){
  sensor recvData;
  while(1){
    if(xQueueReceive(my_queue, &recvData, portMAX_DELAY) == pdTRUE){
      if(recvData.ID == 111){
        Serial.print("HC_04 la: ");
        Serial.println(recvData.value);
      }

      else if(recvData.ID == 222){
        Serial.print("WTR la: ");
        Serial.println(recvData.value);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(10));
}
