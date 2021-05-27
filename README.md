# CS307-Assignment-4
**Note:** We have build the programs locally because my current PC does not support virtual box as the processor is Apple Silicon M1. But all the functionalities work fine and without any error.

**Q1. Design a file transfer application.**
**Running the code:**
1. Open Q1 folder.
2. In the Q1 folder open the terminal.
3. Now compile q1.c by using “gcc q1.c”
4. Now run the server on this terminal using “./a.out server”
5. Now run the client on another terminal in the same folder using “./a.out client”
6. Enter the address of file to be downloaded on client side.
7. Enter the name with which you want to save file which is to be downloaded on client side.
8. The file is downloaded and saved successfully on client side.

**Note:** If you want to run multiple clients you can run another "./a.out client" command in third download and download files simultaneously from both the clients. To implement this we have used fork() command.


**Q2. Error Control
Running the code:**
1. Open Q2 folder.
2. In the Q2 folder open the terminal.
3. Now compile q1.c by using “gcc q2.c”
4. Now run the server on this terminal using “./a.out server”
5. Now run the client on another terminal in the same folder using “./a.out client”
6. Enter the key for crc algorithm. on client side
7.  Now server will automatically generate a random string of '0' and '1' and will apply crc algorithm on it, then it will send that message to client and client will again apply crc on it using the key entered in step 6.
8. Client will tell whether there is error present in the message received from server or not.

**Note:** Q3 results are present in the report.
