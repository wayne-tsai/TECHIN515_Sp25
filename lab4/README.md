# TECHIN515 Lab 4 - Magic Wand

This project implements a gesture recognition system using an ESP32 microcontroller with an MPU6050. The system consists of two main components: a gesture data capture system and a gesture inference system.

## Learning Objectives
By completing this lab, students will:
- Understand the fundamentals of gesture recognition using IMU sensors
- Learn to collect and preprocess sensor data
- Gain experience with Edge Impulse's DSP and ML capabilities
- Develop practical skills in embedded systems programming
- Learn to optimize ML models for resource-constrained devices
- Understand the importance of proper hardware integration and user interaction

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

## Tasks

### Part 1: Data Collection
1. Set up the hardware and collect data:
   - Connect the MPU6050 to your ESP32. Note: How MPU6050 is placed on the board. 
   - Use the provided `gesture_capture.ino` sketch and `process_gesture_data.py` to record gesture data. Your data should cover all gestures shown in the spell cards.
   - For each gesture, collect at least 20 samples. Ensure good quality data by performing the gesture consistently.
   - Save all data in the `data` directory with clear naming conventions.

**Note**: You are allowed to share your recorded data with others. However, you need to collect your own data.


### Part 2: Edge Impulse Model Development
1. Create a new project in Edge Impulse:
   - Upload your collected gesture data
   - Split the data into training and testing sets (80/20 split)

2. Design and implement the model:
   - Explore different DSP blocks:
     - Try different window sizes
     - Experiment with various preprocessing techniques
     - Document your choices and reasoning
   - Implement a neural network classifier:
     - Start with a simple architecture
     - Experiment with different layer configurations
     - Use dropout to prevent overfitting
   - Train and evaluate your model:
     - Monitor training metrics
     - Analyze confusion matrix
     - Optimize for performance vs. accuracy

3. Model optimization:
   - Use Edge Impulse's model testing features
   - Optimize the model size for ESP32 deployment
   - Document your optimization process

### Part 3: ESP32 Implementation
1. Deploy the model to your ESP32 and use `wand.ino` to test its performance with real-time gestures. **Note**: You need to rename header file (Line 18) accordingly.
2. Repeat your testing for a few times, and use appropriate metrics to document your wand's performance.
3. Modify the provided `wand.ino` code: Implement a button-triggered inference: ESP32 predicts the gesture once the button is pressed.
4. Test your code to ensure your wand functions. 
5. Create a demo video showing your implementation.

### Part 4: Battery and Enclosure 
1. Power your ESP32 using a battery rather than USB cable.
2. Use appropriate enclosure for your wand. 

## Deliverables
1. Code and Documentation:
   - All Arduino sketches with comments
   - Python scripts for data collection
   - Your dataset
   - Edge Impulse project export
   - README with setup instructions

2. Report (PDF format) including:
   - Pictures of hardware setup and connections
   - Data collection process and results
   - Edge Impulse model architecture and optimization
   - Performance analysis and metrics
   - Demo video link
   - Challenges faced and solutions

3. Demo Video (2-3 minutes):
   - Show hardware setup
   - Demonstrate data collection
   - Show real-time gesture recognition
   - Include performance metrics

4. Enclosure and Battery are due on **May 20th, 2025**.
  
## Troubleshooting

- **Port not found**: Specify the port manually with `--port COM3` (Windows) or `--port /dev/ttyUSB0` (Linux/Mac)
- **Permission denied**: On Linux/Mac, run `sudo chmod 666 /dev/ttyUSB0` (replace with your port)
- **No data received**: Check connections and ensure the sketch is uploaded correctly
