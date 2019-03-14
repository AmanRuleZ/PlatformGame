import sys
import PyQt5
from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but it might need fine tuning.
#additional_mods = ['numpy.core._methods', 'numpy.lib.format']
build_exe_options = {"packages": ["os", "numpy"], "excludes": ["tkinter"], "includes": ["PyQt5"]}

# GUI applications require a different base on Windows (the default is for a
# console application).
base = None
if sys.platform == "win32":
    base = "Win32GUI"

setup(  name = "MapEditor",
        version = "0.1",
        description = "MapEditor to Platformer Game!",
        options = {"build_exe": build_exe_options},
        executables = [Executable("mapEditor.py", base=base)])