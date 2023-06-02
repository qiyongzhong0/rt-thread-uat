from building import *

cwd = GetCurrentDir()
path = [cwd+'/inc']
src  = Glob('src/*.c')
 
group = DefineGroup('uat', src, depend = ['PKG_USING_UAT'], CPPPATH = path)

Return('group')