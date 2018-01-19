** ESP8266 **

Modules:
* debug: used to blink a LED in GPIO2 to make sure ESP8266 hasn't crashed
* network_station: used to connect to the access point, taking configuration either from flash memory or from the definition in config.h.
* discovery_device: implementation of the IoTDiscovery. Joins the multicast group to receive the discovery messages and responds to this messages using TCP.

Note: the sdk 0.9.4 was used for compiling and some methods used may be deprecated with newer sdks. Also, a ESP-01 module with 1MB of flash memory was used.
