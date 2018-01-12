# IotDiscovery

This repository contains a practical method for discovering IoT devices currently active in a network.

The system consists in sending a multicast message from the discoverer application (the one where we want to list all the devices). This message will be received by the IoT devices, that must be subscribed to the particular multicast address. Once a device receives the discovery message it connects to the discoverer application in a specific port and sends its info such as type of device and firmware version.

Note that the discoverer app must have an active TCP Server waiting for the response messages.

![Diagram of messages transferred between the application and the IoT devices](https://image.ibb.co/hDjVGm/discovery.png)

## Repo Structure

* discoverer: contains examples of the implementation of the discovery application (app in the image)
* iot_devices: contains examples of the implementation in the different IoT devices such as MCUs, Arduino, Raspberry,...