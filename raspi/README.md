Run realtime prediction on Raspberry Pi using trained model.

To run the program, type:
```
python3 model.py "temperature, air_humid, wind_speed, water_level, soil_humidity"
```
For example:
```
python3 model.py "27.6, 60, 0, 0, 62"
```

Requirements:
- tflite-runtime>=2.11.0
- model.tflite (can be found at: [../model_develop/model.tflite](../model_develop/model.tflite))
- scaler_min.txt (can be found at: [../model_develop/scaler_min.txt](../model_develop/scaler_min.txt))
- scaler_scale.txt (can be found at: [../model_develop/scaler_scale.txt](../model_develop/scaler_scale.txt))
