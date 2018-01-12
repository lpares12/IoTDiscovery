Contains examples of the implementation of the discoverer in different systems

Current implementations:
* unix: The unix implementation sends a simple multicast message to the specified address:port and waits for incoming messages from the IoT devices by creating a tcp server and accepting new connections. Once a connection is made with a device, lists the device IP. Closes the TCP server after X seconds of sending the multicast message, to avoid waiting indefinitely. Requires Boost libraries.

Implementations yet to be done:
* Android