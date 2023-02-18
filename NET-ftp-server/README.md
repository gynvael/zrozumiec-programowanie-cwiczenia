
## Simple FTP server
This is very simple FTP server written in python 3 without any external libraries except `pytest` for testing puroses.
The task to create it came from the book "Zrozumieć Programowanie" by Gynvael Coldwind. The server implements most of
the mandatory FTP commands specified in RFC 5797. 

## Using FTP server locally
For local usage you need to run `main.py` script with optional settings:
```
-h, --help              show this help message and exit
-p PORT, --port PORT    <Optional> Specify the port number for FTP Server
                        (example python main.py -p 8888). Default value: 8000
-hs HOST, --host HOST   <Optional> Specify the host for FTP Server 
                        (example python main.py -h 127.0.0.1). Default value: 127.0.0.1
-r ROOT, --root ROOT    <Optional> Specify the root directory for FTP Server 
                        (example python main.py -r asdf). Default value: root_dir
```
or run with default settings:
```
python main.py
```


## Contact

* [@michal-siedlecki](https://github.com/michal-siedlecki) 😎 [author]


## License

This project uses the following license: MIT (<https://github.com/michal-siedlecki/ftp-python-sock/blob/master/LICENSE>).
