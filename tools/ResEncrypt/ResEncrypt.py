#!/usr/bin/python
# encoding=utf-8
# 对 lua 代码进行加密和压缩

import os
import sys
import time
import json
import shutil

sys.path.append(os.path.normpath(os.path.join(os.path.dirname(__file__), '../utils')))

import utils
from logUtils import Logging
from logUtils import raise_known_error
from logUtils import KnownError

ENCRYPT_CFG = {
    ".lua" : ".wld",
    ".png" : ".png",
    ".jpg" : ".jpg",
    ".jpeg" : ".jpeg"
}

class ResEncrypt(object):

    def __init__(self, src, dst, rm_src, exclude_cfg, do_copy, quiet=False):
        self.src_path = utils.flat_path(src)
        if not os.path.exists(self.src_path):
            raise_known_error('%s 不存在' % self.src_path, KnownError.ERROR_WRONG_ARGS)

        if not dst:
            if os.path.isdir(src):
                self.dst_path = self.src_path
            else:
                self.dst_path = os.path.dirname(self.src_path)
        else:
            self.dst_path = utils.flat_path(dst)

        if os.path.isfile(self.dst_path):
            raise_known_error('-d 参数必须为文件夹', KnownError.ERROR_WRONG_ARGS)

        self.dst_is_src = (os.path.isdir(self.src_path) and self.src_path == self.dst_path )

        self.rm_src = rm_src
        self.do_copy = do_copy
        self.quiet = quiet
        if exclude_cfg is None:
            self.exclude_cfg = None
        elif isinstance(exclude_cfg, list):
            self.exclude_cfg = self.convert_rules(exclude_cfg)
        else:
            self.exclude_cfg = self.parse_exclude_cfg(exclude_cfg)

    def convert_rules(self, rules):
        ret_rules = []
        for rule in rules:
            ret = rule.replace('.', '\\.')
            ret = ret.replace('*', '.*')
            ret = "%s" % ret
            ret_rules.append(ret)

        return ret_rules

    def parse_exclude_cfg(self, cfg_file):
        cfg_full_path = utils.flat_path(cfg_file)
        if not os.path.isfile(cfg_full_path):
            raise_known_error("%s 文件不存在" % cfg_full_path, KnownError.ERROR_PATH_NOT_FOUND)

        ret = None
        try:
            f = open(cfg_full_path)
            ret = json.load(f)
            f.close()

            ret = self.convert_rules(ret)
        except:
            raise_known_error('解析 %s 失败' % cfg_full_path, KnownError.ERROR_PARSE_FILE)

        return ret

    def is_exclude(self, file_path):
        if self.exclude_cfg is None:
            return False

        import re
        rel_path = os.path.relpath(file_path, self.src_path)
        path_str = rel_path.replace("\\", "/")
        for rule in self.exclude_cfg:
            if re.match(rule, path_str):
                return True

        return False

    def output_info(self, msg):
        if not self.quiet:
            Logging.debug_msg(msg)

    def encrypt_file(self, src_path, dst_path):
        if not os.path.isdir(dst_path):
            os.makedirs(dst_path)

        if self.is_exclude(src_path):
            self.output_info('%s 文件被排除!' % src_path)
            if not self.dst_is_src:
                # 如果文件被排除，而且源路径与目标路径不同，那么需要将源文件拷贝到目标路径
                shutil.copy(src_path, os.path.join(dst_path, os.path.basename(src_path)))
            return

        basename, ext = os.path.splitext(os.path.basename(src_path))
        if os.path.basename(src_path) == 'manifest':
            dst_ext = '.wld'
        else:
            dst_ext = ENCRYPT_CFG[ext]
        dst_file_path = os.path.join(dst_path, '%s%s' % (basename, dst_ext))
        self.output_info('Encrypt %s to %s' % (src_path, dst_file_path))

        if src_path.lower() == dst_file_path.lower():
            # 源路径与目标路径一致，需要使用临时文件
            encrypt_file_path = '%s_temp' % dst_file_path
        else:
            encrypt_file_path = dst_file_path

        cur_path = os.path.dirname(__file__)
        if utils.os_is_win32():
            cmd_tool_path = utils.flat_path(os.path.join(cur_path, 'bin/encryptor.exe'))
        else:
            cmd_tool_path = utils.flat_path(os.path.join(cur_path, 'bin/encryptor'))

        encrypt_cmd = '%s "%s" "%s"' % (cmd_tool_path, src_path, encrypt_file_path)
        utils.run_shell(encrypt_cmd, quiet=True)

        if encrypt_file_path != dst_file_path:
            # 是生成的临时文件，需要替换
            os.remove(src_path)
            os.rename(encrypt_file_path, dst_file_path)

        if self.rm_src and (src_path.lower() != dst_file_path.lower()):
            os.remove(src_path)

    def do_encrypt(self):
        if os.path.isfile(self.src_path):
            self.encrypt_file(self.src_path, self.dst_path)
        else:
            for (parent, subdirs, files) in os.walk(self.src_path):
                dst_path = utils.flat_path(os.path.join(self.dst_path, os.path.relpath(parent, self.src_path)))
                for f in files:
                    if f == '.DS_Store':
                        continue

                    full_path = utils.flat_path(os.path.join(parent, f))
                    basename, ext = os.path.splitext(full_path)
                    if ext.lower() in ENCRYPT_CFG.keys():
                        self.encrypt_file(full_path, dst_path)
                    elif f == 'manifest':
                        self.encrypt_file(full_path, dst_path)
                    elif self.do_copy and not self.dst_is_src:
                        # 需要拷贝非 lua 文件
                        if not os.path.isdir(dst_path):
                            os.makedirs(dst_path)
                        self.output_info('%s 文件被拷贝' % full_path)
                        shutil.copy(full_path, os.path.join(dst_path, os.path.basename(full_path)))

if __name__ == "__main__":
    from argparse import ArgumentParser
    parser = ArgumentParser(prog="ResEncrypt", description=utils.get_sys_encode_str("对资源文件进行加密和压缩"))

    parser.add_argument("-s", "--src", dest="src", required=True, help=utils.get_sys_encode_str("指定源文件或者文件夹。"))
    parser.add_argument("-d", "--dst", dest="dst", help=utils.get_sys_encode_str("指定目标路径。默认与源文件位置相同。"))
    parser.add_argument("--rm-src", dest="rm_src", action="store_true", help=utils.get_sys_encode_str("删除原始的文件"))
    parser.add_argument("--copy", dest="do_copy", action="store_true", help=utils.get_sys_encode_str("指定拷贝非加密文件"))
    parser.add_argument("--cfg", dest="exclude_cfg", help=utils.get_sys_encode_str("指定一个配置文件，这个文件用于配置排除某些资源文件"))

    (args, unknown) = parser.parse_known_args()

    # record the start time
    begin_time = time.time()
    try:
        if len(unknown) > 0:
            raise_known_error('未知参数 : %s' % unknown, KnownError.ERROR_WRONG_ARGS)

        encryptor = ResEncrypt(args.src, args.dst, args.rm_src, args.exclude_cfg, args.do_copy)
        encryptor.do_encrypt()
    except KnownError as e:
        # a known exception, exit with the known error number
        sys.exit(e.get_error_no())
    except Exception:
        raise
    finally:
        # output the spend time
        end_time = time.time()
        Logging.log_msg('\n总共用时： %.2f 秒\n' % (end_time - begin_time))
