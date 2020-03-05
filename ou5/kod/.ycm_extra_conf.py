import os
import ycm_core

flags = [
  '-Wall',
  '-std=c99',
  '-I',
  '../../datastructures-v1.0.8.2/include'
  ]

SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', ]

def FlagsForFile( filename, **kwargs ):
  return {
  'flags': flags,
  'do_cache': True
  }
