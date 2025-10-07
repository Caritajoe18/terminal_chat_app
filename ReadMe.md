# Terminal Chat Application in C

#### Video Demo: [https://youtu.be/ASZGoaYNF1c?si=viI_W-ZyUOByQfcF](https://youtu.be/ASZGoaYNF1c?si=viI_W-ZyUOByQfcF)

A **multi-client terminal-based chat application** written entirely in **C**.
It allows multiple users to communicate in real time over a network using simple terminal commands.
The system supports **nicknames**, **private messaging**, **chat logging**, and **colored text output** for better readability.

---

## Project Overview

This project demonstrates how to implement a **concurrent server–client model** in C using **sockets** and **POSIX threads**.
It simulates a small-scale chatroom, similar to IRC or terminal-based messaging systems, where several clients can join and exchange messages through a central server.

Each connected user is assigned a nickname, and all messages are transmitted through the server, which then relays them to the appropriate recipients.
The project also features **private messages**, **user management commands**, and **automatic message logging** for record keeping.

The application is designed for **local area networks (LAN)** but can be extended for **internet communication** with minor configuration.

---

## Features

### Real-Time Multi-Client Chat

Multiple users can connect simultaneously to the same server and exchange messages in real time. Each message is immediately broadcast to all connected clients.

### Custom Nicknames

Every user chooses a nickname upon joining, allowing personalized identification throughout the chat session.

### Public and Private Messaging

* **Public message**: Sent to everyone in the chatroom.
* **Private message**: Use `@username message` syntax to send a direct message to a specific user.

### ⚙️ Chat Commands

Built-in commands make the chat interactive and easy to manage:

* `/list` — displays all connected users.
* `/quit` — disconnects you from the server.
* `@username message` — sends a private message.

### 🧾 Chat Logging

All chat activities (joins, leaves, public and private messages) are logged automatically in a file named `chat_history.txt`.
This file is created if it doesn’t exist and appended to each time the server runs.

### 🎨 Color-Coded Messages

Color output improves readability:

* 🟩 **Green** — join notifications
* 🔵 **Blue** — public messages
* 🔴 **Red** — leave notifications
* 🟣 **Purple** — private messages

ANSI escape codes are used for cross-terminal compatibility.

---

## Project Files

| File               | Description                                                          |
| ------------------ | -------------------------------------------------------------------- |
| `server.c`         | Handles incoming client connections, message routing, and logging.   |
| `client.c`         | Connects users to the chat server and handles user input/output.     |
| `Makefile`         | Automates compilation and provides shortcuts to run or clean builds. |
| `chat_history.txt` | Auto-generated log file containing chat records.                     |
| `README.md`        | Project documentation (this file).                                   |

---

## ⚒️ How to Build and Run

### 1️⃣ Build with Makefile (recommended)

To compile both the server and client at once:

```bash
make
```

To start the server:

```bash
make run-server
```

To start a client (in another terminal window):

```bash
make run-client
```

To remove compiled binaries and start fresh:

```bash
make clean
```

---

### 2️⃣ Manual Compilation (without Makefile)

If you prefer compiling manually:

```bash
gcc server.c -pthread -o server
gcc client.c -pthread -o client
```

Then run them:

```bash
./server    # Start server
./client    # Connect as client
```

---

## Usage Examples

**Public message:**

```
Hello everyone!
```

**Private message:**

```
@Bob Hey, are you online?
```

**List connected users:**

```
/list
```

**Quit the chat:**

```
/quit
```

---

## How It Works

### Server Side

1. The server listens on a TCP port (default: `8080`).
2. When a client connects, the server creates a new thread for that connection.
3. Messages are received and either broadcast to all users or sent privately if they begin with `@username`.
4. The server logs all messages to `chat_history.txt`.

### Client Side

1. The client connects to the server via IP and port.
2. The user chooses a nickname.
3. Messages typed into the terminal are sent to the server.
4. Incoming messages are displayed in real time.

---

## 🌐 Connecting Across Computers (LAN or Internet)

### 🖧 On the Server Computer

1. Run the server:

   ```bash
   ./server
   ```
2. It will display:

   ```
   Server listening on port 8080...
   ```
3. Find your local IP address:

   ```bash
   ipconfig       # (on Windows)
   ifconfig       # (on macOS/Linux)
   ```

   Example: `192.168.0.105`

### On the Client Computer

1. Copy the compiled `client` executable to the client machine.
2. Connect to the server using its IP:

   ```bash
   ./client 192.168.0.105 8080
   ```
3. You’ll immediately join the shared chatroom.

> ⚠️ **If connection fails:**
>
> * Ensure both computers are on the same network.
> * Check that firewalls allow TCP traffic on port `8080`.
> * Use the server’s **local IPv4 address**, not `127.0.0.1`.

---

## 🧱 Technical Concepts Demonstrated

* **Socket programming** (`AF_INET`, `SOCK_STREAM`)
* **Multithreading** with `pthread_create`
* **Mutex synchronization** for thread safety
* **String handling and message parsing**
* **File I/O** for persistent chat logs
* **Signal handling** for graceful shutdown
* **ANSI terminal colors**

---

## 🧩 Possible Improvements

* Add user authentication (username + password).
* Add message timestamps.
* Encrypt private messages.
* Implement a graphical client using GTK or ncurses.
* Store chat history in a database instead of a text file.
* Support custom ports and configuration via CLI flags.

---

## 🧑‍💻 Author

**Carita Ndibe**
Software Developer passionate about building real-world systems in C, Python, and JavaScript.
This project was built as part of my exploration into **network programming** and **multithreaded system design**.

---

## 🏁 Summary

This project showcases how low-level socket programming can be used to implement a **fully functional, concurrent chat system**.
It combines fundamental computer science concepts — threads, synchronization, and I/O — into a practical and interactive application.
By running the server and client programs on different machines, users can communicate across networks through a simple yet powerful terminal interface.

