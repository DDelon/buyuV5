#!/usr/bin/python
#-*- coding: utf-8 -*-

import os
import sys
import subprocess
import locale
import zipfile
import tempfile
import excopy
import shutil

from logUtils import Logging
from logUtils import raise_known_error
from logUtils import KnownError

def flat_path(p):
    return os.path.normpath(os.path.abspath(os.path.expanduser(p)))

def run_shell(cmd, cwd=None, quiet=False):
    if not quiet:
        Logging.log_msg('Running command: %s\n' % cmd)

    p = subprocess.Popen(cmd, shell=True, cwd=cwd)
    p.wait()

    if p.returncode:
        raise_known_error('Command %s failed' % cmd, p.returncode)

    return p.returncode

def os_is_win32():
    return sys.platform == 'win32'

def os_is_mac():
    return sys.platform == 'darwin'

def os_is_32bit_windows():
    if not os_is_win32():
        return False

    arch = os.environ['PROCESSOR_ARCHITECTURE'].lower()
    archw = "PROCESSOR_ARCHITEW6432" in os.environ
    return (arch == "x86" and not archw)

def non_unicode_str(str):
    if isinstance(str, unicode):
        str = str.encode('utf-8')
    return str

def get_sys_encode_str(s):
    encoding = 'utf-8'
    try:
        lang, encoding = locale.getdefaultlocale()
        if encoding is None:
            encoding = 'utf-8'
    except:
        pass

    if encoding.lower() != 'utf-8' and not isinstance(s, unicode):
        s = s.decode('utf-8')

    if isinstance(s, unicode):
        s = s.encode(encoding)

    return s

def zip_folder(folder_path, zip_file_path):
    zip_parent = os.path.dirname(zip_file_path)
    if not os.path.isdir(zip_parent):
        os.makedirs(zip_parent)

    f_zip = zipfile.ZipFile(zip_file_path, 'w', zipfile.ZIP_DEFLATED)
    for parent, dirs, files in os.walk(folder_path):
        # add directory (needed for empty dirs)
        for file in files:
            filename = os.path.join(parent, file)
            f_zip.write(filename, os.path.relpath(filename, folder_path))
    f_zip.close()

def check_environment_variable(var):
    ''' Checking the environment variable, if found then return it's value, else raise error
    '''
    value = None
    try:
        value = os.environ[var]
    except Exception:
        raise_known_error("环境变量 %s 不存在" % var, KnownError.ERROR_ENV_VAR_NOT_FOUND)

    return value

def get_support_brands():
    return ["jixiang", "weile"]

def convert_rules(rules):
    ret_rules = []
    for rule in rules:
        ret = rule.replace('.', '\\.')
        ret = ret.replace('*', '.*')
        ret = "%s" % ret
        ret_rules.append(ret)

    return ret_rules

def parse_rules_cfg(cfg_file):
    cfg_full_path = flat_path(cfg_file)
    if not os.path.isfile(cfg_full_path):
        raise_known_error("%s 文件不存在" % cfg_full_path, KnownError.ERROR_PATH_NOT_FOUND)

    ret = None
    try:
        import json
        f = open(cfg_full_path)
        ret = json.load(f)
        f.close()

        ret = convert_rules(ret)
    except:
        raise_known_error('解析 %s 失败' % cfg_full_path, KnownError.ERROR_PARSE_FILE)

    return ret

def is_in_rule(file_path, root_path, rules):
    if rules is None:
        return False

    import re
    rel_path = os.path.relpath(file_path, root_path)
    path_str = rel_path.replace("\\", "/")
    for rule in rules:
        if re.match(rule, path_str):
            return True

    return False

NATIVE_INFO_FMT = '''
return { 
	version=%d
}
'''
NATIVE_VERSION = 1
def write_native_info(file_path):
    f = open(file_path, 'w')
    f.write(NATIVE_INFO_FMT % NATIVE_VERSION)
    f.close()

class FileRollback(object):

    def __init__(self):
        self.files = None
        self.dirs = None

    def record_folder(self, folder):
        if not os.path.isdir(folder):
            return

        if self.dirs is None:
            self.dirs = {}

        if self.dirs.has_key(folder):
            # 已经记录过了，不再重复记录
            return

        # 创建临时文件夹来备份文件
        temp_path = tempfile.mkdtemp()
        copy_cfg = {
            "from": ".",
            "to": "."
        }
        excopy.copy_files_with_config(copy_cfg, folder, temp_path)

        # 记录备份路径
        self.dirs[folder] = temp_path

    def record_file(self, file_path):
        if not os.path.isfile(file_path):
            return

        if self.files is None:
            self.files = {}

        if self.files.has_key(file_path):
            # 已经记录过了，不再重复记录
            return

        # 创建临时文件夹来备份文件
        temp_path = tempfile.mkdtemp()
        shutil.copy2(file_path, temp_path)
        self.files[file_path] = os.path.join(temp_path, os.path.basename(file_path))

    def do_rollback(self):
        # 先恢复文件夹
        if self.dirs is not None:
            # rollback dirs
            for dir in self.dirs.keys():
                # 先移除现有的文件夹
                if os.path.isdir(dir):
                    shutil.rmtree(dir)
                os.makedirs(dir)

                # 将文件恢复
                copy_cfg = {
                    "from": ".",
                    "to": "."
                }
                excopy.copy_files_with_config(copy_cfg, self.dirs[dir], dir)

                # 移除临时文件夹
                shutil.rmtree(self.dirs[dir])

            # reset data
            self.dirs = None

        # 再恢复文件
        if self.files is not None:
            # rollback files
            for file_path in self.files.keys():
                dir = os.path.dirname(file_path)
                if not os.path.isdir(dir):
                    os.makedirs(dir)

                target_dir = os.path.dirname(file_path)
                shutil.copy2(self.files[file_path], target_dir)

                # 移除临时文件夹
                shutil.rmtree(os.path.dirname(self.files[file_path]))

            # reset data
            self.files = None

if __name__ == "__main__":
    # test rollback
    rollback = FileRollback()

    # rollback.record_file('/Users/bill/Work/test/FileRollback/gen_sha1.py')
    # rollback.record_folder('/Users/bill/Work/test/FileRollback/138')
    #
    # # delete files & dirs
    # shutil.rmtree('/Users/bill/Work/test/FileRollback/138')
    # os.remove('/Users/bill/Work/test/FileRollback/gen_sha1.py')
    #
    # os.makedirs('/Users/bill/Work/test/FileRollback/138')
    # f = open('/Users/bill/Work/test/FileRollback/138/test.txt', 'w')
    # f.write("asdadasdasdasdsa")
    # f.close()

    # do rollback
    rollback.do_rollback()
