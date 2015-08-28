from distutils.core import setup, Extension


setup (ext_modules = [Extension('getkeypress', ['config.c','key_util.c','getdevicefile.c','getkeypress.c'])],)
