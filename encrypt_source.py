import os
import sys
import time
import shutil

sys.path.append(os.path.normpath(os.path.join(os.path.dirname(__file__), 'tools/ResEncrypt')))
import ResEncrypt

workspace_path = os.getcwd()+"/"

def encrypt_source(src, dst, rm_src, exclude_cfg, do_copy):
	encryptor = ResEncrypt.ResEncrypt(src, dst, rm_src, exclude_cfg, do_copy)
	encryptor.do_encrypt()

if __name__ == "__main__":
	# record the start time
	begin_time = time.time()
	try:
		if os.path.exists(workspace_path+"encrypt/res"):
			shutil.rmtree(workspace_path+"encrypt/res")
		if os.path.exists(workspace_path+"encrypt/src"):
			shutil.rmtree(workspace_path+"encrypt/src")
		encrypt_source(workspace_path+"res", workspace_path+"encrypt/res", None, "encrypt/encrypt_cfg_res.json", "--copy")
		encrypt_source(workspace_path+"src", workspace_path+"encrypt/src", None, "encrypt/encrypt_cfg_src.json", "--copy")
	except Exception:
		raise
	finally:
		# output the spend time
		end_time = time.time()