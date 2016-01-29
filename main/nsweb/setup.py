import os
from setuptools import setup
from distutils.core import Extension
from Cython.Build import cythonize

source_names = """
CAmbiguity.cpp
CDataFile.cpp
CDictionary.cpp
CDisAmbTable.cpp
CGenericProgress.cpp
CGiant.cpp
CHitList.cpp
CHitOffsetList.cpp
CTarget.cpp
CTextExploder.cpp
CTextFetch.cpp
QSortRand.cpp
StringTools.cpp
""".strip().split()

source_files = ['nsweb.pyx', 'CResultBuilder.cpp'] + [os.path.join('..', 'shared', 'src', f) for f in source_names]

setup(ext_modules = cythonize(Extension(
    'nsweb', 
    language='c++',
    sources=source_files,
    include_dirs=[os.path.join('..', 'shared', 'include'), 'include'],
    libraries=['jsoncpp'],
    extra_compile_args='-Wno-write-strings -Wno-unused-variable'.split(),
)),
    name='nsweb',
    install_requires='jsonschema flask Flask-Assets'.split()
)

