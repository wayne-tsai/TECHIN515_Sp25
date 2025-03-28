# TECHIN515 Lab 4 - Magic Wand

This project implements a gesture recognition system using an ESP32 microcontroller with an MPU6050. The system consists of two main components: a gesture data capture system and a gesture inference system.

## Project Structure

```
.
├── gesture_capture/     # Tools for capturing and processing gesture data
│   ├── gesture_capture.ino    # Arduino sketch for data capture
│   ├── process_gesture_data.py # Python script for data processing
│   └── data/                  # Directory for storing gesture data
└── wand/      # Gesture recognition inference system
    └── wand.ino     # Arduino sketch for gesture recognition
```

## Hardware Requirements

- ESP32 development board
- MPU6050 sensor
- Jumper wires for connections
- Breadboard for quick prototyping
- Battery and enclosure for creating your wand

### Hardware Connections

Connect the MPU6050 to your ESP32:
- VCC → 3.3V
- GND → GND
- SCL → GPIO22 (or your I2C clock pin)
- SDA → GPIO21 (or your I2C data pin)

**Note**: Pin numbers may vary depending on your development board.

## Software Requirements

### Arduino IDE
- Arduino IDE with ESP32 board support
- Required libraries:
  - Adafruit MPU6050
  - Adafruit Sensor
  - Wire (built-in)
  - Edge Impulse SDK (for inference)

### Python (for data capture)
- Python 3.8 or newer
- Required packages (install via `pip install -r requirements.txt`)

## Getting Started

### 1. Data Collection

1. Navigate to the `gesture_capture` directory
2. Upload `gesture_capture.ino` to your ESP32
3. Run the Python script to collect gesture data:
   ```bash
   python process_gesture_data.py --gesture "W" --person "your_name"
   ```
   Here "W" represents the gesture. Name it appropriately based on the gesture being recorded.
4. Follow the on-screen instructions to capture gestures

### 2. Gesture Recognition

1. Navigate to the `wand_inference` directory
2. Upload `wand_inference.ino` to your ESP32
3. Open the Serial Monitor at 115200 baud
4. Send 'o' to start gesture capture
5. The system will automatically recognize and classify the gesture

## Data Format

The gesture data is stored in CSV format with three columns:
- x: X-axis acceleration (m/s²)
- y: Y-axis acceleration (m/s²)
- z: Z-axis acceleration (m/s²)

Each capture contains approximately 100 samples (1 second at 100Hz).

## Wand Inference System

The wand inference system uses Edge Impulse for real-time gesture recognition. Here's how it works:

### Features
- Real-time gesture recognition at 100Hz sampling rate
- 1-second gesture capture window
- Automatic gesture classification
- Confidence score for each prediction

### How It Works
1. The system continuously monitors the MPU6050 accelerometer
2. When triggered (by sending 'o' via Serial), it captures 1 second of accelerometer data
3. The captured data is processed using the Edge Impulse model
4. The system outputs the predicted gesture and confidence score

### Configuration
The inference system is configured with:
- Sampling rate: 100Hz (10ms between samples)
- Capture duration: 1 second
- Accelerometer range: ±8g
- Gyroscope range: ±500°/s
- Filter bandwidth: 21Hz

### Output Format
The system outputs predictions in the following format:
```
Prediction: [gesture_name] (confidence_percentage%)
```

## Troubleshooting

- **Port not found**: Specify the port manually with `--port COM3` (Windows) or `--port /dev/ttyUSB0` (Linux/Mac)
- **Permission denied**: On Linux/Mac, run `sudo chmod 666 /dev/ttyUSB0` (replace with your port)
- **No data received**: Check connections and ensure the sketch is uploaded correctly
