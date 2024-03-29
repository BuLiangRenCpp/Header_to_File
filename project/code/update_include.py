import shutil
import os
import filecmp

def copy_dir(src, tar):
    if not os.path.exists(tar):
        os.makedirs(tar)
    for item in os.listdir(src):
        s = os.path.join(src, item)
        t = os.path.join(tar, item)
        if (os.path.isdir(s)):
            copy_dir(s, t)
        else:
            if not filecmp.cmp(s, t, shallow=False):
                shutil.copy2(s, t)

# *******************************
src_dir = os.getcwd()
inc_str = "inc"

target = os.path.dirname(src_dir)
target = os.path.join(target, "include")
# ********************************

# 当前目录下的所有子目录名称
sub_dirs = [name for name in os.listdir(os.curdir)
        if os.path.isdir(os.path.join(os.curdir, name))]            

for sub_dir in sub_dirs:
    src = os.path.join(sub_dir, inc_str)
    copy_dir(src, target)

print("更新成功")