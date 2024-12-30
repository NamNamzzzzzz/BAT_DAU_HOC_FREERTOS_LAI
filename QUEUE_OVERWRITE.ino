//Cai nay danh cho cam bien muc nuoc
#define PW_PIN 19
#define SS_PIN 15

QueueHandle_t my_queue;
typedef struct{
  int ID;
  float val;
}sensor;

void sensor1(void *pvParameters);
void consumer (void *pvParameters);

void setup() {
  Serial.begin(115200);
  pinMode(PW_PIN, OUTPUT);
  digitalWrite(PW_PIN, LOW);

  my_queue = xQueueCreate(1, sizeof(sensor));
  if(my_queue == NULL){
    Serial.println("Queue full or falure !!");
    while(1);
  }
  xTaskCreate(sensor1, "Water SS", 2048, NULL, 1, NULL);
  xTaskCreate(consumer, "Nhan Data", 2048, NULL, 1, NULL);

}

float readSensor() {
    digitalWrite(PW_PIN, HIGH);    
    delay(100);                 
    int val = analogRead(SS_PIN);        
    digitalWrite(PW_PIN, LOW);        
    return val;                 
}

void sensor1(void *pvParameters){
  sensor WTR;
  while(1){
    WTR.ID = 111;
    WTR.val = 9999;
    if(xQueueOverwrite(my_queue, &WTR)){
      Serial.print("WTR la: ");
      Serial.println(WTR.val);
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
        Serial.print("Data nhan duoc la: ");
        Serial.println(recvData.val);
      }
    vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(10));
}
