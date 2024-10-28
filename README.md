<h1>R-TYPE: A Modern Recreation</h1>

<p>Welcome to <strong>R-TYPE</strong>, a modern reimagining of the iconic side-scrolling shoot 'em up game. Immerse yourself in this multiplayer network game where players band together to fend off waves of enemies in a fast-paced, action-packed space adventure.</p>

<p><strong>R-TYPE</strong> combines classic gameplay mechanics with modern networking features, allowing players to connect across the globe and enjoy intense battles in space.</p>

<hr>

<h2>Game Overview</h2>

<p><strong>R-TYPE</strong> is a horizontal shoot 'em up game set in space, where you and other players battle alien forces. Equipped with powerful weapons, upgrades, and boosts, your mission is to survive and defeat as many enemies as possible while navigating through dangerous space environments.</p>

<hr>

<h2>R-TYPE Server: The Core of the Game</h2>

<p>The <strong>R-TYPE Server</strong> is the heart of the game. It handles all network operations, player connections, game state management, and interactions between players and enemies.</p>

<blockquote>
    <p><strong>Written in C++</strong></p>
</blockquote>

<h3>Usage</h3>

<pre><code>USAGE: ./r-type_server -p port
    port        the port number
</code></pre>

<p>Once launched, the server manages the game state and allows multiple <span style="color:#4d30bf">GUI</span> clients to connect and interact with it.</p>

<hr>

<h2>GUI Client: Your Window into the Game</h2>

<p>The <strong>GUI Client</strong> allows players to interact with the game visually, controlling their spaceship, shooting enemies, and seeing other players.</p>

<h3>Usage</h3>

<pre><code>USAGE: ./r-type_gui -p port -n name [-h host]
    port        the port number
    name        the player name
    host        the server host (default: localhost)
</code></pre>

<p>The <span style="color:#4d30bf">GUI</span> displays menus, game levels, and other players in real-time.</p>

<h3>Client-Server Communication Protocol</h3>

<p>The <strong>GUI</strong> communicates with the server using a well-defined protocol. For more details, see the <a href="./server/readme.md">server protocol documentation</a>.</p>

<h4>Example Commands:</h4>

<table>
  <thead>
    <tr>
      <th>Command from Server</th>
      <th>Command from Client</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>player X Y\n</td>
      <td>player\n</td>
      <td>Sends the player's position</td>
    </tr>
  </tbody>
</table>

<p>This communication flow ensures smooth synchronization between the client and the server:</p>

<ol>
  <li>The client connects to the server on the specified port.</li>
  <li>The server and the client exchange data using the protocol defined above.</li>
</ol>

<hr>

<h2>Game Mechanics and Features</h2>

<h3>Context</h3>

<pre>
The game takes place in outer space where you must defeat monstrous enemies.
You can collect power-ups, new weapons, and boosts to help you survive
and complete your mission. Movement is controlled using the arrow keys,
while firing weapons is handled via keyboard inputs.
</pre>

<p>Prepare for thrilling multiplayer action as you fight your way through enemies in classic <strong>R-TYPE</strong> fashion!</p>

<hr>

<h2>General Information</h2>

<p>For more detailed documentation, refer to the protocol and server details in the respective subdirectories. Enjoy the game!</p>
