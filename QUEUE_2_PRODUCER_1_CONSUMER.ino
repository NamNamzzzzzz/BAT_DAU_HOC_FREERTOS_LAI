// Kích thước mỗi phần tử queue -> 2x byte
#define SIZE_OF_CHAR 25
QueueHandle_t my_queue;

//Function prototype
void producer1(void *pvParameters);
void producer2(void *pvParameters);
void consumer(void *pvParameters);

void setup() {
  Serial.begin(115200);
  my_queue = xQueueCreate(5, SIZE_OF_CHAR);
  if(my_queue == NULL){
    Serial.println("That bai trong viec tao Queue!");
    while(1);
  }

  xTaskCreate(producer1, "Ham gui 1", 2048, NULL, 1, NULL);
  xTaskCreate(producer2, "Ham gui 2", 2048, NULL, 1, NULL);
  xTaskCreate(consumer, "Ham nhan", 2048, NULL, 1, NULL);
}

void producer1(void *pvParameters){
  while(1){
    char mess1 [SIZE_OF_CHAR];
    snprintf(mess1, sizeof(mess1), "Xin chao tu Producer1");
    if(xQueueSend(my_queue, &mess1, portMAX_DELAY) == pdTRUE){
      Serial.println("P1 da gui");
    }
    vTaskDelay(1000/ portTICK_PERIOD_MS);
  }
}

void producer2(void *pvParameters){
  while(1){
    char mess2 [SIZE_OF_CHAR];
    snprintf(mess2, sizeof(mess2), "Xin chao tu Producer2");
    if(xQueueSend(my_queue, &mess2, portMAX_DELAY) == pdTRUE){
      Serial.println("P2 da gui");
    }
    vTaskDelay(1500/ portTICK_PERIOD_MS);
  }
}

void consumer(void *pvParameters){
  char receive_mess[SIZE_OF_CHAR];
  while(1){
    if(xQueueReceive(my_queue, &receive_mess, pdMS_TO_TICKS(2000)) == pdTRUE){
      Serial.print("Da nhan duoc tin nhan la: ");
      Serial.println(receive_mess);
    }
    else Serial.println("Khong co data trong 2s");
    vTaskDelay(500/ portTICK_PERIOD_MS);
  }
}

void loop() {
  vTaskDelay(100/ portTICK_PERIOD_MS);
}
