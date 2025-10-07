# Terminal Chat Application in C
#### Video Demo: https://youtu.be/ASZGoaYNF1c?si=viI_W-ZyUOByQfcF

A **multi-client terminal-based chat application** written in C.  
Supports **nicknames, private messages, commands, chat logging, and color-coded messages**.

---

## Features

- Multiple clients can connect to the server simultaneously.  
- Each client chooses a **nickname** at connection.  
- **Broadcast messages** to all connected clients.  
- **Private messages** using `@username message`.  
- **Commands**:
  - `/list` – display all connected users.  
  - `/quit` – leave the chat.  
- **Chat logs** are saved automatically in `chat_history.txt`.  
- **Color-coded messages** for readability:
  - Green: user join notifications  
  - Blue: messages  
  - Red: user leave notifications  

---

## Files

- `server.c` – the server program.  
- `client.c` – the client program.  
- `chat_history.txt` – auto-generated log of all messages.  
- `Makefile` – compile and run server/client easily.  
- `README.md` – project documentation.  

---

## How to Build and Run

### 1. Using the Makefile (recommended)

- Build both server and client:

```bash
make
````

* Run the server:

```bash
make run-server
```

* Run a client (in a separate terminal):

```bash
make run-client
```

* Clean compiled binaries:

```bash
make clean
```

---

### 2. Using GCC directly

If you prefer compiling manually:

```bash
gcc server.c -pthread -o server
gcc client.c -pthread -o client
```

Run the programs:

```bash
./server   # start server
./client   # start client
```

---

## Usage Examples

* **Send a public message:**

```
Hello everyone!
```

* **Send a private message:**

```
@Bob Hey, are you free today?
```

* **List all connected users:**

```
/list
```

* **Quit the chat:**

```
/quit
```

---

## Chat Log

All messages are saved to `chat_history.txt` in plain text. Example:

```
Alice has joined the chat!
Bob has joined the chat!
Alice: Hello everyone!
Bob: Hi Alice!
[PM from Alice]: Hey Bob, are you free today?
Bob has left the chat.
```

---

## Notes

* Nicknames are temporary and exist only for the current server session.
* Open multiple client terminals to test the chat system.
* Colors may not display properly on some terminals (depends on ANSI color support).

