import sys
import subprocess



def validate_py_version(versionMajor = 3, versionMinor = 11):
        if sys.version is not None:
            print("Python version {0:d}.{1:d}.{2:d} detected.".format( \
                sys.version_info.major, sys.version_info.minor, sys.version_info.micro))
            
            if sys.version_info.major < versionMajor or (sys.version_info.major == versionMajor and sys.version_info.minor < versionMinor):
                print("Python version too low, expected version {0:d}.{1:d} or higher.".format( \
                    versionMajor, versionMinor))
                return False
            return True
        
def download_pip_package(pkgname : str):
    try:
        print(f"Trying to download {pkgname} package")
        subprocess.check_call(['pip', 'install', pkgname])
        print(f"Successfully installed {pkgname}")
    except subprocess.CalledProcessError:
        print(f"Failed to install {pkgname}")
        
def validate_packages():
    print("Checking python dependencies that will be used for the engine")
    
    try:
        import PySide6
    except ImportError:
        print(f"PySide6 package not detected, installing it...")
        download_pip_package("PySide6")

    try:
        import PyOpenGL
    except ImportError:
        print(f"PyOpenGL package not detected, installing it...")
        download_pip_package("PyOpenGL")

    try:
        import requests
    except ImportError:
        print(f"Requests package not detected, installing...")
        download_pip_package("requests")


if __name__ == "__main__":
    if not validate_py_version():
        print("Version detected is lower than the required version. Aborting...")
    else:
        validate_packages()