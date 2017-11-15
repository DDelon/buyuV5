import os
import sys
import time
import shutil
import zipfile
from filecmp import dircmp
from hashlib import sha1

sys.path.append(os.path.normpath(os.path.join(os.path.dirname(__file__), 'tools/ResEncrypt')))
import ResEncrypt

workspace_path = os.getcwd()+"/"
if sys.platform == 'win32':
	hot_file = "hot/win32/"
elif sys.platform == 'darwin':
	hot_file = "hot/ios/"
source_file = "source/"
platform_path = workspace_path+hot_file
temp_file = "temp/"
encrypt_file = "encrypt/"
source_path_1 = workspace_path+"hot/"+source_file
source_path_2 = platform_path+source_file
first_version_code = "1.4.1"
new_version_code = "1.4.1"
is_encrypt = True
encrypt_cfg_file = "hot/encrypt_cfg.json"
is_changed = True

def exit(code, text):
	if os.path.exists(platform_path+temp_file):
		shutil.rmtree(platform_path+temp_file)
	if text != None:
		raw_input(text)
	if code == None:
		sys.exit()
	else:
		sys.exit(code)

def replace_string_from_file(filename, key, value):
	fp=open(filename,"r+")
	if fp:
		s=fp.read()
		fp.seek(0,0)
		fp.write(s.replace(key,value))
		fp.close()

def create_version_source(version_code):
	if os.path.exists(platform_path+source_file) == False:
		os.makedirs(platform_path+source_file)
	if os.path.exists(platform_path+source_file+version_code+".zip") == False:
		if os.path.exists(platform_path+source_file+version_code) == False:
			shutil.copytree(source_path_1, platform_path+source_file+version_code)
		zip_version(source_path_2+version_code, platform_path+source_file+version_code+".zip")

def copy_new_version_file(version_code, str_path, filename):
	if os.path.exists(platform_path+temp_file+version_code) == False:
		os.makedirs(platform_path+temp_file+version_code)
	new_path = platform_path+temp_file+version_code+"/"+source_file+str_path[len(source_path_2+version_code):]
	if os.path.exists(new_path) == False:
		os.makedirs(new_path)
	shutil.copyfile(str_path+"/"+filename, new_path+"/"+filename)

def make_new_version(version_code, dcmp):
	for name in dcmp.diff_files:
		print "modify file %s found in %s and %s" % (name, dcmp.left,
			dcmp.right)
		copy_new_version_file(version_code, dcmp.right, name)
	for name in dcmp.left_only:
		print "remove file %s found in %s and %s" % (name, dcmp.left,
			dcmp.right)
	for name in dcmp.right_only:
		print "add file %s found in %s and %s" % (name, dcmp.left,
			dcmp.right)
		copy_new_version_file(version_code, dcmp.right, name)
	for sub_dcmp in dcmp.subdirs.values():
		make_new_version(version_code, sub_dcmp)

def encrypt_source(src, dst, rm_src, exclude_cfg, do_copy):
	encryptor = ResEncrypt.ResEncrypt(src, dst, rm_src, exclude_cfg, do_copy)
	encryptor.do_encrypt()

def zip_version(filepath, zip_path):
	if os.path.exists(filepath):
		zips = zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED)
		for dirPath, dirNames, fileNames in os.walk(filepath):
			for fileName in fileNames:
				fullPath = os.path.join(dirPath, fileName)
				localPath = fullPath[len(filepath):]
				zips.write(fullPath, localPath)
		zips.close()
		
def extract_zip(zip_path, filepath):
	if os.path.exists(zip_path):
		fz = zipfile.ZipFile(zip_path,'r')
		for file in fz.namelist():
			fz.extract(file,filepath)

def calc_sha1(filename):
	fp=open(filename,"rb")
	if fp:
		s=fp.read()
		fp.close()
		sha1obj = sha1()
		sha1obj.update(s)
		hash = sha1obj.hexdigest()
		return hash

def compare_version(version_code, last_version_code):
	dcmp = dircmp(platform_path+source_file+last_version_code, platform_path+source_file+version_code)
	make_new_version(version_code, dcmp)
		
def create_new_version(version_code, last_version_code):
	if os.path.exists(platform_path+temp_file+version_code+"/"+source_file):
		shutil.rmtree(platform_path+temp_file+version_code+"/"+source_file)
	if os.path.exists(platform_path+source_file+version_code) == False:
		extract_zip(platform_path+source_file+version_code+".zip", platform_path+source_file+version_code)
	if last_version_code == None:
		shutil.copytree(platform_path+source_file+version_code, platform_path+temp_file+version_code+"/"+source_file)
	else:
		if os.path.exists(platform_path+source_file+last_version_code) == False:
			extract_zip(platform_path+source_file+last_version_code+".zip", platform_path+source_file+last_version_code)
		compare_version(version_code, last_version_code)
		if os.path.exists(platform_path+temp_file+version_code+"/"+source_file) == False:
			is_changed = False
			return
	is_changed = True
	if is_encrypt == True:
		if os.path.exists(platform_path+temp_file+version_code+"/"+encrypt_file):
			shutil.rmtree(platform_path+temp_file+version_code+"/"+encrypt_file)
		encrypt_source(platform_path+temp_file+version_code+"/"+source_file, platform_path+temp_file+version_code+"/"+encrypt_file, None, encrypt_cfg_file, "--copy")
		shutil.rmtree(platform_path+temp_file+version_code+"/"+source_file)
		os.rename(platform_path+temp_file+version_code+"/"+encrypt_file, platform_path+temp_file+version_code+"/"+source_file)
	zip_version(platform_path+temp_file+version_code+"/"+source_file, platform_path+temp_file+version_code+"/tmp.zip")
	if os.path.exists(platform_path+temp_file+version_code+"/"+encrypt_file):
		shutil.rmtree(platform_path+temp_file+version_code+"/"+encrypt_file)
	shutil.rmtree(platform_path+temp_file+version_code+"/"+source_file)
	shutil.rmtree(platform_path+source_file+version_code)
	if last_version_code != None:
		shutil.rmtree(platform_path+source_file+last_version_code)
	if os.path.exists(platform_path+temp_file+version_code+"/tmp.zip"):
		zipSize = os.path.getsize(platform_path+temp_file+version_code+"/tmp.zip")
		zipHash = calc_sha1(platform_path+temp_file+version_code+"/tmp.zip")
		if os.path.exists(platform_path+temp_file+version_code+"/"+zipHash.upper()+".cuf") == False:
			os.rename(platform_path+temp_file+version_code+"/tmp.zip",platform_path+temp_file+version_code+"/"+zipHash.upper()+".cuf")
		else:
			os.remove(platform_path+temp_file+version_code+"/tmp.zip")
		cur_code = version_code.split('.')[2]
		last_code_1 = "1"
		if version_code == first_version_code:
			last_code_1 = "0"
		elif last_version_code != None:
			last_code_1 = last_version_code.split('.')[2]
		last_code_2 = "1"
		if last_version_code != None:
			last_code_2 = last_version_code.split('.')[2]
		fp = open(platform_path+temp_file+version_code+"_"+last_code_1+".lua", 'w')
		fp.write("return { url=\""+version_code+"/\",version="+cur_code+','+'\n')
		fp.write("	{op=0,version="+last_code_2+",fname=\""+zipHash.upper()+".cuf\",path=\"_tmp_.tmp\",size="+str(zipSize)+",compr=1}"+'\n')
		fp.write('}')
		fp.close()

if __name__ == "__main__":
	from argparse import ArgumentParser
	parser = ArgumentParser(prog="create_hot_version", description="create hot version")
	parser.add_argument("-f", "--first-version", dest="first_version_code", help="first version code")
	parser.add_argument("-n", "--new-version", dest="new_version_code", help="new version code")
	parser.add_argument("-e", "--encrypt", dest="is_encrypt", help="Is need encrypt?(Y/N)")
	(args, unknown) = parser.parse_known_args()

	if os.path.exists(platform_path) == False:
		os.makedirs(platform_path)
	if os.path.exists(platform_path+temp_file):
		shutil.rmtree(platform_path+temp_file)

	if args.first_version_code != None:
		first_version_code = args.first_version_code

	if args.is_encrypt != None:
		if args.is_encrypt != 'Y' and args.is_encrypt != 'y':
			is_encrypt = False
	else:
		is_need_encrypt = raw_input("Is need encrypt?(Y/N): ")
		if is_need_encrypt != 'Y' and is_need_encrypt != 'y':
			is_encrypt = False

	first_version_code_list = first_version_code.split('.')
	if len(first_version_code_list) != 3:
		exit(0, first_version_code+" is a wrong version code !")

	new_version_code = first_version_code
	is_first_version_exist = os.path.exists(platform_path+first_version_code+".zip")
	if is_first_version_exist:
		if args.new_version_code != None:
			new_version_code = args.new_version_code
		else:
			new_version_code = raw_input("Input New Version: ")
		version_code_list = new_version_code.split('.')
		if len(version_code_list) == 3:
			if int(version_code_list[0]) == int(first_version_code_list[0]) and int(version_code_list[1]) == int(first_version_code_list[1]) and int(version_code_list[2]) > int(first_version_code_list[2]):
				if os.path.exists(platform_path+source_file+new_version_code+".zip") == False:
					create_version_source(new_version_code)
				for index in range(int(first_version_code_list[2]), int(version_code_list[2])):
					version_code_index = version_code_list[0]+'.'+version_code_list[1]+'.'+str(index)
					if os.path.exists(platform_path+source_file+version_code_index+".zip") == False:
						if os.path.exists(platform_path+source_file+new_version_code+".zip"):
							os.remove(platform_path+source_file+new_version_code+".zip")
						if os.path.exists(platform_path+source_file+new_version_code):
							shutil.rmtree(platform_path+source_file+new_version_code)
						exit(0, "The version code "+version_code_index+" is not exist!")
					create_new_version(new_version_code, version_code_index)
					if os.path.exists(platform_path+source_file+version_code_index):
						shutil.rmtree(platform_path+source_file+version_code_index)
					if is_changed == False:
						if os.path.exists(platform_path+source_file+new_version_code+".zip"):
							os.remove(platform_path+source_file+new_version_code+".zip")
						if os.path.exists(platform_path+source_file+new_version_code):
							shutil.rmtree(platform_path+source_file+new_version_code)
						exit(0, "The version code "+new_version_code+" haven't changed compared with "+version_code_index+"!")
				zip_version(platform_path+temp_file, platform_path+new_version_code+".zip")
				if os.path.exists(platform_path+source_file+new_version_code):
					shutil.rmtree(platform_path+source_file+new_version_code)
			else:
				exit(0, "Wrong version, please input version code "+first_version_code_list[0]+"."+first_version_code_list[1]+".x(x>"+first_version_code_list[2]+")!")
		else:
			exit(0, new_version_code+" is a wrong version code !")
	else:
		version_code_list = new_version_code.split('.')
		if os.path.exists(platform_path+source_file+new_version_code+".zip") == False:
			create_version_source(new_version_code)
		create_new_version(new_version_code, None)
		zip_version(platform_path+temp_file, platform_path+new_version_code+".zip")
		if os.path.exists(platform_path+source_file+new_version_code):
			shutil.rmtree(platform_path+source_file+new_version_code)

	exit(0, "Press Enter to continue: ")