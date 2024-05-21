import shutil
import os
import filecmp

# ! 约定
# ! 此文件用于 VScode 自动化，而不是在命令行使用的 (故当前路径为：build 目录)
# ! 1. code中的每个目录含有inc的，则inc下的文件的父目录不是inc，而是inc的父目录 (不包括 _lib, _dll)
# ! 2. 需要 update include 的目录(长度大于4)以 _lib, _dll 结尾
# ! 3. update 后，include目录下为源文件的父目录 (1中说的父目录)
# * 每个code下的文件 cmake 使用 INC 包含目录，因此 如果有文件 xx/inc/file.h 需要被其他文件包含，
# *     为了区分的话，应写为 #include "xx/file.h" (不要inc)


def get_dir_name(dir):      # 给每个code下的 lib or dll 的文件 update 到 inlcude 时保留最顶层目录 (xx_lib 为 xx)
    file_name = os.path.basename(dir)
    index = file_name.find('_lib')
    if index == -1:
        index = file_name.find('_dll')
    if index == -1: return dir
    else:
        new_dir = os.path.dirname(dir)
        new_file_name = file_name[0:index]
        return os.path.join(new_dir, new_file_name)

def input_yn(prompt):
    while (True):
        s = input(prompt)
        if (s == "y" or s == "Y"): return True
        if (s == 'n' or s == 'N'): return False 

def copy_dir(src, tar):
    if not os.path.exists(src): 
        print(src + " not exist")
        return False
    if not os.path.exists(tar):
        if (input_yn(tar + "不存在，是否创建(y/n): ")):
            os.makedirs(tar)
        else: return False
    for item in os.listdir(src):
        s = os.path.join(src, item)
        t = os.path.join(tar, item)
        if (os.path.isdir(s)):
            copy_dir(s, t)
        else:
            if not os.path.exists(t):
                shutil.copy2(s, t)
            else:
                if not filecmp.cmp(s, t, shallow=False):
                    shutil.copy2(s, t)
    return True

# *******************************
build = os.getcwd()    # 当前路径为：build 目录
if (os.path.basename(build) != 'build'):
    print("~~~~~~~~~" + build + " isn't 'build' dir")
inc_str = "inc"

# update target
root = os.path.dirname(build)

code_dir = os.path.join(root, "code")
target = os.path.join(root, "include")
# ********************************

# 当前目录下的所有子目录名称 (只找 以 '_lib' or '_dll' 结尾的目录, 如 xx_lib、xx_dll)
sub_dirs = [name for name in os.listdir(code_dir)
        if (os.path.isdir(os.path.join(code_dir, name))) and (len(name) > 4)  and \
            (name.find("_lib") == len(name) - 4 or name.find("_dll") == len(name) - 4)]          

# 目录下有 inc 的目录
res_flag = False

for sub_dir in sub_dirs:
    src = os.path.join(code_dir, os.path.join(sub_dir, inc_str))
    if (copy_dir(src, get_dir_name(os.path.join(target, sub_dir)))):
        res_flag = True

if (res_flag): print(target + "更新成功")
else: print(target + "更新失败")