import os
import sys
import subprocess

VCINSTALLDIR = "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat"

current_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
build_dir = os.path.normpath(current_dir + "/build")
os.makedirs(build_dir, exist_ok=True)

build_cmd = [f'{os.path.normpath(VCINSTALLDIR)}', 'x86']

cmake_cmd = [
    f'cmake',
    f'-DCMAKE_BUILD_TYPE=RelWithDebInfo',
    f'-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE',
    f'-S{current_dir}',
    f'-B{build_dir}',
    '-G', 'Ninja'
]
cmake_build_cmd = [
    f'cmake',   
    f'--build', f'{build_dir}', 
    f'--config', f'RelWithDebInfo'
]

cmd = build_cmd + ['&&'] + cmake_cmd + ['&&'] + cmake_build_cmd

try:
    subprocess.run(cmd, shell=True, check=True)
    print("编译成功！")
    sys.exit(0)
except subprocess.CalledProcessError as e:
    print("编译失败：", e)
    sys.exit(1)