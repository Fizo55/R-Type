# R-TYPE

This is a recreation of a famous game, the R-Type.
A network game with multiple players and ennemies to kill.
The game is a shoot them up, a game where you shoot enemies on a horizontal world.

## First executable: R-type Server

This is the main server, that generates everything we need.
> Created in C++

### Usage

```sh
USAGE: ./r-type_server -p port
    port        the port number
```

The server, once launched, is the core of the game himself.
We can connect multiple <span style="color:#4d30bf">GUI</span> in order to interact with it.

## GUI executable

### Usage

```sh
USAGE: ./r-type_gui -p port -n name [-h host]
    port        the port number
    name        the player name
    host        the host (default localhost)
```

The <span style="color:#4d30bf">GUI</span> is used to display menus and levels. You can also see other players.

#### Protocol

> This is how the <span style="color:#4d30bf">GUI</span> can interact with the server
> See the protocol [here](./server/readme.md)

##### Commands:

|                   Server                   |   Client  |                   Details                     |
|--------------------------------------------|:---------:|-----------------------------------------------|
| player X Y\n                               | player\n  | player position                               |

This is how informations are transmitted between the server and the <span style="color:#4d30bf">GUI</span>:

```
1- The client connects the GUI to the server's port

2- The server and the client communicate the following way:
```

### General informations
#### Context
<pre>
The game is about killing monstruous ennemies in space.
You can have power ups, guns and boosts that allows you to
complete your mission more easily.
You move with arrows.
</pre>
