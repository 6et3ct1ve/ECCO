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
- **Flexible Key Sizes**: Support for key lengths from **256 to 521 bits**
- **Cross-Platform**: Compatible with macOS and Linux
- **Manual Implementation**: Built entirely in C without external cryptographic libraries

## Installation

```bash
git clone https://github.com/6et3ct1ve/ecco.git
cd ecco
make
sudo make install
```

### Uninstall

```bash
sudo make uninstall
```

## Usage

### Generate key pair

```bash
ecco -k [key_size]
```

### Encrypt a file

```bash
ecco -e <filename> <public_key>
```

### Decrypt a file

```bash
ecco -d <filename> <private_key>
```

### Get help

```bash
ecco --help
man ecco
```

## Examples

```bash
# Generate keys with default size (256 bits)
ecco -k

# Generate keys with 384-bit size
ecco -k 384

# Encrypt a file using public key
ecco -e secret.txt public_key.pem

# Decrypt a file using private key
ecco -d secret.txt.ecco private_key.pem
```

## Workflow Example

```bash
# Step 1: Generate your key pair once
ecco -k 384

# Step 2: Encrypt multiple files with the same public key
ecco -e document1.pdf public_key.pem
ecco -e document2.txt public_key.pem
ecco -e photo.jpg public_key.pem

# Step 3: Decrypt files when needed
ecco -d document1.pdf.ecco private_key.pem
ecco -d document2.txt.ecco private_key.pem
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
ecco/
├── README.md
├── LICENSE
├── Makefile
├── ecco.1
├── requirements.txt
├── include/
│   └── ecco/
│       ├── input_service.h
│       ├── file_buffer.h
│       ├── output_service.h
│       ├── ecc_crypto.h
│       ├── key_generation.h
│       ├── curve_params.h
│       ├── aes_wrapper.h
│       ├── kdf.h
│       └── ecco.h
├── src/
│   ├── main.c
│   ├── input_service.c
│   ├── file_buffer.c
│   ├── output_service.c
│   ├── ecc_crypto.c
│   ├── key_generation.c
│   ├── curve_params.c
│   ├── aes_wrapper.c
│   └── kdf.c
├── lib/
│   └── tinyargs/
│       ├── tinyargs.c
│       └── tinyargs.h
├── tests/
│   ├── test_ecc_crypto.c
│   ├── test_key_generation.c
│   └── test_file_buffer.c
└── demo/
    └── demo_ecco.c
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
