# CHEESE Protocol

CHEESE protocol is the secure communication protocol used by x-rat services.



### Quick Facts Table

| Item               | Value                                                        |
| ------------------ | ------------------------------------------------------------ |
| Transport Protocol | Any datagram based protocol                                  |
| Encryption         | AES-128-CCM, AES-128-GCM, AES-128-CTR (block cipher >= 128 bits) |
| Endianness         | Big Endian                                                   |



## Basic Concepts

CHEESE is a symmetric protocol, i.e. the protocol from both end are the same. The protocol provide the ability to tunnel IP endpoints between the firewall which is capable of deep packet inspection. The design aims to provide a minimal protocol footprint.

### User ID

Every user has a random, service provider assigned, 4-byte user ID.

### Encryption Key Generation

User may set a password key on both end and **keep it safe**. The encryption keys are derived from the master password using [PKCS#5 PBKDF2](https://tools.ietf.org/html/rfc2898#page-9). The salt Salt is generated randomly and can be changed by the initiator at any time. The iteration is 10,000 times.

### Minimum Packet Size

The minimum packet size is 58 bytes. The CHEESE header has 30 bytes, however it needs next 28 bytes to provides pseudo-randomness. The sender must append the data to at least 58 bytes if it does not have enough bytes.

## Packet Header

<table style="table-layout: fixed">
    <th>
        <td>0</td>
        <td>1</td>
        <td>2</td>
        <td>3</td>
    </th>
    <tr>
  	    <td>0</td>
        <td colspan="4">User Id (uint32, BE, xored)</td>
    </tr>
    <tr>
  	    <td>4</td>
        <td colspan="4" rowspan="2">Salt (8 byte array, xored)</td>
    </tr>
    <tr>
  	    <td>8</td>
    </tr>
    <tr>
  	    <td>12</td>
        <td colspan="4" rowspan="4">IV (16 byte array, xored)</td>
    </tr>
    <tr>
  	    <td>16</td>
    </tr>
    <tr>
  	    <td>20</td>
    </tr>
    <tr>
  	    <td>24</td>
    </tr>
    <tr>
        <td>28</td>
        <td colspan="2">Data Size (uint16, encrypted)</td>
        <td colspan="2">Payload (encrypted)</td>
    </tr>
    <tr>
        <td>32</td>
        <td colspan="4">Payload (encrypted)</td>
    </tr>
    <tr>
        <td>...</td>
        <td colspan="4">......</td>
    </tr>
    <tr>
        <td>...</td>
        <td colspan="4">Random Trail Data</td>
    </tr>
</table>

### Header Field Calculation

#### User Id

User id is given by service provider. Xored with payload[0:3].

#### Salt

Salt is generated randomly. It may be changed by initiator at any time. Xored with payload[4:11].

#### IV

First 12 bytes of IV are generated randomly. Last 4 bytes serves as counter. Xored with payload[12:27].

#### Data Size

Data size indicates the real size of a encapsulated packet. Packet may append to any length of data to introduce randomness.
