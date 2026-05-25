# IRC Commands Implementation

## Overview
This document describes the implementation of two new IRC commands: **LUSERS** and **NAMES**.

---

## LUSERS Command

### Purpose
The **LUSERS** command displays the total number of users currently connected to the IRC server.

### Syntax
```
LUSERS
```

### Example Usage
```
Client sends: LUSERS
Server replies: :localhost 251 <nick> :There are <count> users
```

### Implementation Details
- **File**: `cmd/LUSERS.cpp`
- **Method**: `Command::lusers()`
- **Functionality**:
  - Retrieves the server's client list via `_serv->getList()`
  - Counts the total number of connected clients
  - Sends a reply message with the format `251` (LUSERS reply code)
  - Response: `:localhost 251 <nickname> :There are <total> users\r\n`

### IRC Standard
According to RFC 1459, LUSERS returns:
- `251`: Reply with user count on the server

---

## NAMES Command

### Purpose
The **NAMES** command lists all users currently connected to one or more channels.

### Syntax
```
NAMES [channel]
```
- If no channel is specified, lists users in all channels
- If a channel is specified, lists only users in that channel

### Example Usage
```
Client sends: NAMES #general
Server replies: 
  :localhost 353 <nick> = #general :<user1> @<user2> +<user3>
  :localhost 366 <nick> #general :End of NAMES list

Client sends: NAMES
Server replies:
  :localhost 353 <nick> = #general :<user1> @<user2> +<user3>
  :localhost 353 <nick> = #other :<user4> <user5>
  :localhost 366 <nick> * :End of NAMES list
```

### Implementation Details
- **File**: `cmd/NAMES.cpp`
- **Method**: `Command::names()`
- **Functionality**:
  - **No arguments**: Lists users in all channels
    - Iterates through all channels via `_serv->getChannels()`
    - For each channel, retrieves user list via `chan->getUsers()`
    - Sends `353` reply (NAMREPLY) for each channel
    - Sends `366` reply (ENDOFNAMES) at the end
  - **With channel argument**: Lists users in the specified channel
    - Retrieves the channel via `getChannel(channelName)`
    - If channel doesn't exist, returns `403` error (NOSUCHCHANNEL)
    - Otherwise, sends `353` reply with users and `366` to mark end

### User Prefix Display
Users are displayed with status indicators:
- `@` prefix: Channel operator
- `+` prefix: Voice privilege (if implemented)
- No prefix: Regular member

The `chan->getUsers()` method handles formatting these prefixes.

### IRC Standard
According to RFC 1459, NAMES returns:
- `353`: Channel name and user list
- `366`: End of NAMES list
- `403`: No such channel (error)

---

## Changes Made

### 1. Header File Updates (`includes/Command.hpp`)
- Added method declarations:
  ```cpp
  void lusers(void);
  void names(void);
  ```

### 2. Command Dispatcher (`src/Command.cpp`)
- Added dispatch logic in `execCmd()` to handle "LUSERS" and "NAMES" commands
- These commands are now recognized and routed to their respective implementations

### 3. Reply Messages (`includes/Replies.hpp`)
- Added `lusersreply()` static method:
  ```cpp
  static std::string lusersreply(const std::string &nick, int totalusers);
  ```
- Uses existing `namreply()` and `endofnames()` for NAMES command

### 4. New Command Files
- `cmd/LUSERS.cpp`: Implementation of LUSERS command
- `cmd/NAMES.cpp`: Implementation of NAMES command

### 5. Build System
- No Makefile changes needed - the Makefile automatically includes all `.cpp` files in the `cmd/` directory

---

## Testing

### LUSERS Command
```bash
# Connect to server and send:
LUSERS
# Expected response: :localhost 251 <your_nick> :There are <N> users
```

### NAMES Command
```bash
# List users in a specific channel:
NAMES #general
# Expected response:
# :localhost 353 <nick> = #general :<users>
# :localhost 366 <nick> #general :End of NAMES list

# List users in all channels:
NAMES
# Expected response: (list for all channels)
```

---

## Standards Compliance

Both commands follow RFC 1459 (Internet Relay Chat) specifications:
- Use proper numeric reply codes
- Include appropriate error handling
- Maintain IRC message format (prefix + numeric code + parameters + message)

