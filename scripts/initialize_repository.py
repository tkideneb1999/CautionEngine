import os
import sys
import subprocess

import caution_py_utils as pyutils

USD_BIN = 'thirdparty/usd_bin'

def initialize_repository(build_all : bool):
    os.chdir('../')
    print ('Fixing submodules dependencies...')
    subprocess.call(['git', 'submodule', 'update', '--init', '--recursive'])

    print ('Checking your python installation...')
    pyutils.validate_py_version()
    pyutils.validate_packages()
    
    os.chdir('./scripts/')
    if build_all:
        print("Building USD binaries...")
        subprocess.call([os.path.abspath('./build_usd.bat'), 'nopause'])

    print ('Generating solutions...')
    subprocess.call([os.path.abspath('./generate_solutions_vs2022.bat'), 'nopause'])

if __name__ == "__main__":
    build_all = True
    if len(sys.argv) == 1:
        build_all = False
    else:
        build_all = sys.argv[1] == 'all'
    
    initialize_repository(build_all)