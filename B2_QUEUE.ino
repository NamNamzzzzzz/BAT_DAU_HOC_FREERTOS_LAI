/*
 *   Queue Example 1
 *   - Task "producerTask": Gửi số đếm count (int) vào queue 
 *   - Task "consumerTask": Nhận số đếm từ queue, in ra Serial
 *
*/
QueueHandle_t queue1;

//Hàm nguyên mẫu 
void producer(void *pvParameters);
void consumer(void *pvParameters);

void setup() {
  Serial.begin(115200);
  queue1 = xQueueCreate(10, sizeof(int));
  if(queue1 == NULL){
    Serial.println("That bai trong viec tao Queue!");
    while(1);
  }

  //Tao 2 task
  xTaskCreate(producer, "Ham gui", 1024, NULL, 1, NULL);
  xTaskCreate(consumer, "Ham nhan", 1024, NULL, 1, NULL);
}

void producer(void *pvParameters){
  int count =0;
  while(1){
    if(xQueueSend(queue1, &count, portMAX_DELAY) == pdTRUE){
      Serial.print("Ham producer gui ");
      Serial.println(count);
      count++;
    }
    else Serial.println("Queue full or loi !!");
    vTaskDelay(1000/ portTICK_PERIOD_MS);
  }
}

void consumer(void *pvParameters){
  int receive_value =0;
  while(1){
    if(xQueueReceive(queue1, &receive_value,pdMS_TO_TICKS(1000)) == pdTRUE){
      Serial.print("Nhan duoc gia tri: ");
      Serial.println(receive_value);
    }
    else Serial.println("Khong nhan duoc data tu producer !!");
    vTaskDelay(1000/ portTICK_PERIOD_MS);
  }
}

void loop() {
  vTaskDelay(100/ portTICK_PERIOD_MS);
}
