- Project Description

This project was brought to you by Dielli Doçi and Dionis Sylejmani, developed for a practical application involving client-server communication using the UDP protocol, implements a system where a server hosts files,
including a text file and a batch file, while clients can interact with the server to performspecific operations.
The project, designed using C++ on Windows, supports a role-based model with admin and client roles, where admin has additional privileges, such as writing to files and executing commands.
The primary functionalities include reading from and writing to a file hosted on the server and fetching and executing a batch file from the server on the client's machine.

- Features:
 
  Role-Based Access Control:
Admin role requires a password, with permissions for reading, writing, and executing files.
Client role has restricted access, limited to reading files.
  Hidden Password Input:
When logging in as admin, the password input is hidden for security.
  Read Command:
Allows the client to request and view the contents of a text file hosted on the server.
  Write Command:
Enables admin users to append text to the server-hosted text file.
  Execute Command:
Sends a request to the server to fetch and execute a batch file, which the client saves locally and runs.
  File Handling:
Ensures server-side management of text and batch files for demonstration and testing.
  Client-Side Execution of Server Files:
Clients can fetch a batch file from the server, save it locally, and execute it on their own machine.

- Configuration Requirements:

No special configurations are required beyond having a C++ compiler and a Windows OS environment. The system makes use of Winsock2 for UDP communication. 
The server and client programs can be compiled using standard C++ compilation commands, and the conio.h library is utilized for hidden password input functionality.

- Expected Results:

The expected result of this project is a fully functional client-server system that:

Responds to client requests for reading and writing data to/from a server-hosted file.
Allows an admin user to securely log in with a password and perform privileged actions.
Sends a batch file from the server to the client for local execution.
Provides seamless communication using the UDP protocol, demonstrating a robust interaction model with role-based access control and secure password management.
