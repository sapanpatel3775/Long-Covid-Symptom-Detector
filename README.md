# Long-Covid-Symptom-Detector
C++:
This is an device that utilizes an arduino microcontroller to detect the presence of Long-Covid Symptoms such as heart palpitations, low blood oxygen levels, and muscle fatigue.
Long-Haul COVID is defined as having reoccuring symptoms of COVID-19 after 4 weeks of being infected. This device detects whether a person is experiencing heart palpitations, low blood oxygen levels,or muscle fatigue.
If 1 or more of these symptoms are being experienced, then the user will be notified and advised to visit their doctor.

In this device, I've used a Maxim integrated MAX30102 pulse and heartrate sensor. This device was used to create a pulse-oximeter functionality. The heart rate and the Sp02 levels are calculated and displayed on an OLED panel.
If the user's Sp02 level is 5% lower than the baseline they have on file, then they will be notified.

The second componenet used was an electrocardiogram. It utilizes an AD8232 integrating block to clarify the electric biofeedbacksignal being outputted.
To detect heart palpiations, I've found the peaks of the graph, and calculated the distance between them. If it is too short to too long, then the user will be notified.

The third component of this device is an electromyography(emg) component. To create this, I've used a myoware muscle sensor. This part measures the electric biofeedback signal of a muscle on a scale from 0-1000.
The muscle fatigue is detected muscle usage when flexing is less than 40%. When this is calculated, the user is notified.

Finally, is designed as a waist band, and it is supposed to be worn for about a week. If 1 or more of these symptoms are detected, then the user will be notified and will be advised to visit a healthcare proffesional.
