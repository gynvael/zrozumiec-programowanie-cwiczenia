class FTPException(Exception):
    response = None, None


class CommandNotImplemented(FTPException):
    response = 502, "Command not implemented"


class WrongFilename(FTPException):
    response = 533, "Wrong Filename"


class FileNotExists(FTPException):
    response = 553, "File not exists"


class DirectoryExists(FTPException):
    response = 553, "Directory already exists"


class DirectoryNotEmpty(FTPException):
    response = 553, "Directory is not empty"


class FailedRequest(FTPException):
    response = 451, "Requested action aborted: local error in processing"


class WrongAttribute(FTPException):
    response = 501, "Syntax error in parameters or argument"
