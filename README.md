<p align="center">
  <h3 align="center">🚴 bike [WIP]</h3>
  </p>
</p>

The goal of this project is to capture, save, and display RPM derived data from my cyclace excercise bike. This data can be used to calculate speed and distance througout the duration of excercise, which provides interesting information about the intensity of the excercise!


This all started when I noticed that the onboard display received some sort of signal input in a standard 3.5mm mono headphone jack. I enjoy collecting data about my life, particularly excercise, so figured it would be easy enough to build a device to capture this data which is typically "thrown away" by the onboard display after each workout. 

Note: sadly this cannot be done by treating the jack as a 3.5mm headphone microphone input to a phone/laptop because the pinouts and circuits do not line up as needed. Decided to move ahead with microcontrollers since I have plenty handy!

<img src="https://user-images.githubusercontent.com/6299853/73600034-e0f93880-44ff-11ea-8a96-a8d9ed46c009.gif" />

## Current Progress
- [x] Determine bike sensor type (after some futzing, figured out that it's a [magnetic reed switch](https://en.wikipedia.org/wiki/Reed_switch))
- [x] Build initial circuit and microcontroller firmware to capture data, validate that rotations show up properly
- [ ] Write firmware to capture data from a workout
- [ ] Display current stats (distance, speed, duration) on a OLED display on device.
- [ ] Dump the data as CSVs to a microSD card (easy, standard, and secure)?
- [ ] Push data to a server (maybe on a local network)? Bluetooth serial to an app?
- [ ] ... more to come. 

## Hardware
The circuit needed here is pretty simple.

<img src="circuits/circuit.jpg" width=300px />
