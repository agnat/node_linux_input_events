import sys
import Options
from os import unlink, link
from os.path import exists 

APPNAME = 'node_input_event'
VERSION = '0.0.1'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'binding'
  obj.source = bld.path.ant_glob('src/*.cpp')
  obj.uselib = "DNSSD"
                

def shutdown():
  # HACK to get binding.node out of build directory.
  # better way to do this?
  if exists('lib/binding.node'): unlink('lib/binding.node')
  if Options.commands['build']:
    link('build/default/binding.node', 'lib/binding.node')

# vim: set filetype=python :
