import os
import ftplib


def test_connect(ftp_client):
    response = ftp_client.getwelcome()
    assert response == "220 Welcome! "


def test_user(ftp_client):
    ftp_client.login(user="test_user")
    response = ftp_client.lastresp
    assert response == "230"


def test_cwd(ftp_client, get_test_dirs):
    dirs = get_test_dirs
    ftp_client.cwd(dirs.get("nested"))
    response = ftp_client.lastresp
    assert response == "250"


def test_cwd_pwd(ftp_client, get_test_dirs):
    dirs = get_test_dirs
    nested = dirs.get("nested")
    ftp_client.cwd(nested)
    pwd = ftp_client.pwd()
    response = ftp_client.lastresp
    assert response == "257"
    assert pwd == f"/{nested}"


def test_cwd_cant_escape(ftp_client, escape_path):
    try:
        ftp_client.cwd(escape_path)
    except ftplib.error_perm:
        assert ftp_client.lastresp == "553"


def test_cdup_cant_escape(ftp_client):
    try:
        ftp_client.cwd("..")
    except ftplib.error_perm:
        assert ftp_client.lastresp == "553"



def test_list(ftp_client, dirs):
    ls = ftp_client.nlst()
    root = dirs.get("root")
    assert len(ls) == len(os.listdir(root))


def test_mkd(ftp_client, dirs):
    new_dir_name = "new_directory"
    new_dir_path = os.path.join(dirs.get("root"), new_dir_name)
    if os.path.exists(new_dir_path):
        os.rmdir(new_dir_path)
    ftp_client.mkd(new_dir_name)
    exists = os.path.exists(new_dir_path)
    assert ftp_client.lastresp == "250"
    assert exists


def test_rmd(ftp_client, dirs):
    dir_to_remove = dirs.get("test_rm")
    exists = os.path.exists(os.path.join(dirs.get("root"), dir_to_remove))
    if not exists:
        os.mkdir(os.path.join(dirs.get("root"), dir_to_remove))
    ftp_client.rmd(dir_to_remove)
    exists = os.path.exists(os.path.join(dirs.get("root"), dir_to_remove))
    assert ftp_client.lastresp == "250"
    assert not exists


def test_rein(ftp_client):
    ftp_client.putcmd("REIN")
    assert "220" in ftp_client.getline()

def test_store_file(ftp_client, dirs):
    filename = 'test_file.txt'
    with open('big.txt', 'rb') as f:
        ftp_client.storbinary('STOR' + filename, f)
    exists = os.path.exists(os.path.join(dirs.get("root"), filename))
    assert ftp_client.lastresp == "250"
    assert exists

def test_store_file_escape(ftp_client, dirs, escape_path):
    filename = os.path.join(escape_path,'test_file.txt')
    try:
        with open('big.txt', 'rb') as f:
            ftp_client.storbinary('STOR' + filename, f)
    except ftplib.error_perm:
        exists = os.path.exists(os.path.join(dirs.get("root"), filename))
        assert ftp_client.lastresp == "501"
        assert not exists
