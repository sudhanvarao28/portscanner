# portscanner

- A multi-threaded portscanner that scans through all the ports and specifies which is open.

###  Server:

- This server is just used to open a port to show the functionality of portscanner
- To use the server executable use the following syntax :
    - .\server.exe \<IP> \<Port>
    - This will spin up a server on the specified IP and port
- Example usage:
    -  .\server.exe 127.0.0.1 60


### Portscanner:
- The port scanner can be used to look through all the specified ports and tell us which is open.
- To use the portscanner executable use the following syntax:
    - .\portscanner \<IP> \<Start Port> \<End Port> \<Number of threads>  
    - This scans the start port to the end port and specify which port is open for a tcp connection.
- Example usage:  .\portscanner.exe 127.0.0.1 1 1000 50
    
    - This will scan ports from 1 to 1000 using 50 threads on 127.0.0.1
