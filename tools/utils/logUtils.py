#!/usr/bin/python
# encoding=utf-8

__author__ = 'lengaoxin@gmail.com'

import os
import multiprocessing
import time
import sys
import locale

REPORT_LOG_ENABLED = False

def get_current_path():
    if getattr(sys, 'frozen', None):
        ret = os.path.realpath(os.path.dirname(sys.executable))
    else:
        ret = os.path.realpath(os.path.dirname(__file__))

    return ret

log_file_lock = multiprocessing.Lock()
LOG_FILE = 'report.log'
def report_log(msg, mode='a'):
    if not REPORT_LOG_ENABLED:
        return

    log_file_lock.acquire()
    folder = get_current_path()
    log_file_path = os.path.join(folder, LOG_FILE)
    f = None
    try:
        log_time = time.strftime('%Y-%m-%d %H:%M:%S')
        f = open(log_file_path, mode)
        f.write('%s %s\n' % (log_time, msg))
        f.close()
    except:
        if f:
            f.close()
    log_file_lock.release()

class Logging(object):
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    RESET = '\033[0m'
    DEBUG_ENABLED = True

    @staticmethod
    def _print(s, color=None):
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

        if color and sys.stdout.isatty() and sys.platform != 'win32':
            print(color + s + Logging.RESET)
        else:
            print(s)

    @classmethod
    def log_msg(cls, msg):
        Logging._print(msg, Logging.GREEN)

    @classmethod
    def debug_msg(cls, msg):
        if cls.DEBUG_ENABLED:
            Logging._print(msg)

    @classmethod
    def warn_msg(cls, msg):
        warn_str = 'WARN : %s' % msg
        Logging._print(warn_str, Logging.YELLOW)

    @classmethod
    def error_msg(cls, msg, err_no=1):
        error_str = 'ERROR (%d): %s' % (err_no, msg)
        Logging._print(error_str, Logging.RED)
        report_log(error_str)

def raise_known_error(err_msg, err_no=1):
    Logging.error_msg(err_msg, err_no)
    raise KnownError(err_msg, err_no)

class KnownError(Exception):
    ERROR_WRONG_ARGS = 11           # wrong arguments
    ERROR_PATH_NOT_FOUND = 12       # path not found
    ERROR_BUILD_FAILED = 13         # build failed
    ERROR_RUNNING_CMD = 14          # error when running command
    ERROR_CMD_NOT_FOUND = 15        # command not found
    ERROR_ENV_VAR_NOT_FOUND = 16    # environment variable not found
    ERROR_TOOLS_NOT_FOUND = 17      # depend on tools not found
    ERROR_PARSE_FILE = 18           # error when parse files
    ERROR_WRONG_CONFIG = 19         # configuration is wrong

    ERROR_OTHERS = 101              # other errors

    def __init__(self, err_args, err_no=1):
        super(KnownError, self).__init__(err_args)
        self.error_no = err_no

    def get_error_no(self):
        return self.error_no
