import os
import time
import uuid
import socket
import random
import datetime
from pathlib import Path
from datetime import date

from server_base import ServerThread
from exceptions import *

ALLOWED_COMMANDS = ["ABOR", "ACCT", "ALLO", "APPE", "CWD", "DELE", "HELP", "LIST", "MODE", "NLST",
                    "NOOP", "PASS", "PASV", "PORT", "QUIT", "REIN", "REST", "RETR", "RNFR", "RNTO",
                    "SITE", "STAT", "STOR", "STRU", "TYPE", "USER", "CDUP", "MKD", "PWD", "RMD",
                    "SMNT", "STOU", "SYST", "FEAT"]


class AnonymusFtpServerThread(ServerThread):
    f"""
    Anonymus FTP server. Allowed commands list: {', '.join(ALLOWED_COMMANDS)}
    """

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.root = os.path.abspath(kwargs.get("root_dir"))
        self.cwd = self.root
        self.datasocket = None
        self.serversocket = None
        self.pasv_mode = False
        self.type = "A"
        self.crlf = "\r\n"
        self.bcrlf = b"\r\n"

    def run(self):
        self._ftp_response(220, "Welcome!")
        while True:
            msg = self.recvuntil(self.bcrlf)
            cmd, arg = msg[:4].strip(), msg[4:].strip()
            print(f'[{datetime.datetime.now().strftime("%m-%d-%Y %H:%M:%S")}] {msg}')
            if not cmd:
                break
            if cmd not in ALLOWED_COMMANDS:
                self._ftp_response(500, "Bad command or not implemented")
                continue
            if cmd == "HELP":
                self.sendall(self.HELP(arg))
                continue
            try:
                method = getattr(self, cmd)
                status, message = method(arg)
                self._ftp_response(status, message)
            except FTPException as ftp_e:
                self._ftp_response(*ftp_e.response)
            except Exception as e:
                print(f"Got exception {e}")
                self._ftp_response(500, "Bad command or not implemented")

    def _ftp_response(self, status, message):
        msg = f"{status} {message} {self.crlf}"
        self.sock.sendall(msg.encode())

    def _start_datasock(self):
        if self.pasv_mode:
            self.datasocket, _ = self.serversocket.accept()
        else:
            self.datasocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.datasocket.connect((self.data_addr, self.data_port))

    def _stop_datasock(self):
        self.datasocket.close()
        self.datasocket = None
        if self.pasv_mode:
            self.serversocket.close()
            self.serversocket = None

    def _trim_anchor(self, path):
        anchor = Path(path).anchor
        return path.lstrip(anchor)

    def _is_name_valid(self, name):
        trimmed = self._trim_anchor(name)
        result = os.path.realpath(os.path.join(self.root, trimmed))
        common = os.path.commonpath([self.root, result])
        return self.root in common

    def _get_entry_info(self, entry):
        stats = entry.stat()
        type = "d" if entry.is_dir() else "-"
        base = "rwxrwxrwx"
        links = stats.st_nlink
        owner = "anonymus"
        group = "anonymus"
        size = stats.st_size
        modified_date = date.fromtimestamp(stats.st_mtime)
        date_format = "%b %d %H:%m"
        modified = modified_date.strftime(date_format)
        name = entry.name
        return f"{type}{base} {links} {owner} {group} {size} {modified}\t{name}"

    def _create_or_append(self, mode, filename):
        if not self._is_name_valid(filename):
            return False
        self._ftp_response(150, "Opening data connection")
        self._start_datasock()
        new_file = self.recvuntil(self.bcrlf, self.datasocket)
        if self.type == "I":
            new_file = new_file.encode()
        self._stop_datasock()
        path = os.path.join(self.cwd, filename)
        try:
            with open(path, mode) as f:
                f.write(new_file)
        except Exception as e:
            print(e)
            return False
        return True

    # FTP API Commands

    def ABOR(self, arg=None):
        """
        The ABOR command can be issued by the client to abort the previous FTP command.
        If the previous FTP command is still in progress or has already completed, the server will terminate its
        execution and close any associated data connection. This command does not cause the connection to close.
        """
        if self.datasocket:
            self._stop_datasock()
            return 226, "Closing data connection."
        return 225, "Data connection open, no transfer in progress."

    def ACCT(self, arg=None):
        """
        The ACCT command is used to provide an account name to the server.
        The account is not necessarily related to the USER .
        A client may log in using their username and password, however, an account may be required for
        specific access to server resources.
        """
        raise CommandNotImplemented

    def RNFR(self, arg=None):
        """
        The RNFR command is issued when an FTP client wants to rename a file on the server.
        The client specifies the name of the file to be renamed along with the command.
        After issuing an RNFR command, an RNTO command must immediately follow.
        """
        if not self._is_name_valid(arg):
            raise WrongFilename
        file_path = os.path.abspath(os.path.join(self.cwd, arg))
        if not Path(file_path).exists():
            raise FileNotExists
        self.filename_cache = arg
        return 350, "File ready to rename"

    def RNTO(self, arg=None):
        """
        The RNTO command is used to specify the new name of a file specified in a preceding RNFR (Rename From) command.
        """
        if not self._is_name_valid(arg):
            raise WrongFilename
        old_filename_path = os.path.abspath(os.path.join(self.cwd, self.filename_cache))
        new_filename_path = os.path.abspath(os.path.join(self.cwd, arg))
        try:
            os.rename(old_filename_path, new_filename_path)
        except Exception as e:
            raise FailedRequest
        return 250, f"File renamed to {arg}"

    def RETR(self, arg=None):
        """
        A client issues the RETR command after successfully establishing a data connection
        when it wishes to download a copy of a file on the server. The client provides the file name
        it wishes to download along with the RETR command. The server will send a copy of the file to the client.
        This command does not affect the contents of the server’s copy of the file.
        """
        if not self._is_name_valid(arg):
            raise WrongFilename
        file_path = os.path.abspath(os.path.join(self.cwd, arg))
        if not Path(file_path).exists():
            raise FileNotExists
        self._ftp_response(150, "Sending file")
        self._start_datasock()
        with open(file_path, "r") as f:
            data = f.read()
        self.datasocket.send(f"{data}{self.crlf}".encode())
        self._stop_datasock()
        return 250, "OK."

    def STRU(self, arg=None):
        """
        The STRU command is issued with a single Telnet character parameter that specifies a file structure
        for the server to use for file transfers.
        The following codes are assigned for structure:
        F - File (no record structure)
        R - Record structure (Not implemented)
        P - Page structure (Not implemented)
        """
        if arg == "F":
            return 200, "OK."
        raise CommandNotImplemented

    def DELE(self, arg=None):
        """
        The DELE command is used to delete the specified file from the server.
        To delete a directory, use the RMD command.
        """
        if not self._is_name_valid(arg):
            raise WrongFilename
        if not Path(os.path.join(self.cwd, arg)).exists():
            raise FileNotExists
        os.remove(os.path.join(self.cwd, arg))
        return 250, "Deleted"

    def NOOP(self, arg=None):
        """
        The NOOP command does not cause the server to perform any action
        beyond acknowledging the receipt of the command.
        """
        return 200, "OK."

    def ALLO(self, arg=None):
        """
        The ALLO command may be sent to a server that requires the necessary space for an uploaded to be reserved
        before the transfer takes place. The argument shall be a decimal integer representing the number of bytes
        (using the logical byte size) of storage to be reserved for the file.
        A server that does not require space to be reserved in advance should treat the command as a NOOP operation.
        """
        return 200, "OK."

    def SMNT(self, arg=None):
        """
        The SMNT command allows the user to mount a different file system data structure without altering their
        login or accounting information. Transfer parameters are similarly unchanged.
        The argument is a path name specifying a directory or other system dependent file group designator.
        """
        raise CommandNotImplemented

    def SITE(self, arg=None):
        """
        The SITE command is used by the server to provide services to a client that may not be universally
        supported by all FTP clients. These commands are commonly server specific implementations provided
        to allow for additional functionality to FTP clients choosing to implement the command as well.
        The SITE command is followed by the extended command and any additional parameters
        """
        raise CommandNotImplemented

    def REST(self, arg=None):
        """
        The REST command is used to specify a marker to the server for the purposes of resuming a file transfer.
        Issuing the REST command does not actually initiate the transfer. After issuing a REST command,
        the client must send the appropriate FTP command to transfer the file. The server will use the marker
        specified in the REST command to resume file transfer at the specified point.
        """
        if not isinstance(arg, int):
            raise WrongAttribute
        raise CommandNotImplemented

    def STAT(self, arg=None):
        """
        If the STAT command is issued during a file transfer, information about the current
        file transfer is sent to the client. If a path name is provided with the command, it is analogous
        to the LIST command except for the file information for the specified pathname is sent over the command
        connection instead of a data connection. A partial pathname can be provided instead, in which case the
        server will respond with a list of file names or attributes associated with that specification.
        If no parameter is provided and a file transfer is not in progress, the server will respond with general
        status information about the FTP server and the current connection.
        """
        raise CommandNotImplemented

    def REIN(self, arg=None):
        """
        The REIN resets the FTP connection to the state it is in when the client first connects to the FTP server.
        Any file transfers in progress when the REIN command is sent are allowed to finish.
        The server should reset all parameters to their default states and flush all I/O and previous
        account information. The command connection remains open and a USER command may be expected to follow.
        """
        while self.datasocket or self.serversocket:
            time.sleep(1)
        self.cwd = self.root
        self.pasv_mode = False
        self.type = "A"
        return 220, "Ready.."

    def TYPE(self, arg=None):
        """
        The TYPE command is issued to inform the server of the type of data that is being transferred by the client.
        Most modern Windows FTP clients deal only with type A (ASCII) and type I (image/binary).
        """
        if arg == "A":
            self.type = arg
            return 200, "OK."
        if arg == "I":
            self.type = arg
            return 200, "OK."
        raise CommandNotImplemented

    def PORT(self, arg=None):
        """
        The PORT command is issued by the client to initiate a data connection required to transfer data
        (such as directory listings or files) between the client and server.
        This command is used during “active” mode transfers.
        """
        host_port = arg.split(",")
        self.data_addr = ".".join(host_port[:4])
        self.data_port = (int(host_port[4]) << 8) + int(host_port[5])
        return 200, "Get port."

    def SYST(self, arg=None):
        """
        A client can issue this command to the server to determine the operating system running on the server.
        Not all server responses are accurate in this regard, however, as some servers respond with the system they
        emulate or may not respond at all due to potential security risks.
        """
        syst_name = random.choice(["ASP", "AUGUST", "BKY", "CCP", "DOS/360", "ELF", "EPOS", "EXEC-8", "GCOS", "GPOS",
                                   "ITS", "INTERCOM", "INTERLISP", "KRONOS", "MCP", "MOS", "MPX-RT", "MULTICS", "MVT",
                                   "NOS", "NOS/BE", "OS/MVS", "OS/MVT", "RIG", "RSX-11M", "RT11", "SCOPE", "SIGNAL",
                                   "SINTRAN", "TAC", "TENEX", "TOPS-10", "TOPS-20", "TSS", "UNIX", "VM/370", "VM/CMS",
                                   "VMS", "WAITS", "XDE"])

        return 215, syst_name

    def STOR(self, arg=None):
        """
        A client issues the STOR command after successfully establishing a data connection when it wishes to upload
        a copy of a local file to the server. The client provides the file name it wishes to use for the upload.
        If the file already exists on the server, it is replaced by the uploaded file.
        If the file does not exist, it is created.
        This command does not affect the contents of the client's local copy of the file.
        """
        if self.type == "A":
            mode = "w"
        else:
            mode = "wb"
        try:
            self._create_or_append(mode=mode, filename=arg)
        except Exception as e:
            raise FailedRequest
        return 250, "File created"

    def STOU(self, arg=None):
        """
        Similar to the STOR command, the STOU command is issued after successfully establishing a data connection
        to transfer a local file to the server. A file name is provided for the server to use.
        If the file does not exist on the server, it is created. If the file already exists, it is not overwritten.
        Instead, the server creates a unique file name and creates it for the transferred file.
        The response by the server will contain the created file name.
        """
        if self.type == "A":
            mode = "w"
        else:
            mode = "wb"
        if not self._is_name_valid(arg):
            raise WrongFilename
        uniq = False
        if os.path.exists(os.path.join(self.cwd, arg)):
            arg = str(uuid.uuid4())
            uniq = True
        file_created = self._create_or_append(mode=mode, filename=arg)
        if file_created and not uniq:
            return 226, "Closing data connection"
        if file_created and uniq:
            return 226, arg
        raise FailedRequest

    def APPE(self, arg=None):
        """
        A client issue the APPE command after successfully establishing a data connection
        when it wishes to upload data to the server. The client provides the file name it wishes to use for the upload.
        If the file already exists on the server, the data is appended to the existing file.
        If the file does not exist, it is created.
        """
        if self._create_or_append(mode="a", filename=arg):
            return 226, "Closing data connection"
        raise FailedRequest

    def FEAT(self, arg=None):
        """
        The FEAT command provides FTP clients with a mechanism of quickly determining
        what extended features the FTP server supports. If this command is supported, the server
        will reply with a multi-line response where each line of the response contains an extended feature command
        supported by the server.
        """
        raise CommandNotImplemented

    def USER(self, arg=None):
        """
        USER is followed by a text string identifying the user.
        The user identification is that which is required by the server for access to its file system.
        This command will normally be the first command transmitted by the user after the control connections are made.
        """
        return 230, "OK."

    def PASS(self, arg=None):
        """
        A string specifying the user's password follows the PASS command. This command must be immediately
        preceded by the user name command and, for most sites, completes the user's identification for access control.
        """
        return 230, "OK."

    def QUIT(self, arg=None):
        """
        Close an ftp connection to a remote system by using the bye command.
        """
        return 221, "Goodbye"

    def CDUP(self, arg=None):
        """
        The CDUP command causes the server to change the client's current working directory to the
        immediate parent directory of the current working directory.
        """
        if self.cwd == self.root:
            raise WrongAttribute
        self.cwd = os.path.abspath(os.path.join(self.cwd, ".."))
        return 230, "OK."

    def PWD(self, arg=None):
        """
        This command displays the current working directory on the server for the logged in user.
        """
        cwd = os.path.relpath(self.cwd, self.root)
        return 257, f'"/{cwd}"'

    def MKD(self, arg=None):
        """
        This command causes the directory specified in the pathname to be created on the server.
        If the specified directory is a relative directory, it is created in the client’s current working directory.
        """
        if not self._is_name_valid(arg):
            raise WrongAttribute
        try:
            os.mkdir(os.path.join(self.cwd, arg))
        except FileExistsError:
            raise DirectoryExists
        return 250, "Created"

    def RMD(self, arg=None):
        """
        This command causes the directory specified in the path name to be removed.
        If a relative path is provided, the server assumes the specified directory to be a subdirectory of the
        client's current working directory. To delete a file, the DELE command is used.
        """
        if not self._is_name_valid(arg):
            raise WrongFilename
        if not os.path.exists(os.path.join(self.cwd, arg)):
            raise FileNotExists
        if len(os.listdir(os.path.join(self.cwd, arg))):
            raise DirectoryNotEmpty
        try:
            os.rmdir(os.path.join(self.cwd, arg))
        except Exception:
            raise FailedRequest
        return 250, "Removed"

    def CWD(self, arg=None):
        """
        The CWD command is issued to change the client's current working directory
        to the path specified with the command. FTP Voyager and other GUI-based FTP clients will automatically
        issue these commands as the user browses the remote file system from within the program.
        """
        if not self._is_name_valid(arg):
            raise WrongFilename
        arg_tr = self._trim_anchor(arg)
        location = Path.joinpath(Path(self.root), Path(arg_tr))
        if not location.exists():
            raise FileNotExists
        self.cwd = location.absolute()
        return 250, "OK."

    def LIST(self, arg=None):
        """
        The LIST command is issued to transfer information about files on the server through a
        previously established data connection. When no argument is provided with the LIST command,
        the server will send information on the files in the current working directory.
        If the argument specifies a directory or other group of files, the server should transfer
        a list of files in the specified directory. If the argument specifies a file, then the server
        should send current information about the file.
        """
        if arg and not self._is_name_valid(arg):
            raise WrongFilename
        path = arg if arg else self.cwd
        self._ftp_response(150, "Directory listing")
        entry = Path(path)
        info = []
        if entry.is_dir():
            for e in entry.iterdir():
                info.append(self._get_entry_info(e))
        else:
            info.append(self._get_entry_info(entry))
        self._start_datasock()
        for i in info:
            self.datasocket.send(f"{i}{self.crlf}".encode())
        self._stop_datasock()
        return 226, "Directory send OK."

    def NLST(self, arg=None):
        """
        The NLST command is used to retrieve a list of files from the server over a previously
        established data connection. Unlike the LIST command, the server will send only the list of files and no
        other information on those files. If a parameter is specified, the server returns the list
        of files contained in the provided path. If no parameter is present, the server uses
        the client’s current working directory.
        """
        return self.LIST(arg)

    def HELP(self, arg=None):
        """
        Print an informative message about the meaning of command. If no argument is given,
        ftp prints a list of the known commands.
        """
        if arg in ALLOWED_COMMANDS:
            method = getattr(self, arg)
            return method.__doc__.replace("\n", " ") + self.crlf
        return ", ".join(ALLOWED_COMMANDS) + self.crlf

    def PASV(self, arg=None):
        """
        There are two ways to transfer data in FTP communications, active ( PORT ) and PASV .
        PASV command changes FTP communication into the passive mode.
        """
        self.pasv_mode = True
        self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.serversocket.bind((self.host, 0))
        self.serversocket.listen(1)
        self.data_addr, self.data_port = self.serversocket.getsockname()
        host_ip = self.data_addr.replace(".", ",")
        port = f"{self.data_port >> 8 & 0xFF},{self.data_port & 0xFF}"
        return 227, f"Entering Passive Mode ({host_ip},{port})"
