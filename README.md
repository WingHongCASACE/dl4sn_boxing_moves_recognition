# dl4sn_boxing_moves_recognition
Using Arduino Nano33BLE and Edge Impulse for boxing movement classification

**`Stay tune as more information will be provided when the written report is submitted.`**

Most data was generated with the boxing glove on.
<p align="center"><img src="https://user-images.githubusercontent.com/91855312/159836941-1ff23025-5665-4e21-ac50-28e64baba6b7.jpg" 
alt="IMAGE ALT TEXT HERE" width=50% border="10" />
  </p>
  
While mainly the boxing glove was being used in this project, the MMA glove was served as one type of viration.  
  <p align="center"><img src="https://user-images.githubusercontent.com/91855312/159837691-3bfa05f5-a80d-41d3-85c5-b102774c318a.jpg" 
alt="IMAGE ALT TEXT HERE" width=50% border="10" />
  </p>

### The ML architecture using the optimal combination of hyperparameter

```py
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, InputLayer, Dropout, Conv1D, Conv2D, Flatten, Reshape, MaxPooling1D, MaxPooling2D, BatchNormalization, TimeDistributed
from tensorflow.keras.optimizers import Adam

# model architecture
model = Sequential()
model.add(Dense(20, activation='relu',
    activity_regularizer=tf.keras.regularizers.l1(0.00001)))
model.add(Dense(20, activation='relu',
    activity_regularizer=tf.keras.regularizers.l1(0.00001)))
model.add(Dense(10, activation='relu',
    activity_regularizer=tf.keras.regularizers.l1(0.00001)))
model.add(Dense(classes, activation='softmax', name='y_pred'))

# this controls the learning rate
opt = Adam(learning_rate=0.01, beta_1=0.9, beta_2=0.999)
# this controls the batch size, or you can manipulate the tf.data.Dataset objects yourself
BATCH_SIZE = 8
train_dataset = train_dataset.batch(BATCH_SIZE, drop_remainder=False)
validation_dataset = validation_dataset.batch(BATCH_SIZE, drop_remainder=False)
callbacks.append(BatchLoggerCallback(BATCH_SIZE, train_sample_count))

# train the neural network
model.compile(loss='categorical_crossentropy', optimizer=opt, metrics=['accuracy'])
model.fit(train_dataset, epochs=85, validation_data=validation_dataset, verbose=2, callbacks=callbacks)

# Use this flag to disable per-channel quantization for a model.
# This can reduce RAM usage for convolutional models, but may have
# an impact on accuracy.
disable_per_channel_quantization = False
```

I have created [a public Edge Impulse project version](https://studio.edgeimpulse.com/public/86666/latest/) to share the experimental data and impulse.

