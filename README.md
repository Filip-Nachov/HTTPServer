# HTTPServer

HTTPServer is a simple HTTP server written in C, capable of serving static files over the HTTP protocol.

## Features

- Supports HTTP GET requests for static files.
- Multi-threaded design using pthreads to handle multiple client connections simultaneously.
- Proper error handling for socket operations and file I/O.

## Prerequisites

- GCC compiler
- Linux environment (tested on Arch Linux)

## Usage

1. Clone the repository:

    ```bash
    git clone https://github.com/yourusername/HTTPServer.git
    ```

2. Navigate to the project directory:

    ```bash
    cd HTTPServer
    ```

3. Make a directory to store the compiled code
    ```bash
    mkdir build
    ```

5. Compile the server:

    ```bash
    make
    ```

6. Run the server:

    ```bash
    ./server
    ```

5. The server will start listening on port 8080 by default. You can change the port in the `server.c` file if needed.

6. Access files using your web browser or HTTP client by specifying the file path in the URL, e.g., `http://localhost:8080/index.html`.

## License

This project is licensed under the [GNU GENERAL PUBLIC LICENSE Version 3](LICENSE).

## Acknowledgements

- This project was inspired by the desire to learn more about network programming and HTTP servers.
- Special thanks to online resources and tutorials that helped me in understanding socket programming in C.

Feel free to contribute to this project by submitting pull requests or reporting issues.
