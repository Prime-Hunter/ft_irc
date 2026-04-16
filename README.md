This project has been created as part of the 42 curriculum by lde-guil & barnaud

# Internet Relay Chat — `ircserv`

## Description

This project implements a **minimal IRC server** in **C++98**. The goal is to understand TCP networking with non-blocking I/O and event-driven design, using a single `poll()` loop to manage many clients on one process.

The server accepts client connections, authenticates them with a shared password, and supports classic IRC features: user registration (`PASS`, `USER`, `NICK`), channels (`JOIN`, `PART`), messaging (`PRIVMSG`), channel administration (`KICK`, `INVITE`, `MODE`, `TOPIC`), and disconnection (`QUIT`), along with `CAP` for basic client compatibility.

**Overview:** The program listens on a TCP port, multiplexes read/write events with `poll()`, parses IRC-style commands, maintains clients and channels in memory, and sends numeric replies and broadcasts according to the expected IRC behavior.

## Instructions

### Prerequisites

- A C++ compiler with **C++98** support (such as `c++`, `g++` or `clang++`)
- A POSIX environment (Linux) with BSD sockets (`socket`, `bind`, `listen`, `accept`, `poll`, etc....)

### Compilation

From the repository root:

```bash
make
```

This will compile the executable **`ircserv`** in the root directory.

| Target    | Action                              |
|-----------|-------------------------------------|
| `make`    | Build `ircserv`                     |
| `clean`   | Remove object files (`obj/`)       |
| `fclean`  | Remove objects and `ircserv`        |
| `re`      | `fclean` then `make`               |

There is no separate install step; run the binary from the build directory or copy `ircserv` to a directory in your `PATH` if desired.

### Execution

```text
./ircserv <port> <password>
```

- **`<port>`** — integer from **1024** to **65535** (inclusive).
- **`<password>`** — non-empty string of printable ASCII characters (32–126). Clients must send this password with `PASS` before completing registration.

Example:

```bash
./ircserv 4444 irc42
```

Stop the server with **Ctrl+C** (SIGINT or SIGQUIT); file descriptors are closed cleanly.

### Connecting clients

**Netcat (quick manual tests):**

```bash
nc 127.0.0.1 4444
```

Then send IRC commands in order, e.g. `PASS`, `NICK`, `USER`, followed by channel and messaging commands.

**irssi:**

```text
/connect localhost 4444 <password>
```

Use the same port and password as when starting `ircserv`. (Note: if you are using localhost anr running multiple instances of irssi to test the project, make sure to set different nicknames for each instances, otherwise the server won't let you connect properly)

### Supported commands (summary)

`PASS`, `USER`, `NICK`, `JOIN`, `PART`, `PRIVMSG`, `KICK`, `INVITE`, `MODE`, `TOPIC`, `QUIT`.

## Additional features

This project also handles file transfer and a moderation bot to better ressemble a classic IRC server>

### File transfer

File transfer can be realised on the **Irssi client** using the `DCC` command. To send a file, you can use `DCC SEND <nick> <filename>`. The client receiving the file has to use the `DCC GET <filename>` to accept the transfer request.

### Moderation bot

A moderation bot is initialized when running the program. It will scan every message sent using `PRIVMSG` and will send a warning everytime a client uses inappropriate words. At the third warning, the bot will kick said user off the channel.

## Technical choices (brief)

- **Language:** C++98 to match the subject constraints and keep the codebase portable across campus environments.
- **I/O model:** `poll()` on the listening socket and all client sockets avoids one thread per client and keeps the design single-threaded while being non-blocking.
- **Layout:** core logic under `src/`, IRC command handlers under `cmd/`, declarations under `includes/` and bonus part under `bonus/`.

## Resources

### IRC and networking

- **[RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459)** — *Internet Relay Chat Protocol* (classic IRC reference; many servers and this project align with its spirit).
- **[RFC 2812](https://datatracker.ietf.org/doc/html/rfc2812)** — *Internet Relay Chat: Client Protocol* (updated client command semantics; useful for command names and behavior).
- **`man poll`**, **`man socket`**, **`man recv`**, **`man send`** — POSIX API used by the server.

### AI usage

| Area | How AI was used |
|------|------------------|
| **This `README.md`** | Structure, English wording, checklist alignment with the 42 guidelines|
| **Code clarification** | Reorganisation the organisation of some files while explicitly not rewriting the code|
| **Better research** | Help with the search of niche/hard to find datas, such as the IRC replies prefix or `DCC` messages format