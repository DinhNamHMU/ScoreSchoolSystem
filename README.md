# **SCORE SCHOOL SYSTEM**  
**Programming Assignment for the Course "4G-LTE Network Protocol Development"**  
by Viettel High Technology Industries Corporation  

---

## **1. Student Information**
- **Full Name**: Nguyễn Đình Nam  
- **Student ID**: B15DCVT262  
- **Class**: D16CQVT06-B  
- **Institution**: Posts and Telecommunications Institute of Technology  

---

## **2. Project Description**

### **2.1. Introduction**
This project involves implementing a **TCP User Client** and a **TCP User Server** system with the following components:

- **TCP User Server**:  
  - Acts as a server, storing and managing client information (`client_id`, `client_password`, `client_score`, etc.).  
  - Handles requests from clients and provides appropriate responses for access results and scores.  

- **TCP User Client 1 (Client_1)**:  
  - Sends login and score requests for `Client_1` to the server.  

- **TCP User Client 2 (Client_2)**:  
  - Sends login and score requests for `Client_2` to the server.  

---

### **2.2. Process Flow**

#### **TCP User Client 1 (C1) and TCP User Server (S):**

1. **C1** sends a `LOG_IN_REQUEST` message to **S**  
   (_processed on `thread_1`_).  
2. **S** responds to **C1** with a `LOG_IN_SUCCESS` message.  

3. **C1** sends a `SCORE_REQUEST` message to **S**.  
4. **S** responds to **C1** with a `SCORE_SUCCESS` message.  

5. **C1** sends a `LOG_OUT_REQUEST` message to **S**.  
6. **S** responds to **C1** with a `LOG_OUT_SUCCESS` message.  

7. **C1** sends another `SCORE_REQUEST` message to **S** (after logout).  
8. **S** responds to **C1** with a `SCORE_FAILURE` message.  

#### **TCP User Client 2 (C2) and TCP User Server (S):**

1. **C2** sends a `LOG_IN_REQUEST` message to **S**  
   (_processed on `thread_2`_).  
2. **S** responds to **C2** with a `LOG_IN_SUCCESS` message.  

---

### **2.3. Notes**
- The project must be implemented in **C** using the **TCP library** on the Linux operating system.  
- The `LOG_IN_REQUEST` message must include at least the `client_id` and `client_password`.  
- The **TCP User Server** must use separate threads:  
  - `thread_1` for `Client_1`.  
  - `thread_2` for `Client_2`.  

---

## **3. How to Run the Project**

1. **Clone the Repository**:  
   ```bash
   git clone https://github.com/your-username/score-school-system.git
   cd score-school-system
