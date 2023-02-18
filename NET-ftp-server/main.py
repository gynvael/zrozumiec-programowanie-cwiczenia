import argparse
from server_base import ServerThread
from server_ftp import AnonymusFtpServerThread


def _parse_args():
    parser = argparse.ArgumentParser(
        prog="Simple FTP Server",
        description="The program runs a simple FTP Server on specified port",
    )
    parser.add_argument(
        "-p",
        "--port",
        type=int,
        help="<Optional> Specify the port number for FTP Server (example python main.py -p 8888). Default value: 8000",
        default=8000,
        required=False,
    )
    parser.add_argument(
        "-hs",
        "--host",
        help="<Optional> Specify the host for FTP Server (example python main.py -h 127.0.0.1). Default value: 127.0.0.1",
        default="127.0.0.1",
        required=False,
    )
    parser.add_argument(
        "-r",
        "--root",
        help="<Optional> Specify the root directory for FTP Server (example python main.py -r asdf). Default value: root_dir",
        default="root_dir",
        required=False,
    )
    return parser.parse_args()


args = _parse_args()

BASE = {
    "host": args.host,
    "port": args.port,
    "server_type": AnonymusFtpServerThread,
}
FTP = {"root_dir": args.root}


if __name__ == "__main__":
    ftp = ServerThread(
        host=BASE.get("host"),
        port=BASE.get("port"),
        server_type=BASE.get("server_type"),
        server_config=FTP,
    )
    print(f"Server is listening on {ftp.host}:{ftp.port}")
    ftp.daemon = True
    ftp.start()
    input("Press enter to stop ...")
    ftp.stop()
    print("Server stopped")
