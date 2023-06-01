Create a trained model using [Algerian Forest Fires Dataset](https://archive.ics.uci.edu/ml/datasets/Algerian+Forest+Fires+Dataset++)

You can see how I trained the model at: [model_algeria.ipynb](../model_develop/model_algeria.ipynb)

I trained the model using different machine learning methods:
- Artificial Neural Network
- Random Forest
- Support Vector Machine with RBF kernel

And the result shown that ANN model is the best model for fire prediction by using recall and F2 metrics. The model then converted into [TF-Lite](../model_develop/model.tflite) and the scaler are saved as [scaler_min.txt](../model_develop/scaler_min.txt) and [scaler_scale.txt](../model_develop/scaler_scale.txt)
