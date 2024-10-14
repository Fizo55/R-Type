# RFC: R-Type Network Protocol Specification

**Author:** Fizo55
**Date:** 08 October 2024
**Version:** 1.1

## Table of Contents

1. [Introduction](#1-introduction)
2. [Conventions](#2-conventions)
3. [Overview](#3-overview)
4. [Packet Structure](#4-packet-structure)
   - 4.1 [Common Header](#41-common-header)
   - 4.2 [Message Types](#42-message-types)
5. [Data Types and Encoding](#5-data-types-and-encoding)
6. [Packet Definitions](#6-packet-definitions)
   - 6.1 [Connection Packets](#61-connection-packets)
     - 6.1.1 [Client Hello](#611-client-hello)
     - 6.1.2 [Server Welcome](#612-server-welcome)
     - 6.1.3 [Connection Denied](#613-connection-denied)
   - 6.2 [Gameplay Packets](#62-gameplay-packets)
     - 6.2.1 [Player Input](#621-player-input)
     - 6.2.2 [Game State Update](#622-game-state-update)
     - 6.2.3 [Entity Spawn](#623-entity-spawn)
     - 6.2.4 [Entity Destroy](#624-entity-destroy)
     - 6.2.5 [Chat Message](#625-chat-message)
   - 6.3 [Acknowledgment Packets](#63-acknowledgment-packets)
7. [Reliability and Ordering](#7-reliability-and-ordering)
8. [Error Handling](#8-error-handling)
9. [Security Considerations](#9-security-considerations)
10. [Extensibility](#10-extensibility)
11. [Example Communication Flow](#11-example-communication-flow)
12. [Conclusion](#12-conclusion)

---

## 1. Introduction

This document specifies the binary protocol used for communication between clients and the server in the networked R-Type game. The protocol is designed to be efficient, reliable, and suitable for real-time gameplay over UDP.

---

## 2. Conventions

- All multi-byte integers are transmitted in **big-endian** (network byte order).
- Numeric data types are of fixed size to ensure consistency across platforms.
- Strings are encoded in UTF-8 and are prefixed with a 16-bit unsigned length.

---

## 3. Overview

The R-Type network protocol uses a custom binary format over UDP to exchange messages between clients and the server. The protocol includes mechanisms for reliability, ordering, and session management to handle the unreliability of UDP.

---

## 4. Packet Structure

### 4.1 Common Header

Each packet begins with a common header that includes essential metadata.

| Offset (bytes) | Size (bytes) | Field         | Description                             |
|----------------|--------------|---------------|-----------------------------------------|
| 0              | 2            | Length        | Total packet length excluding this field|
| 2              | 2            | Sequence ID   | Packet sequence number                  |
| 4              | 1            | Message Type  | Type of the message (see Message Types) |
| 5              | 1            | Flags         | Packet flags (e.g., ACK request)        |
| **Total**      | **6**        |               |                                         |

- **Length** (`uint16_t`): Total length of the packet payload, excluding the length field itself.
- **Sequence ID** (`uint16_t`): Used for packet ordering and acknowledgment.
- **Message Type** (`uint8_t`): Specifies the type of the message.
- **Flags** (`uint8_t`): Bitmask for additional packet options.

### 4.2 Message Types

| Value | Message Type        |
|-------|---------------------|
| 0x01  | Client Hello        |
| 0x02  | Server Welcome      |
| 0x03  | Connection Denied   |
| 0x10  | Player Input        |
| 0x11  | Game State Update   |
| 0x12  | Entity Spawn        |
| 0x13  | Entity Destroy      |
| 0x14  | Chat Message        |
| 0xFE  | Acknowledgment      |
| 0xFF  | Keep-Alive          |

---

## 5. Data Types and Encoding

- **`uint8_t`**: Unsigned 8-bit integer
- **`uint16_t`**: Unsigned 16-bit integer
- **`uint32_t`**: Unsigned 32-bit integer
- **`float`**: 32-bit IEEE 754 floating-point number
- **`string`**: Length-prefixed UTF-8 string (`uint16_t` length + bytes)

---

## 6. Packet Definitions

### 6.1 Connection Packets

#### 6.1.1 Client Hello

Sent by the client to initiate a connection.

**Message Type:** `0x01`

**Payload:**

| Offset | Size | Field     | Description            |
|--------|------|-----------|------------------------|
| 0      | 2    | Protocol Version | Version number (e.g., `0x0001`) |

#### 6.1.2 Server Welcome

Sent by the server to accept a client's connection.

**Message Type:** `0x02`

**Payload:**

| Offset | Size | Field     | Description            |
|--------|------|-----------|------------------------|
| 0      | 4    | Client ID | Assigned unique client ID (`uint32_t`) |

#### 6.1.3 Connection Denied

Sent by the server if the connection is rejected.

**Message Type:** `0x03`

**Payload:**

| Offset | Size | Field Length | Field     | Description            |
|--------|------|--------------|-----------|------------------------|
| 0      | 2    | N/A          | Reason Length | Length of reason string (`uint16_t`) |
| 2      | N    | N            | Reason    | UTF-8 encoded reason message |

### 6.2 Gameplay Packets

#### 6.2.1 Player Input

Sent by the client to provide input actions.

**Message Type:** `0x10`

**Payload:**

| Offset | Size | Field     | Description            |
|--------|------|-----------|------------------------|
| 0      | 1    | Input Flags | Bitmask of input actions |
| 1      | 4    | Timestamp  | Client timestamp (`uint32_t`) |

**Input Flags (bitmask):**

- Bit 0 (`0x01`): Move Up
- Bit 1 (`0x02`): Move Down
- Bit 2 (`0x04`): Move Left
- Bit 3 (`0x08`): Move Right
- Bit 4 (`0x10`): Fire
- Bits 5-7: Reserved

#### 6.2.2 Game State Update

Sent by the server to update clients on the game state.

**Message Type:** `0x11`

**Payload:**

| Offset | Size | Field     | Description            |
|--------|------|-----------|------------------------|
| 0      | N    | Entities  | List of entity states  |

**Entity State Structure:**

Each entity state is structured as follows:

| Offset | Size | Field     | Description            |
|--------|------|-----------|------------------------|
| 0      | 4    | Entity ID | Unique entity ID (`uint32_t`) |
| 4      | 1    | Entity Type | Type of entity (`uint8_t`) |
| 5      | 4    | Position X | X coordinate (`float`) |
| 9      | 4    | Position Y | Y coordinate (`float`) |
|13      | 4    | Velocity X | X velocity (`float`)   |
|17      | 4    | Velocity Y | Y velocity (`float`)   |
|21      | N    | Additional Data | Entity-specific data |

#### 6.2.3 Entity Spawn

Sent by the server to notify clients of a new entity.

**Message Type:** `0x12`

**Payload:**

Same as the Entity State Structure defined above.

#### 6.2.4 Entity Destroy

Sent by the server to notify clients that an entity has been destroyed.

**Message Type:** `0x13`

**Payload:**

| Offset | Size | Field     | Description            |
|--------|------|-----------|------------------------|
| 0      | 4    | Entity ID | Unique entity ID (`uint32_t`) |

#### 6.2.5 Chat Message

Optional: Sent by clients or server to transmit chat messages.

**Message Type:** `0x14`

**Payload:**

| Offset | Size | Field Length | Field     | Description            |
|--------|------|--------------|-----------|------------------------|
| 0      | 2    | N/A          | Message Length | Length of message (`uint16_t`) |
| 2      | N    | N            | Message   | UTF-8 encoded string   |

### 6.3 Acknowledgment Packets

Used to acknowledge receipt of packets when reliability is needed.

**Message Type:** `0xFE`

**Payload:**

| Offset | Size | Field         | Description                |
|--------|------|---------------|----------------------------|
| 0      | 2    | Sequence ID   | Sequence ID being acknowledged (`uint16_t`) |

---

## 7. Reliability and Ordering

- **Sequence ID**: Incremented with each packet sent; used to detect lost or out-of-order packets.
- **Acknowledgments**: Sent in response to packets requiring acknowledgment (ACK flag set in the header).

**Flags Field (Header):**

- Bit 0 (`0x01`): ACK Required
- Bits 1-7: Reserved

---

## 8. Error Handling

- **Malformed Packets**: Should be discarded; may optionally log an error.
- **Unknown Message Types**: Should be ignored to maintain forward compatibility.
- **Timeouts**: Clients should consider the connection lost if no messages are received within a specified timeout period.

---

## 9. Security Considerations

- **Input Validation**: All incoming data must be validated to prevent buffer overflows and injection attacks.
- **Denial of Service**: Implement rate limiting and connection limits to prevent abuse.
- **Authentication**: Not covered in this version; recommended for future versions.

---

## 10. Extensibility

- **Versioning**: Protocol versions are included in the Client Hello and Server Welcome packets.
- **Reserved Message Types**: Message types `0x80` to `0xEF` are reserved for future use.
- **Extension Mechanisms**: Additional data can be appended to packets if both client and server understand the extended format.

---

## 11. Example Communication Flow

1. **Client initiates connection:**

   - Sends **Client Hello** with protocol version.

2. **Server responds:**

   - If accepted, sends **Server Welcome** with assigned client ID.
   - If rejected, sends **Connection Denied** with a reason.

3. **Gameplay:**

   - Client sends **Player Input** packets with input flags.
   - Server sends **Game State Update** packets to all clients.

4. **Entity Events:**

   - Server sends **Entity Spawn** and **Entity Destroy** packets as entities are created or removed.

5. **Acknowledgments:**

   - If a packet requires acknowledgment, the receiver sends an **Acknowledgment** packet with the corresponding sequence ID.

---

## 12. Conclusion

This document outlines the R-Type network protocol for client-server communication. By adhering to this specification, developers can ensure interoperability and a consistent gameplay experience across different implementations.

---
