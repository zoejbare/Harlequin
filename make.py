#!/usr/bin/env python3
#
# Copyright (C) 2021, Zoe Bare
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions
# of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#

from __future__ import unicode_literals, division, print_function

import csbuild
import platform
import os
import shutil
import stat

from csbuild.tools.common.android_tool_base import AndroidStlLibType
from csbuild.tools.project_generators import visual_studio

###################################################################################################

# Disabling file type folders will put all source files under the the same respective directories in
# the generated project files, so for instance, there will be no separation between .cpp and .hpp files.
# This is useful for development that touches many files, but it can make the filters in each project
# look somewhat less organized.
visual_studio.SetEnableFileTypeFolders(False)

###################################################################################################

_REPO_ROOT_PATH = os.path.dirname(os.path.abspath(__file__))

###################################################################################################

outputSubPath = "{userData.outputRootName}/{targetName}"

csbuild.SetUserData("outputRootName", "{toolchainName}")
csbuild.SetOutputDirectory(f"_out/{outputSubPath}")
csbuild.SetIntermediateDirectory(f"_int/{outputSubPath}")

with csbuild.Toolchain("gcc", "clang", "msvc"):
	csbuild.SetUserData("outputRootName", f"{platform.system().lower()}-{{toolchainName}}-{{architectureName}}")

###################################################################################################

with csbuild.Target("debug"):
	csbuild.AddDefines("_DEBUG")
	csbuild.SetDebugLevel(csbuild.DebugLevel.ExternalSymbolsPlus)
	csbuild.SetOptimizationLevel(csbuild.OptimizationLevel.Disabled)
	csbuild.SetDebugRuntime(True)

with csbuild.Target("fastdebug"):
	csbuild.AddDefines("_DEBUG")
	csbuild.SetDebugLevel(csbuild.DebugLevel.ExternalSymbolsPlus)
	csbuild.SetOptimizationLevel(csbuild.OptimizationLevel.Max)
	csbuild.SetDebugRuntime(True)

with csbuild.Target("release"):
	csbuild.AddDefines("NDEBUG")
	csbuild.SetDebugLevel(csbuild.DebugLevel.Disabled)
	csbuild.SetOptimizationLevel(csbuild.OptimizationLevel.Max)
	csbuild.SetDebugRuntime(False)

###################################################################################################

csbuild.SetCcLanguageStandard("c99")
csbuild.SetCxxLanguageStandard("c++17")

with csbuild.Toolchain("psvita"):
	# PSVita needs "__STDC_FORMAT_MACROS" defined in order
	# to see the integer type format macros in <inttypes.h>.
	csbuild.AddDefines("__STDC_FORMAT_MACROS")
	csbuild.AddCompilerFlags("-Xthumb=1")

with csbuild.Toolchain("ps3", "psvita"):
	csbuild.SetCxxLanguageStandard("cpp11")

with csbuild.Platform("Windows"):
	with csbuild.Toolchain("gcc", "clang", "ps4", "ps5", "android-gcc", "android-clang"):
		csbuild.AddCompilerFlags("-fdiagnostics-format=msvc")

with csbuild.Platform("Linux"):
	with csbuild.Toolchain("clang"):
		csbuild.AddLinkerFlags("-stdlib=libstdc++")

with csbuild.Platform("Darwin"):
	with csbuild.Toolchain("clang"):
		csbuild.AddLinkerFlags("-stdlib=libc++")

with csbuild.Toolchain("android-gcc", "android-clang"):
	csbuild.SetAndroidTargetSdkVersion(26) # Android 7.0
	csbuild.SetAndroidStlLibType(AndroidStlLibType.LibCpp)

with csbuild.Toolchain("gcc", "clang", "ps4", "ps5", "android-gcc", "android-clang"):
	with csbuild.Architecture("x86", "x64"):
		csbuild.AddCompilerCxxFlags("-msse4.1")

	csbuild.AddCompilerFlags("-fexceptions")
	csbuild.AddCompilerCcFlags("-fPIC")

with csbuild.Toolchain("gcc", "android-gcc"):
	csbuild.AddCompilerCxxFlags(
		# Enabled warnings.
		"-Wall",
		"-Wextra",

		# Disabled warnings.
		"-Wno-address",
		"-Wno-ignored-qualifiers",
		"-Wno-implicit-fallthrough",
		"-Wno-missing-field-initializers",
		"-Wno-nonnull-compare",
	)

with csbuild.Toolchain("clang", "ps4", "ps5", "android-clang"):
	csbuild.AddCompilerCxxFlags(
		# Enabled warnings.
		"-Wall",
		"-Wextra",
		"-Wpedantic",

		# Disabled warnings.
		"-Wno-extra-semi",
		"-Wno-format-pedantic",
		"-Wno-gnu-anonymous-struct",
		"-Wno-gnu-zero-variadic-macro-arguments",
		"-Wno-nested-anon-types",
		"-Wno-undefined-var-template",

		# Test warnings - only use when tracking down specific issues!
		#"-Weverything",
		#"-Wno-c++98-compat",
		#"-Wno-c++98-compat-pedantic",
		#"-Wno-float-equal",
		#"-Wno-old-style-cast",
		#"-Wno-padded",
		#"-Wno-sign-conversion",
		#"-Wno-weak-vtables",
	)

with csbuild.Toolchain("msvc"):
	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddDefines(
			"__cplusplus=201703L",
			"_HAS_CXX14=1",
		)
	csbuild.SetVisualStudioVersion("17") # Visual Studio 2022
	csbuild.AddDefines(
		"_CRT_SECURE_NO_WARNINGS",
		"_CRT_NONSTDC_NO_WARNINGS",
	)
	csbuild.AddCompilerCxxFlags(
		"/bigobj",
		"/permissive-",
		"/Zc:__cplusplus",
		"/EHsc",
		"/W4",
	)
	csbuild.AddCompilerFlags(
		"/wd4804", # 'operation' : unsafe use of type 'bool' in operation
	)


###################################################################################################

class ExtGoogleTest(object):
	projectName = "external_gtest"
	path = "external/googletest/googletest"

with csbuild.Project(ExtGoogleTest.projectName, ExtGoogleTest.path, autoDiscoverSourceFiles=False):
	csbuild.SetOutput("{name}", csbuild.ProjectType.Stub)

	with csbuild.Scope(csbuild.ScopeDef.Children):
		csbuild.AddIncludeDirectories(f"{ExtGoogleTest.path}/include")

	with csbuild.Scope(csbuild.ScopeDef.Final):
		csbuild.AddIncludeDirectories(
			f"{ExtGoogleTest.path}",
		)
		csbuild.AddSourceFiles(
			f"{ExtGoogleTest.path}/src/gtest-all.cc",
			f"{ExtGoogleTest.path}/src/gtest_main.cc",
		)

###################################################################################################

class ExtSkipProbe(object):
	projectName = "external_skipprobe"
	path = "external/SkipProbe/include"

with csbuild.Project(ExtSkipProbe.projectName, ExtSkipProbe.path, autoDiscoverSourceFiles=False):
	csbuild.SetOutput("{name}", csbuild.ProjectType.Stub)

	with csbuild.Scope(csbuild.ScopeDef.Children):
		csbuild.AddIncludeDirectories(ExtSkipProbe.path)

###################################################################################################

class ExtXxHash(object):
	projectName = "external_xxhash"
	path = "external/xxHash"

with csbuild.Project(ExtXxHash.projectName, ExtXxHash.path, autoDiscoverSourceFiles=False):
	csbuild.SetOutput("{name}", csbuild.ProjectType.Stub)

	with csbuild.Scope(csbuild.ScopeDef.Children):
		csbuild.AddIncludeDirectories(ExtXxHash.path)

###################################################################################################

class HarlequinCommon(object):
	appRootPath = "src/app"
	libRootPath = "src/lib"

	@staticmethod
	def setLibCommonOptions(outputName):
		csbuild.SetOutput(outputName, csbuild.ProjectType.SharedLibrary)

		with csbuild.Toolchain("ps3", "ps4", "ps5", "psvita"):
			csbuild.SetOutput(outputName, csbuild.ProjectType.StaticLibrary)
			csbuild.AddDefines("HQ_BUILD_STATIC_LIB")

		with csbuild.Scope(csbuild.ScopeDef.All):
			csbuild.AddIncludeDirectories(HarlequinCommon.libRootPath)

		if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
			csbuild.AddSourceDirectories(HarlequinCommon.libRootPath)

	@staticmethod
	def setAppCommonOptions(outputName):
		csbuild.SetOutput(outputName, csbuild.ProjectType.Application)

		with csbuild.Toolchain("msvc"):
			csbuild.SetMsvcSubsystem("CONSOLE")

		with csbuild.Toolchain("gcc", "clang"):
			with csbuild.Platform("Linux"):
				csbuild.AddLibraries(
					"pthread",
				)

		with csbuild.Toolchain("ps4", "ps5", "blup"):
			csbuild.AddLibraries(
				"ScePosix_stub_weak",
			)

###################################################################################################

class LibHarlequinBase(object):
	projectName = "LibHarlequinBase"
	outputName = "libhqbase"
	dependencies = [
		ExtSkipProbe.projectName,
		ExtXxHash.projectName,
	]

with csbuild.Project(LibHarlequinBase.projectName, HarlequinCommon.libRootPath, LibHarlequinBase.dependencies, autoDiscoverSourceFiles=False):
	HarlequinCommon.setLibCommonOptions(LibHarlequinBase.outputName)

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddExcludeDirectories(
			f"{HarlequinCommon.libRootPath}/compiler",
			f"{HarlequinCommon.libRootPath}/runtime",
		)
	else:
		csbuild.AddSourceDirectories(f"{HarlequinCommon.libRootPath}/base")
		csbuild.AddExcludeDirectories(
			f"{HarlequinCommon.libRootPath}/base/hi-res-timer-impl",
			f"{HarlequinCommon.libRootPath}/base/mutex-impl",
			f"{HarlequinCommon.libRootPath}/base/rwlock-impl",
			f"{HarlequinCommon.libRootPath}/base/thread-impl",
		)

	with csbuild.Toolchain("msvc", "gcc", "clang"):
		csbuild.AddDefines("HQ_BUILD_BASE_LIB_EXPORT")

	with csbuild.Toolchain("msvc"):
		csbuild.AddSourceFiles(
			f"{HarlequinCommon.libRootPath}/base/*-impl/*Win32.cpp",
		)

	with csbuild.Toolchain("gcc", "clang", "ps4", "ps5"):
		csbuild.AddSourceFiles(
			f"{HarlequinCommon.libRootPath}/base/*-impl/*Posix.cpp",
		)

	with csbuild.Toolchain("ps3"):
		csbuild.AddSourceFiles(
			f"{_REPO_ROOT_PATH}/support/Harlequin-PS3/lib/base/*/*.cpp",
		)

	with csbuild.Toolchain("psvita"):
		csbuild.AddSourceFiles(
			f"{_REPO_ROOT_PATH}/support/Harlequin-PSVita/lib/base/*/*.cpp",
		)

###################################################################################################

class LibHarlequinCompiler(object):
	projectName = "LibHarlequinCompiler"
	outputName = "libhqcompiler"
	dependencies = [
		LibHarlequinBase.projectName,
	]

with csbuild.Project(LibHarlequinCompiler.projectName, HarlequinCommon.libRootPath, LibHarlequinCompiler.dependencies, autoDiscoverSourceFiles=False):
	HarlequinCommon.setLibCommonOptions(LibHarlequinCompiler.outputName)

	csbuild.SetSupportedToolchains("msvc", "gcc", "clang")

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddExcludeDirectories(
			f"{HarlequinCommon.libRootPath}/base",
			f"{HarlequinCommon.libRootPath}/runtime",
		)

	else:
		csbuild.AddSourceDirectories(
			f"{HarlequinCommon.libRootPath}/common",
			f"{HarlequinCommon.libRootPath}/compiler",
		)

	with csbuild.Scope(csbuild.ScopeDef.All):
		csbuild.AddDefines("HQ_LIB_COMPILER")

	with csbuild.Toolchain("msvc", "gcc", "clang"):
		csbuild.AddDefines("HQ_BUILD_MAIN_LIB_EXPORT")

###################################################################################################

class LibHarlequinRuntime(object):
	projectName = "LibHarlequinRuntime"
	outputName = "libhqruntime"
	dependencies = [
		LibHarlequinBase.projectName,
	]

with csbuild.Project(LibHarlequinRuntime.projectName, HarlequinCommon.libRootPath, LibHarlequinRuntime.dependencies, autoDiscoverSourceFiles=False):
	HarlequinCommon.setLibCommonOptions(LibHarlequinRuntime.outputName)

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddExcludeDirectories(
			f"{HarlequinCommon.libRootPath}/base",
			f"{HarlequinCommon.libRootPath}/compiler",
		)

	else:
		csbuild.AddSourceDirectories(
			f"{HarlequinCommon.libRootPath}/common",
			f"{HarlequinCommon.libRootPath}/runtime",
		)

	with csbuild.Scope(csbuild.ScopeDef.All):
		csbuild.AddDefines("HQ_LIB_RUNTIME")

	with csbuild.Toolchain("msvc", "gcc", "clang"):
		csbuild.AddDefines("HQ_BUILD_MAIN_LIB_EXPORT")

###################################################################################################

class HarlequinCompiler(object):
	projectName = "HarlequinCompiler"
	outputName = "hqc"
	path = f"{HarlequinCommon.appRootPath}/compiler"
	dependencies = [
		LibHarlequinCompiler.projectName
	]

with csbuild.Project(HarlequinCompiler.projectName, HarlequinCompiler.path, HarlequinCompiler.dependencies):
	HarlequinCommon.setAppCommonOptions(HarlequinCompiler.outputName)

	csbuild.SetSupportedToolchains("msvc", "gcc", "clang")

###################################################################################################

class HarlequinRuntime(object):
	projectName = "HarlequinRuntime"
	outputName = "hq"
	path = f"{HarlequinCommon.appRootPath}/runtime"
	dependencies = [
		LibHarlequinRuntime.projectName
	]

with csbuild.Project(HarlequinRuntime.projectName, HarlequinRuntime.path, HarlequinRuntime.dependencies):
	HarlequinCommon.setAppCommonOptions(HarlequinRuntime.outputName)

###################################################################################################

class HarlequinUnitTest(object):
	projectName = "UnitTest"
	outputName = "unittest"
	path = f"{HarlequinCommon.appRootPath}/unit_test"
	dependencies = [
		ExtGoogleTest.projectName,
		LibHarlequinCompiler.projectName,
		LibHarlequinRuntime.projectName,
	]

with csbuild.Project(HarlequinUnitTest.projectName, HarlequinUnitTest.path, HarlequinUnitTest.dependencies):
	HarlequinCommon.setAppCommonOptions(HarlequinUnitTest.outputName)

	csbuild.SetSupportedToolchains("msvc", "gcc", "clang")

	with csbuild.Toolchain("gcc", "clang"):
		csbuild.AddCompilerCxxFlags(
			"-Wno-sign-compare",
		)

	with csbuild.Toolchain("msvc"):
		csbuild.AddCompilerCxxFlags(
			"/wd4389", # '==': signed/unsigned mismatch
		)

###################################################################################################
