from setuptools import setup, Extension

module = Extension('stats', sources=['stats.c'])
setup(name='stats', version='1.0', ext_modules=[module])
