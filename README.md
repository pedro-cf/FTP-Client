# FTP Client

FTP Client is a command-line application that allows you to download a single file from an FTP server using FTP sockets. It is written in C and uses sockets to communicate with the FTP server.

## Usage

To use FTP Client, you need to compile the source code provided in this repository. Here are the steps to follow:

1. Clone this repository to your local machine:

   ***
   git clone https://github.com/pedro-cf/FTP-Client.git
   ***

2. Change to the cloned directory:

   ***
   cd FTP-Client
   ***

3. Compile the source code:

   ***
   gcc -o ftp_client ftp_client.c
   ***

4. Download a single file from an FTP server:

   ***
   ./ftp_client ftp://[<user>:<password>@]<host>/<url-path>
   ***

   This command downloads a single file from the specified FTP server. Replace `<user>` and `<password>` with your username and password (if required), `<host>` with the FTP server address, and `<url-path>` with the path to the file on the server.

## Requirements

FTP Client requires the following:

* C compiler (such as GCC)
* An FTP server to download files from

## License

FTP Client is released under the MIT License. See the ***LICENSE*** file for details.

## Support

If you find this project useful, consider buying me a coffee! Donations help keep this project going and are greatly appreciated.

[![Buy me a coffee](https://img.shields.io/badge/-Buy%20me%20a%20coffee-orange?logo=buy-me-a-coffee&logoColor=white&style=for-the-badge)](https://www.buymeacoffee.com/pedro_cf)
