Import('RTT_ROOT')
Import('rtconfig')
# RT-Thread building script for component
from building import *

cwd     = GetCurrentDir()
src     = Glob('*.c')

CPPPATH = [os.path.join(cwd)]

group = DefineGroup('bytearray', src, depend = [''], CPPPATH = CPPPATH)

Return('group')
