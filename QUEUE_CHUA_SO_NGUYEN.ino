#define PW_PIN 5
#define SS_PIN 15
QueueHandle_t my_queue;

void send_sensor(void *pvParameters); 
void receive(void *pvParameters);

void setup() {
  Serial.begin(115200);
  pinMode(PW_PIN, OUTPUT);
  digitalWrite(PW_PIN, LOW);

  my_queue = xQueueCreate(3, sizeof(int));
  if(my_queue == NULL){
    Serial.println("Queue bi loi !!");
    while(1);
  }

  xTaskCreate(send_sensor, "Gui data tu SS", 2048, NULL, 1, NULL);
  xTaskCreate(receive, "Doc gia tri", 2048, NULL, 1, NULL);
}

int readSensor() {
    digitalWrite(PW_PIN, HIGH);    
    delay(100);                 
    int val = analogRead(SS_PIN);        
    digitalWrite(PW_PIN, LOW);        
    return val;                 
}

void send_sensor(void *pvParameters){
  //int value =readSensor();
  while(1){
    int value =readSensor();
    if(xQueueSend(my_queue, &value, pdMS_TO_TICKS(1000)) == pdTRUE){
      Serial.print("Gia tri cam bien la: ");
      Serial.println(value);
    }
    else Serial.println("Khong gui duoc");
    vTaskDelay(300/ portTICK_PERIOD_MS);
  }
}

void receive(void *pvParameters){
  int receive = 0;
  while(1){
    if(xQueueReceive(my_queue, &receive, portMAX_DELAY) == pdTRUE){
      Serial.print("Nhan duoc: ");
      Serial.println(receive);
      if(receive > 1500) Serial.println("Gia tri qua cao !!!");
    }
    else Serial.println("Ko nhan duoc gi!!");
  }
}

void loop() {
  vTaskDelay(10/ portTICK_PERIOD_MS);
}
