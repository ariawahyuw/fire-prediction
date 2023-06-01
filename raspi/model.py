import tflite_runtime.interpreter as tflite
import numpy as np
import sys

x = sys.argv[1]
data = str.split(x, ',')
interpreter = tflite.Interpreter(model_path="../model_develop/model.tflite")
scale_ = np.loadtxt("../model_develop/scaler_scale.txt")
min_ = np.loadtxt("../model_develop/scaler_min.txt")

if abs(float(data[0]) - 0.) < 1e-2:
    print(-1)
else:
    soilHumid = np.interp(float(data[4]), [0, 80], [65.9, 0.7])
    x_test = np.array([data[0], data[1], data[3], data[2], soilHumid], dtype=np.float32)
    x_test *= scale_
    x_test += min_
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    interpreter.allocate_tensors()
    interpreter.set_tensor(input_details[0]['index'], x_test.reshape(-1,5))
    interpreter.invoke()

    y_test = interpreter.get_tensor(output_details[0]['index'])
    y_test = np.squeeze(y_test)
    print(int(y_test >= 0.5))
