QueueHandle_t my_queue; 

void producer(void *pvParameters);
void consumer1(void *pvParameters);
void consumer2(void *pvParameters);

void setup() {
  Serial.begin(115200);

  my_queue = xQueueCreate(5, sizeof(char[20]));
  if (my_queue == NULL) {
    Serial.println("Failed to create Queue!");
    while (1);
  }

  xTaskCreate(producer, "Producer", 2048, NULL, 2, NULL);
  xTaskCreate(consumer1, "Consumer1", 2048, NULL, 1, NULL);
  xTaskCreate(consumer2, "Consumer2", 2048, NULL, 1, NULL);
}

void producer(void *pvParameters) {
  char message[20];
  int count = 0;

  while (1) {
    sprintf(message, "Message %d", count++);
    if (xQueueSend(my_queue, &message, pdMS_TO_TICKS(1000)) == pdPASS) {
      Serial.print("Producer sent: ");
      Serial.println(message);
    } else {
      Serial.println("Queue Full!");
    }
    vTaskDelay(pdMS_TO_TICKS(500)); 
  }
}

void consumer1(void *pvParameters) {
  char receivedMessage[20];
  while (1) {
    if (xQueueReceive(my_queue, &receivedMessage, portMAX_DELAY) == pdTRUE) {
      Serial.print("Consumer1 received: ");
      Serial.println(receivedMessage);
    }
  }
}

void consumer2(void *pvParameters) {
  char receivedMessage[20];
  while (1) {
    if (xQueueReceive(my_queue, &receivedMessage, portMAX_DELAY) == pdTRUE) {
      Serial.print("Consumer2 received: ");
      Serial.println(receivedMessage);
    }
  }
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(10));
}
