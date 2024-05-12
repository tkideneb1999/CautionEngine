import os
import subprocess

USD_BIN = 'thirdparty/usd_bin'

os.chdir('../')
print ('Fixing submodules dependencies...')
subprocess.call(['git', 'submodule', 'update', '--init', '--recursive'])

print("Building USD binaries...")
subprocess.call([os.path.abspath('./scripts/build_usd.bat'), 'nopause'])

# TODO: make a checker if python and premake are installed properly
print ('Generating premake solutions')
subprocess.call([os.path.abspath('./scripts/generate_solutions_vs2022.bat'), 'nopause'])