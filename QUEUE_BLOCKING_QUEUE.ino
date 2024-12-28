#define BUTTON 15
QueueHandle_t my_queue;

void producer_Button(void *pvParameters);
void consumer_Blocking(void *pvParameters);

void setup() {
  Serial.begin(115200);
  my_queue = xQueueCreate(5, sizeof(int));
  if(my_queue == NULL){
    Serial.println("Queue bi loi!!");
    while(1);
  }
  pinMode(BUTTON, INPUT_PULLUP);
  xTaskCreate(producer_Button, "Ham gui", 2048, NULL, 1, NULL);
  xTaskCreate(consumer_Blocking, "Ham nhan", 2048, NULL, 1, NULL);
}

void producer_Button(void *pvParameters){
  int count =0;
  bool last_state = HIGH;
  while(1){
    bool cur_state = digitalRead(BUTTON);
    if(cur_state == LOW && last_state == HIGH){
      count++;
      if(xQueueSend(my_queue, &count, 0) ==pdTRUE){
        Serial.print("So lan nhan nut la: ");
        Serial.println(count);
      }
      else Serial.println("Queue full or error!!");
    }
    last_state = cur_state;
    vTaskDelay(100/ portTICK_PERIOD_MS);
  }
}

void consumer_Blocking(void *pvParameters){
  int value =0;
  while(1){
    if(xQueueReceive(my_queue, &value, portMAX_DELAY) == pdTRUE){
      Serial.print("Nhan thong tin la: ");
      Serial.println(value);
    }
  }
}

void loop() {
  vTaskDelay(10/ portTICK_PERIOD_MS);
}
