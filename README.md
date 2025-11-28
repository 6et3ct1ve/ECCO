# ECCO - Elliptic Curve Cryptography Operator

A command-line utility for encrypting and decrypting files using Elliptic Curve Cryptography (ECC) for macOS and Linux systems.

## Team Members

- **Butsko Bohdan**
- **Mishchishyn Volodymyr**
- **Hlazkova Yuliia**

## About The Project

ECCO is a lightweight, secure file encryption utility that leverages the power of Elliptic Curve Cryptography to protect your sensitive data. Built from scratch in C, this tool provides a simple command-line interface for encrypting and decrypting files with separate key generation and encryption operations.

### Key Features

- **Asymmetric Encryption**: Uses ECC for robust file security
- **Flexible Key Generation**: Generate key pairs independently with customizable sizes
- **Key Reusability**: Generate keys once, encrypt multiple files
- **Flexible Key Sizes**: Support for key lengths both **256 and 384 bits** (more coming soon)
- **Cross-Platform**: Compatible with macOS and Linux
- **Manual Implementation**: Built entirely in C without external cryptographic libraries

## Installation

### Note

### Installation 

```bash
git clone https://github.com/6et3ct1ve/ecco.git
cd ecco
make
sudo make install
```

## Usage

------

### Generate key pair

```bash
ecco -k [key_name] -o [keyring_name]
```

### Encrypt a file

```bash
ecco -k <public_key> -e <filename> -o <output_file>
```

### Decrypt a file

```bash
ecco -k <private_key> -e <filename> -o <output_file>
```

### Get help

```bash
ecco --help
```

## Examples

```bash
# Generate keys with default size (256 bits)
ecco -k

# Generate keys with 384-bit size
ecco -k P-384

# Encrypt a file using public key
ecco -k keyring.pub -e secret.txt -o output.txt

# Decrypt a file using private key
ecco -k keyring.priv -d output.txt -o secret.txt
```

## Workflow Example

```bash
# Step 1: Generate your key pair once
ecco -k 384

# Step 2: Encrypt multiple files with the same public key
ecco -e document1.pdf public_key.pub
ecco -e document2.txt public_key.pub
ecco -e photo.jpg public_key.pub

# Step 3: Decrypt files when needed
ecco -d document1.ecco private_key.priv
ecco -d document2.ecco private_key.priv
```

## Key Management

**Important Security Notes**:
- Store your private key securely! Without it, you cannot decrypt your files.
- Back up your private key in a safe location
- Never share your private key
- You can share your public key - it's safe for others to encrypt files for you

## Supported Key Sizes

- **256 bits** (default) - Fast, suitable for most use cases
- **384 bits** - Enhanced security
- **521 bits** - Maximum security level

## Project Structure

```
ECCO
├── include
│   ├── ecco
│   │   ├── aes_wrapper.h
│   │   ├── curves.h
│   │   ├── ecc_crypto.h
│   │   ├── ecco.h
│   │   ├── input_service.h
│   │   ├── kdf_wrapper.h
│   │   ├── key_generation.h
│   │   ├── main.h
│   │   ├── math_module.h
│   │   └── output_service.h
│   └── tinyargs
│       ├── funcs.c
│       ├── funcs.h
│       ├── parser.c
│       ├── parser.h
│       ├── README.md
│       ├── tests
│       │   └── test
│       ├── tinyargs.h
│       └── types.h
├── LICENSE
├── Makefile
├── README.md
├── requirments.txt
├── src
│   ├── aes_wrapper.c
│   ├── curves.c
│   ├── ecc_crypto.c
│   ├── input_service.c
│   ├── kdf_wrapper.c
│   ├── key_generation.c
│   ├── main.c
│   ├── math_module.c
│   └── output_service.c
└── tests
    ├── test_crypto.c
    ├── test_curves.c
    ├── test_ecc_crypto.c
    ├── test_io.c
    ├── test_kdf_aes.c
    ├── test_key_generation.c
    └── test_math_module.c
```

## Requirements

- GCC compiler
- macOS or Linux operating system
- POSIX-compliant environment
- Make utility

## Documentation

For detailed documentation, see `man ecco` after installation.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Note**: This is a learning project created for educational purposes as part of a C programming course. For production use, consider using established cryptographic libraries and tools that have undergone extensive security audits.
```
