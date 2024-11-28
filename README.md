Programming Assignment for the Course "4G-LTE Network Protocol Development" by Viettel High Technology Industries Corporation
SCORE SCHOOL SYSTEM
1. Student Information:
Full Name: Nguyễn Đình Nam
Student ID: B15DCVT262
Class: D16CQVT06-B
Institution: Posts and Telecommunications Institute of Technology
2. Assignment Description:
2.1. Introduction:
This mini-project involves developing a TCP User Client and a TCP User Server with three processes:

TCP User Server:
Acts as the main server, maintaining client information such as client_id, client_password, and client_score. It processes client requests and responds with either access confirmation or the requested scores.

TCP User Client 1 (Client_1):
Represents a TCP user client that sends login requests and score queries to the server.

TCP User Client 2 (Client_2):
Another TCP user client with similar functionality, sending login requests and score queries to the server.

2.2. Process Flow:
TCP User Client 1 (C1) and TCP User Server (S):

C1 sends a LOG_IN_REQUEST message to S (processed on thread_1).

S responds to C1 with a LOG_IN_SUCCESS message.

C1 sends a SCORE_REQUEST message to S.

S responds to C1 with a SCORE_SUCCESS message.

C1 sends a LOG_OUT_REQUEST message to S.

S responds to C1 with a LOG_OUT_SUCCESS message.

C1 sends a SCORE_REQUEST message to S (after logging out).

S responds to C1 with a SCORE_FAILURE message.

TCP User Client 2 (C2) and TCP User Server (S):

C2 sends a LOG_IN_REQUEST message to S (processed on thread_2).
S responds to C2 with a LOG_IN_SUCCESS message.
2.3. Notes:
The project must be implemented in C using the TCP library on the Linux operating system.
The LOG_IN_REQUEST message must include at least the client_id and client_password.
The TCP User Server should use thread_1 for Client_1 and thread_2 for Client_2.
