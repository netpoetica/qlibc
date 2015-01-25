#.rst:
# CMakePushCheckStateEx
# -------------------
#
#
#
# This module is based on module CMakePushCheckState and defines three macros: 
# CMAKE_PUSH_CHECK_STATE_EX(), CMAKE_POP_CHECK_STATE_EX() 
# and CMAKE_RESET_CHECK_STATE_EX(). These macros can
# be used to save, restore and reset (i.e., clear contents) the state of
# the variables CMAKE_REQUIRED_FLAGS, CMAKE_REQUIRED_DEFINITIONS,
# CMAKE_REQUIRED_LIBRARIES, CMAKE_REQUIRED_INCLUDES and CMAKE_EXTRA_INCLUDE_FILES, 
# used by the various Check-files coming with CMake, like e.g.
# check_function_exists() etc.  The variable contents are pushed on a
# stack, pushing multiple times is supported.  This is useful e.g.  when
# executing such tests in a Find-module, where they have to be set, but
# after the Find-module has been executed they should have the same
# value as they had before.
#
# CMAKE_PUSH_CHECK_STATE() macro receives optional argument RESET.
# Whether it's specified, CMAKE_PUSH_CHECK_STATE() will set all
# CMAKE_REQUIRED_* variables to empty values, same as
# CMAKE_RESET_CHECK_STATE() call will do.
#
# Usage:
#
# ::
#
#    cmake_push_check_state(RESET)
#    set(CMAKE_REQUIRED_DEFINITIONS -DSOME_MORE_DEF)
#    check_function_exists(...)
#    cmake_reset_check_state()
#    set(CMAKE_REQUIRED_DEFINITIONS -DANOTHER_DEF)
#    check_function_exists(...)
#    cmake_pop_check_state()

#=============================================================================
# Based on CmakePushCheckState with modifications by Dmitry Vorobiev
# Copyright 2006-2011 Alexander Neundorf, <neundorf@kde.org>
# Copyright 2000-2014 Kitware, Inc.
# Copyright 2000-2011 Insight Software Consortium
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
# * Neither the names of Kitware, Inc., the Insight Software Consortium,
#   nor the names of their contributors may be used to endorse or promote
#   products derived from this software without specific prior written
#   permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


macro(CMAKE_RESET_CHECK_STATE_EX)

   set(CMAKE_REQUIRED_INCLUDES)
   set(CMAKE_REQUIRED_DEFINITIONS)
   set(CMAKE_REQUIRED_LIBRARIES)
   set(CMAKE_REQUIRED_FLAGS)
   set(CMAKE_REQUIRED_QUIET)
   set(CMAKE_EXTRA_INCLUDE_FILES)
endmacro()

macro(CMAKE_PUSH_CHECK_STATE_EX)

   if(NOT DEFINED _CMAKE_PUSH_CHECK_STATE_COUNTER_EX)
      set(_CMAKE_PUSH_CHECK_STATE_COUNTER_EX 0)
   endif()

   math(EXPR _CMAKE_PUSH_CHECK_STATE_COUNTER_EX "${_CMAKE_PUSH_CHECK_STATE_COUNTER_EX}+1")

   set(_CMAKE_REQUIRED_INCLUDES_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER_EX}    ${CMAKE_REQUIRED_INCLUDES})
   set(_CMAKE_REQUIRED_DEFINITIONS_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER_EX} ${CMAKE_REQUIRED_DEFINITIONS})
   set(_CMAKE_REQUIRED_LIBRARIES_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER_EX}   ${CMAKE_REQUIRED_LIBRARIES})
   set(_CMAKE_REQUIRED_FLAGS_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER_EX}       ${CMAKE_REQUIRED_FLAGS})
   set(_CMAKE_REQUIRED_QUIET_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER_EX}       ${CMAKE_REQUIRED_QUIET})
   set(_CMAKE_EXTRA_INCLUDE_FILES_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER_EX}       ${CMAKE_EXTRA_INCLUDE_FILES})

   if (ARGC GREATER 0 AND ARGV0 STREQUAL "RESET")
      cmake_reset_check_state_ex()
   endif()

endmacro()

macro(CMAKE_POP_CHECK_STATE_EX)

# don't pop more than we pushed
   if("${_CMAKE_PUSH_CHECK_STATE_COUNTER_EX}" GREATER "0")

      set(CMAKE_REQUIRED_INCLUDES    ${_CMAKE_REQUIRED_INCLUDES_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER}})
      set(CMAKE_REQUIRED_DEFINITIONS ${_CMAKE_REQUIRED_DEFINITIONS_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER}})
      set(CMAKE_REQUIRED_LIBRARIES   ${_CMAKE_REQUIRED_LIBRARIES_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER}})
      set(CMAKE_REQUIRED_FLAGS       ${_CMAKE_REQUIRED_FLAGS_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER}})
      set(CMAKE_REQUIRED_QUIET       ${_CMAKE_REQUIRED_QUIET_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER}})
	  set(CMAKE_EXTRA_INCLUDE_FILES  ${_CMAKE_EXTRA_INCLUDE_FILES_SAVE_EX_${_CMAKE_PUSH_CHECK_STATE_COUNTER}})

      math(EXPR _CMAKE_PUSH_CHECK_STATE_COUNTER_EX "${_CMAKE_PUSH_CHECK_STATE_COUNTER_EX}-1")
   endif()

endmacro()