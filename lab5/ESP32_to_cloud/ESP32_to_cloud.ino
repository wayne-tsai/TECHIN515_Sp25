#include <wayne-tsai-project-1_inferencing.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#define BUTTON_PIN 5
#define RED_LED_PIN 3
#define GREEN_LED_PIN 3
#define BLUE_LED_PIN 3
#define CONFIDENCE_THRESHOLD 80.0

// WiFi credentials
const char* ssid = "myLab";
const char* password = "********";
const char* serverUrl = "http://192.168.1.100:5000/predict";

Adafruit_MPU6050 mpu;

#define SAMPLE_RATE_MS 10
#define CAPTURE_DURATION_MS 1000
#define FEATURE_SIZE EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE

bool capturing = false;
unsigned long last_sample_time = 0;
unsigned long capture_start_time = 0;
int sample_count = 0;
float features[FEATURE_SIZE];

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, LOW);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");

    Serial.println("Initializing MPU6050...");
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) delay(10);
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("MPU6050 initialized successfully");
    Serial.println("Send 'o' or press button to start gesture capture");
}

void capture_accelerometer_data() {
    if (millis() - last_sample_time >= SAMPLE_RATE_MS) {
        last_sample_time = millis();
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        if (sample_count < FEATURE_SIZE / 3) {
            int idx = sample_count * 3;
            features[idx] = a.acceleration.x;
            features[idx + 1] = a.acceleration.y;
            features[idx + 2] = a.acceleration.z;
            sample_count++;
        }

        if (millis() - capture_start_time >= CAPTURE_DURATION_MS) {
            capturing = false;
            Serial.println("Capture complete");
            run_inference();
        }
    }
}

void run_inference() {
    ei_impulse_result_t result = { 0 };
    signal_t features_signal;
    features_signal.total_length = FEATURE_SIZE;
    features_signal.get_data = &raw_feature_get_data;

    if (sample_count * 3 < FEATURE_SIZE) {
        Serial.println("ERROR: Not enough data for inference. Default to 'o'");
        set_led_by_prediction("o");
        return;
    }

    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);
    if (res != EI_IMPULSE_OK) {
        Serial.print("ERR: Failed to run classifier (");
        Serial.print(res);
        Serial.println(")");
        return;
    }

    print_inference_result(result);
}

void sendRawDataToServer() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected!");
        return;
    }

    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Build JSON array from features[]
    // Reshape the data to match the model's expected input shape (100, 3)
    DynamicJsonDocument doc(2048);  // Increased size to accommodate larger payload
    JsonArray data = doc.createNestedArray("data");
    
    // Create a 2D array of shape (100, 3)
    for (int i = 0; i < 100; i++) {
        JsonArray row = data.createNestedArray();
        for (int j = 0; j < 3; j++) {
            int idx = i * 3 + j;
            if (idx < FEATURE_SIZE) {
                row.add(features[idx]);
            } else {
                row.add(0.0);  // Pad with zeros if needed
            }
        }
    }
    
    String jsonPayload;
    serializeJson(doc, jsonPayload);

    int httpResponseCode = http.POST(jsonPayload);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Server response: " + response);

        // Parse the JSON response
        DynamicJsonDocument responseDoc(256);
        DeserializationError error = deserializeJson(responseDoc, response);
        if (!error) {
            const char* prediction = responseDoc["prediction"];
            float confidence = responseDoc["confidence"] * 100;  // Convert to percentage

            Serial.println("Server Inference Result:");
            Serial.print("Prediction: ");
            Serial.println(prediction);
            Serial.print("Confidence: ");
            Serial.print(confidence);
            Serial.println("%");
            
            set_led_by_prediction(prediction);
        } else {
            Serial.print("Failed to parse server response: ");
            Serial.println(error.c_str());
        }
    } else {
        Serial.printf("Error sending POST: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
}

void print_inference_result(ei_impulse_result_t result) {
    float max_value = 0;
    int max_index = -1;

    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > max_value) {
            max_value = result.classification[i].value;
            max_index = i;
        }
    }

    if (max_index != -1) {
        const char* prediction = ei_classifier_inferencing_categories[max_index];
        float confidence = max_value * 100;
        
        Serial.print("Prediction: ");
        Serial.print(prediction);
        Serial.print(" (");
        Serial.print(confidence);
        Serial.println("%)");

        if (confidence < CONFIDENCE_THRESHOLD) {
            Serial.println("Low confidence - sending raw data to server...");
            sendRawDataToServer();
        } else {
            set_led_by_prediction(prediction);
        }
    }
}

void set_led_by_prediction(const char* label) {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, LOW);

    if (strcmp(label, "z") == 0) {
        digitalWrite(RED_LED_PIN, HIGH);
    } else if (strcmp(label, "o") == 0) {
        digitalWrite(GREEN_LED_PIN, HIGH);
    } else if (strcmp(label, "v") == 0) {
        digitalWrite(BLUE_LED_PIN, HIGH);
    }
}

void loop() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        if (cmd == 'o') {
            Serial.println("Starting gesture capture...");
            sample_count = 0;
            capturing = true;
            capture_start_time = millis();
            last_sample_time = millis();
        }
    }

    if (digitalRead(BUTTON_PIN) == LOW && !capturing) {
        delay(50);
        if (digitalRead(BUTTON_PIN) == LOW) {
            Serial.println("Button pressed - Starting gesture capture...");
            sample_count = 0;
            capturing = true;
            capture_start_time = millis();
            last_sample_time = millis();
        }
    }

    if (capturing) {
        capture_accelerometer_data();
    }
}