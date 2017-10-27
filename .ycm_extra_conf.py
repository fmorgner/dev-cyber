"""
A simple, dynamic configuration for YouCompleteMe's clang-completer

The variables in the section 'User configuration' allow you to customize paths
and flags for your project. You can also execute this script using the python
interpreter, to generate a generic list of flags for you project, which in turn
can be used by VIM color_coded for example.
"""

import os
from os import path as op
import re
import subprocess
import sys

# User configuration:
KERNEL_DIR = op.join('/lib/modules/', os.uname().release, 'build')
PLATFORM = (
    'x86' if os.uname().machine.startswith('x86')
    else 'arm'
)

# The language to use ('c' or 'c++')
LANG = 'c'

# The language dialect ('iso' or 'gnu')
DIALECT = 'gnu'

# The language standard version to use
# For C use either '89', '90', '99', or '11'
# For C++ use either '98', '03', '11', '14', or '1z' or '17'
STD = '89'

# The project root directory
# Generally the default (the directoy where this script lies) is ok.
PROJECT_ROOT = op.abspath(op.dirname(__file__))

# The directory containing the 'conanbuildinfo.txt' file, if applicable
BUILDINFO_DIR = op.join(PROJECT_ROOT, 'build')

# The directory containing the CMake build environment, if applicable
CMAKE_DIR = BUILDINFO_DIR

# General compiler flags (like warnings, etc.) to use during analysis
PROJECT_FLAGS = [
    '-D__KERNEL__',
    '-DKBUILD_MODNAME=\'"cyber"\'',
    '-include ' + op.join(KERNEL_DIR, 'include', 'linux', 'kconfig.h'),
    '-DMODULE',
    '-Wall',
    '-Werror',
]

# Project local include paths
# Relative paths will be resolved with regard to the PROJECT_ROOT
# NOTE: Do NOT use '-I' or other option flags, just simple paths
PROJECT_LOCAL_INCLUDES = [
    'include',
]

# Project external include paths
# Only absolute paths are applicable
# NOTE: Do NOT use '-I' or other option flags, just simple paths
PROJECT_EXTERNAL_INCLUDES = [
    '/usr/lib/gcc/x86_64-pc-linux-gnu/7.2.0/include',
    op.join(KERNEL_DIR, 'arch', PLATFORM, 'include'),
    op.join(KERNEL_DIR, 'arch', PLATFORM, 'include', 'generated'),
    op.join(KERNEL_DIR, 'include'),
    op.join(KERNEL_DIR, 'include', 'uapi'),
    op.join(KERNEL_DIR, 'include', 'generated', 'uapi'),
    op.join(KERNEL_DIR, 'arch', PLATFORM, 'include', 'uapi'),
    op.join(KERNEL_DIR, 'arch', PLATFORM, 'include', 'generated', 'uapi'),
]

# Magic from here on out:

CLANG_REGEX = re.compile((
    r'(?:\#include \<...\> search starts here\:)'
    r'(?P<list>.*?)'
    r'(?:End of search list)'
), re.DOTALL)

STD = (LANG if DIALECT == 'iso' else
       (DIALECT if LANG == 'c' else DIALECT + '++')) + STD

CLANG_COMMAND = [
    'clang',
    '-v',
    '-E',
    '-x', LANG,
    '-std=' + STD,
    '-'
]

SOURCE_EXTENSIONS = [
    'cpp',
    'cc',
    'cxx',
    'c++',
    'c',
]


def compiler_includes():
    """
    Acquire clangs system include paths
    """
    process = subprocess.Popen(CLANG_COMMAND,
                               stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    process_out, process_err = process.communicate('')
    output = process_out + process_err
    output = str(output).encode('utf8').decode('unicode_escape')
    try:
        for path in re.search(CLANG_REGEX, output).group('list').split('\n'):
            path = path.strip()
            if path and path.find('(framework directory)') < 0:
                yield '-isystem'
                yield op.abspath(path)
    except AttributeError:
        print('Failed to acquire system includes from compiler')
        print('Tried with the following commandline:\n')
        print(' '.join(CLANG_COMMAND))
        sys.exit(1)


def project_local_includes():
    """
    Process the PROJECT_LOCAL_INCLUDES for delivery to the completer

    Makes sure that all paths are absolute.
    """
    paths = iter(PROJECT_LOCAL_INCLUDES)
    for path in paths:
        yield '-I'
        if not op.isabs(path):
            yield op.abspath(op.join(PROJECT_ROOT, path))
        else:
            yield path


def project_external_includes():
    """
    Process the PROJECT_EXTERNAL_INCLUDES for delivery to the completer

    Prevents relative paths.
    """
    paths = iter(PROJECT_EXTERNAL_INCLUDES)
    for path in paths:
        if not op.isabs(path):
            raise RuntimeError('Expected absolute path for external headers')
        yield '-isystem'
        yield op.abspath(path)


def configuration_flags(filename):
    """
    Generate the flags based on the configuration
    """
    flags = [
        '-x',
        LANG,
        '-std=' + STD,
    ]

    flags += PROJECT_FLAGS
    #  flags += compiler_includes()
    flags += project_local_includes()
    flags += project_external_includes()

    return flags


# pylint: disable=C0103
# pylint: disable=W0613
def FlagsForFile(filename, **kwargs):
    """
    Calculate the compiler flags for the given file
    """
    return {'flags': configuration_flags(filename), 'do_cache': True}


if __name__ == '__main__':
    for flag in FlagsForFile('')['flags']:
        print(flag)
