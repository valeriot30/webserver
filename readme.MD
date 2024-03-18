markdown
Copy code
# Web Server in C - README

This repository contains a simple web server implemented in C language. It serves as a basic example of how to create a minimalistic web server capable of handling HTTP requests.

## How to Compile and Run

1. **Clone the Repository:**
git clone https://github.com/valeriot30/webserver
cd webserver

markdown
Copy code

2. **Compile the Server:**
Ensure you have GCC (GNU Compiler Collection) installed on your system.
gcc -o webserver webserver.c

markdown
Copy code

3. **Run the Server:**
Execute the provided bash script named `script.sh` to start the server.
./script.sh or sh script.sh

less
Copy code
This script compiles the server (if not already compiled) and runs it on the default port 8080.

## Usage

Once the server is running, it will listen for incoming HTTP requests. You can access the server through a web browser or use tools like cURL to make requests.

By default, the server serves files from the `public` directory. Place your HTML, CSS, JavaScript, or any other web content in this directory to be served by the server.

## Additional Notes

- The server is intentionally kept simple for educational purposes and may not be suitable for production environments without further enhancements and security measures.
- Feel free to modify and extend the server code to suit your requirements. Understanding how it works can provide valuable insights into web server technology and network programming in C.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).

## Contact

For any questions or inquiries, please contact [valeriotriolo3010@gmail.com](mailto:valeriotriolo3010@gmail.com