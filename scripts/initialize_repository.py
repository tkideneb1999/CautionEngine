import os
import subprocess

os.chdir('../')
print ('Fixing submodules dependencies...')
subprocess.call(['git', 'submodule', 'update', '--init', '--recursive'])

# TODO: make a checker if python and premake are installed properly
print ('Generating premake solutions')
subprocess.call([os.path.abspath('./scripts/generate_solutions_vs2022.bat'), 'nopause'])