<h1>RFC for R-Type Game Engine Networking Protocol</h1>

<h2>Request for Comments: 1001</h2>
<p><strong>Category:</strong> Experimental</p>
<p><strong>Updated:</strong> September 2024</p>
<p><strong>Author:</strong> Fizo55</p>

---

<h2>Abstract</h2>
<p>This RFC defines the binary protocol used in the multiplayer R-Type game for communication between clients and the game server. The protocol facilitates the real-time exchange of game states, player inputs, and other gameplay events over the network, using UDP as the primary transport layer for in-game communications. The protocol is designed to minimize latency while ensuring consistency between all players in the game.</p>

---

<h2>1. Introduction</h2>
<p>R-Type is a networked multiplayer game where several players fight the evil Bydos in real-time. The game's protocol governs all client-server communications, including player movements, enemy spawning, missile firing, and game updates.</p>
<p>The protocol is <strong>binary-based</strong> to optimize bandwidth efficiency, and it uses <strong>UDP</strong> to prioritize speed. Some critical messages (e.g., connection setup) will use <strong>TCP</strong> for reliability. This RFC outlines the message structure, data encoding, and methods for handling network unreliability like packet loss.</p>

<h2>2. Transport Layer</h2>

<h3>2.1 UDP Usage</h3>
<ul>
  <li><strong>UDP</strong> is the primary transport protocol used for real-time game state synchronization between clients and the server. This ensures low-latency communication, but since UDP is inherently unreliable, mechanisms to handle packet loss, duplication, and out-of-order packets are built into the protocol.</li>
</ul>

<h3>2.2 TCP Usage</h3>
<ul>
  <li><strong>TCP</strong> is employed only for critical communications like connection initiation, user authentication, and game session management. This ensures that important information is transmitted reliably, even at the expense of some delay.</li>
</ul>

---

<h2>3. Message Structure</h2>
<p>Each message in the protocol consists of a <strong>header</strong> and a <strong>payload</strong>. The header contains metadata about the message, while the payload contains the actual game-related data.</p>

<h3>3.1 Header Format (8 bytes)</h3>

<table>
  <thead>
    <tr>
      <th>Field</th>
      <th>Size</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Message ID</td>
      <td>2 bytes</td>
      <td>A unique identifier for each type of message.</td>
    </tr>
    <tr>
      <td>Sequence No</td>
      <td>2 bytes</td>
      <td>A sequence number for tracking packets in UDP.</td>
    </tr>
    <tr>
      <td>Client ID</td>
      <td>2 bytes</td>
      <td>Identifies the client or player sending the message.</td>
    </tr>
    <tr>
      <td>Flags</td>
      <td>2 bytes</td>
      <td>Special control flags (e.g., ACK, RE-TRANSMIT).</td>
    </tr>
  </tbody>
</table>

<h3>3.2 Payload Format</h3>
<p>The payload format depends on the type of message. Each message type is described below, including its unique <strong>Message ID</strong> and payload structure.</p>

---

<h2>4. Protocol Messages</h2>

<h3>4.1 Connection Management</h3>

<h4>4.1.1 <code>CONNECT_REQUEST</code> (Message ID: 0x0001)</h4>
<p>Sent by the client to request connection to the server.</p>
<table>
  <thead>
    <tr>
      <th>Field</th>
      <th>Size</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Client Name</td>
      <td>32 bytes</td>
      <td>The name of the player.</td>
    </tr>
  </tbody>
</table>

<h4>4.1.2 <code>CONNECT_RESPONSE</code> (Message ID: 0x0002)</h4>
<p>Sent by the server to acknowledge the connection request and provide the client's ID.</p>

<table>
  <thead>
    <tr>
      <th>Field</th>
      <th>Size</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Client ID</td>
      <td>2 bytes</td>
      <td>The unique ID assigned by the server.</td>
    </tr>
  </tbody>
</table>

---

<h3>4.2 Game State Updates</h3>

<h4>4.2.1 <code>PLAYER_MOVE</code> (Message ID: 0x1001)</h4>
<p>Sent by the client to indicate player movement.</p>
<table>
  <thead>
    <tr>
      <th>Field</th>
      <th>Size</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>PosX</td>
      <td>2 bytes</td>
      <td>The player's X position.</td>
    </tr>
    <tr>
      <td>PosY</td>
      <td>2 bytes</td>
      <td>The player's Y position.</td>
    </tr>
    <tr>
      <td>Direction</td>
      <td>1 byte</td>
      <td>Direction (0: Left, 1: Right, etc.).</td>
    </tr>
  </tbody>
</table>

<h4>4.2.2 <code>FIRE_MISSILE</code> (Message ID: 0x1002)</h4>
<p>Sent by the client when firing a missile.</p>
<table>
  <thead>
    <tr>
      <th>Field</th>
      <th>Size</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>MissileType</td>
      <td>1 byte</td>
      <td>The type of missile fired.</td>
    </tr>
    <tr>
      <td>PosX</td>
      <td>2 bytes</td>
      <td>The missile's starting X position.</td>
    </tr>
    <tr>
      <td>PosY</td>
      <td>2 bytes</td>
      <td>The missile's starting Y position.</td>
    </tr>
  </tbody>
</table>

<h4>4.2.3 <code>ENEMY_SPAWN</code> (Message ID: 0x2001)</h4>
<p>Sent by the server to inform clients of a new enemy spawn.</p>
<table>
  <thead>
    <tr>
      <th>Field</th>
      <th>Size</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Enemy ID</td>
      <td>2 bytes</td>
      <td>The unique ID of the spawned enemy.</td>
    </tr>
    <tr>
      <td>PosX</td>
      <td>2 bytes</td>
      <td>The X position of the enemy.</td>
    </tr>
    <tr>
      <td>PosY</td>
      <td>2 bytes</td>
      <td>The Y position of the enemy.</td>
    </tr>
    <tr>
      <td>EnemyType</td>
      <td>1 byte</td>
      <td>Type of enemy spawned.</td>
    </tr>
  </tbody>
</table>

<h4>4.2.4 <code>ENTITY_DESTROYED</code> (Message ID: 0x2002)</h4>
<p>Sent by the server when an entity (player, enemy, missile) is destroyed.</p>
<table>
  <thead>
    <tr>
      <th>Field</th>
      <th>Size</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Entity ID</td>
      <td>2 bytes</td>
      <td>ID of the destroyed entity.</td>
    </tr>
    <tr>
      <td>PosX</td>
      <td>2 bytes</td>
      <td>X position of the destruction.</td>
    </tr>
    <tr>
      <td>PosY</td>
      <td>2 bytes</td>
      <td>Y position of the destruction.</td>
    </tr>
  </tbody>
</table>

---

<h2>5. Reliability Mechanisms</h2>

<h3>5.1 Sequence Numbering</h3>
<p>Each UDP packet contains a <strong>Sequence Number</strong> in the header. The server and clients use this sequence number to detect and handle out-of-order or duplicate packets.</p>

<h3>5.2 Acknowledgments (ACK)</h3>
<ul>
  <li>Critical messages, such as those handling connection and disconnection, will require <strong>ACK</strong> messages to confirm successful delivery. These messages will include the ACK flag in the header.</li>
</ul>

<h3>5.3 Retransmission</h3>
<p>If an expected ACK is not received within a predefined timeout, the sender will re-transmit the message. Each message will have a retransmission counter to avoid infinite loops.</p>

---

<h2>6. Error Handling</h2>

<h3>6.1 Invalid Messages</h3>
<p>If a client or server receives a message that doesn't conform to the protocol (e.g., corrupted or invalid message ID), it will ignore the message and send an <code>ERROR_RESPONSE</code> message (Message ID: 0x9999) with an error code indicating the issue.</p>

<table>
  <thead>
    <tr>
      <th>Field</th>
      <th>Size</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>ErrorCode</td>
      <td>2 bytes</td>
      <td>Error code describing the failure.</td>
    </tr>
  </tbody>
</table>

<h3>6.2 Packet Loss</h3>
<p>The protocol is designed to tolerate packet loss, especially during gameplay updates (e.g., player position, missile firing). The game will continue even if some position or action updates are missed, but key events (e.g., death, pickups) are sent with guaranteed delivery mechanisms.</p>

---

<h2>7. Security Considerations</h2>
<ul>
  <li><strong>Client Authentication</strong>: Each client must authenticate with the server using a secure method during connection establishment.</li>
  <li><strong>Data Validation</strong>: All incoming data must be validated to prevent buffer overflows, memory corruption, and other security risks.</li>
  <li><strong>Replay Protection</strong>: Sequence numbers prevent replay attacks by ensuring that old or duplicated packets are ignored.</li>
</ul>

---

<h2>8. Conclusion</h2>
<p>This protocol is designed to optimize real-time gameplay in R-Type by balancing low-latency UDP communication with mechanisms to ensure critical message reliability. The binary format ensures efficient transmission, while the use of sequence numbers and ACKs helps mitigate the inherent unreliability of UDP.</p>
---

<h3>Authors:</h3>
<p>Fizo55</p>
