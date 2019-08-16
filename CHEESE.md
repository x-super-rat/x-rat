# CHEESE Protocol

CHEESE protocol is the secure communication protocol used by x-rat services.



### Quick Facts Table

| Item               | Value                       |
| ------------------ | --------------------------- |
| Transport Protocol | Any datagram based protocol |
| Encryption         | AES-128-CCM, AES-128-GCM    |
| Endianness         | Big Endian                  |



## Basic Concepts

CHEESE is a symmetric protocol, i.e. the protocol from both end are the same. The protocol provide the ability to tunnel IP endpoints between the firewall which is capable of deep packet inspection. The design aims to provide a minimal protocol footprint.

### User ID

Every user has a random, service provider assigned, 4-byte user ID.

### Minimum Packet Size

The minimum packet size is 32 bytes. The CHEESE header has 16 bytes, and the minimum cipher block of 16 bytes.

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
        <td colspan="4">User Id (uint32, BE)</td>
    </tr>
    <tr>
  	    <td>4</td>
        <td colspan="4" rowspan="2">Random Data (uint64, BE)</td>
    </tr>
    <tr>
  	    <td>8</td>
    </tr>
    <tr>
        <td>12</td>
        <td colspan="2">Data Size (uint16)</td>
        <td colspan="2">Reserved (random)</td>
    </tr>
    <tr>
        <td>16</td>
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
	<tr>
        <td>...</td>
        <td colspan="4">IV Counter (if algorithms requires counter) (uint64, BE)</td>
    </tr>
</table>



### Packet Field Calculation

#### User Id

User id is given by service provider. Xored with `payload[0:3]`.

#### Random Data

Random Data is a cryptographic random uint64, in big-endian mode.

#### Data Size

Data size indicates the real size of a encapsulated packet. Packet may append to any length of data to introduce randomness. Xored with `header[13:14]`.

#### IV Counter

If the encryption features counter mode, the last 4 bytes of the packet are the counter value, in big endian mode. Xored with `header[4:7]`.

### Encryption Fields Generation

#### Master Key

User may set a password key on both end and **keep it safe**. The master key is derived from `SHA512(password)`.

#### Salt

The salt is calculated from `PBKDF2(SHA512(masterkey), random, 1)`.

#### Encryption/Decryption Key

The encryption keys are derived from the master key using [PKCS#5 PBKDF2](https://tools.ietf.org/html/rfc2898#page-9). The key is calculated from `PBKDF2(masterkey, salt, 1000)`.

#### IV

IV generation is different for each encryption algorithms. See encryption section for details.



### Encryption Algorithms

CHEESE supports multiple encryption algorithms, one may implement the protocol even with the cipher not in this document. Officially supported algorithms are listed below:

1. AES-GCM-128
2. AES-CCM-128



#### AES-GCM-128

##### IV

In NIST SP800 38-D, AES-GCM may uses the IV of 12 bytes/96 bits, leaving 4 bytes/64 bits as the counter. The 12 bytes are calculated from `HKDF-SHA256(salt, random)[0:11]`. The counter value is retrieved from last 4 bytes of the packet.



















