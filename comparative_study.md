### Comparative Study

While developping our R-type, we had to make som technology choices to improve the process speed and other aspects of the program.

#### 1. Data Transmission: Binary Protocol with UDP

**Choice**: Utilizing binary data transmission via UDP (User Datagram Protocol).

**Rationale**:
- **Performance**: 
  - **Speed**: UDP is faster than TCP (Transmission Control Protocol) because it does not require handshaking, which reduces latency. It is something important for a real time multiplayer game, for both the players to get the informations at the same time.
  - **Reduced Overhead**: Binary protocols result in smaller message sizes compared to text-based protocols. This decreases bandwidth usage and transmission delay.

- **Real-Time Communication**: 
  - UDP is connectionless, which allows the server and clients to send messages without establishing a persistent connection. This is ideal for real-time applications like gaming, where timely delivery is more important than guaranteed delivery.
  
- **Simplicity in Data Structure**: 
  - Binary data can represent complex data structures more efficiently. This is particularly advantageous for transmitting game state information, player actions, and other game events.

**Considerations**:
- **Reliability**: 
  - UDP does not guarantee message delivery, order, or integrity. This means that some packets may be lost or arrive out of order. However, in many fast-paced games, it’s often acceptable to lose some data rather than suffer from the latency introduced by waiting for missing packets.

#### 2. Graphics Library: SDL2

**Choice**: Selecting SDL2 (Simple DirectMedia Layer 2) as the graphics library.

**Rationale**:
- **Cross-Platform Compatibility**: 
  - SDL2 is highly portable, supporting multiple platforms, Windows and Linux, on which our we want our game to work.

- **Ease of Use**: 
  - SDL2 provides a simple API that simplifies the handling of graphics and input, making it easier for us, developers, to focus on game logic rather than low-level programming details.

- **Performance**: 
  - SDL2 is designed for high performance, enabling hardware acceleration for rendering, which is crucial for a visually demanding game like R-type. It efficiently manages graphics rendering, audio playback, and input handling.

- **Community and Resources**: 
  - A large community and extensive documentation exist around SDL2, providing ample resources for troubleshooting, examples, and libraries that can be integrated with SDL2 for enhanced functionalities (e.g., physics engines, networking).

**Considerations**:
- **Limited High-Level Features**: 
  - While SDL2 provides a solid foundation for 2D graphics and basic input handling, it may lack some higher-level features found in more comprehensive engines like Unity or Unreal. This means developers may need to implement additional systems (e.g., entity-component systems, physics) from scratch.

- **Learning Curve for Advanced Features**: 
  - While basic usage is straightforward, mastering SDL2’s more advanced features (e.g., handling multiple windows, advanced audio control) can present a learning curve for developers who are new to game development.

### Comparative Summary

| Feature                      | Binary Data Transmission with UDP | SDL2 Graphics Library                   |
|------------------------------|-----------------------------------|----------------------------------------|
| **Performance**              | High-speed, low-latency           | High-performance graphics rendering     |
| **Overhead**                 | Low packet size                   | Moderate overhead with API abstractions |
| **Complexity**               | Requires custom reliability logic  | Simplifies graphics and input handling  |
| **Platform Support**         | Platform agnostic                  | Cross-platform compatibility            |
| **Community Support**        | Niche but growing                  | Large community, extensive resources    |
| **Learning Curve**           | Moderate (for custom protocols)    | Low for basics, moderate for advanced   |
| **Real-time Suitability**    | Excellent for real-time gaming     | Great for fast-paced graphics           |
| **Error Handling**           | Custom implementation needed        | Built-in but limited to graphics context|

### Conclusion

The choice of using binary data transmission over UDP and SDL2 for graphics in the R-type project supports the primary objectives of performance and responsiveness in a real-time gaming environment. While both choices have considerations regarding reliability and feature richness, they collectively enable a robust framework for developing a high-performance multiplayer game. This comparative study can guide future enhancements and assist in evaluating alternative technologies as the project evolves.