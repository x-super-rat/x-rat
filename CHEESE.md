# CHEESE Protocol

CHEESE protocol is the secure communication protocol used by x-rat services.



### Quick Facts Table

| Item               | Value                                 |
| ------------------ | ------------------------------------- |
| Transport Protocol | Any datagram based protocol           |
| Encryption         | AES-128-CCM, AES-128-GCM, AES-128-CTR |
| Endianness         | Big Endian                            |

#### 

## Basic Concepts

### User ID

Every user has a random, service provider assigned, 2-byte user ID.

### Encryption Key Generation

User may set a password master key and **keep it safe**. The encryption keys are derived from the master password using [PKCS#5 PBKDF2](https://tools.ietf.org/html/rfc2898#page-9).