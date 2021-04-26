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

outputSubPath = "{toolchainName}/{architectureName}/{targetName}"

csbuild.SetOutputDirectory(f"_out/{outputSubPath}")
csbuild.SetIntermediateDirectory(f"_int/{outputSubPath}")

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

with csbuild.Platform("Windows"):
	with csbuild.Toolchain("gcc", "clang", "ps4", "android-gcc", "android-clang"):
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

with csbuild.Toolchain("gcc", "clang", "ps4", "android-gcc", "android-clang"):
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

with csbuild.Toolchain("clang", "ps4", "android-clang"):
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
			"__cplusplus=201403",
			"_HAS_CXX14=1",
		)
	csbuild.SetVisualStudioVersion("16") # Visual Studio 2019
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

class ExtNcps(object):
	projectName = "external_ncps"
	path = "external/NCPSQueue"

with csbuild.Project(ExtNcps.projectName, ExtNcps.path, autoDiscoverSourceFiles=False):
	csbuild.SetOutput("{name}", csbuild.ProjectType.Stub)

	with csbuild.Scope(csbuild.ScopeDef.Children):
		csbuild.AddIncludeDirectories(f"{ExtNcps.path}/include")

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

class XenonLib(object):
	rootPath = "src/lib"

	@staticmethod
	def setCommonOptions(outputName):
		csbuild.SetOutput(outputName, csbuild.ProjectType.SharedLibrary)

		with csbuild.Toolchain("ps4"):
			csbuild.SetOutput(outputName, csbuild.ProjectType.StaticLibrary)
			csbuild.AddDefines("XENON_BUILD_STATIC_LIB")

		with csbuild.Scope(csbuild.ScopeDef.All):
			csbuild.AddIncludeDirectories(XenonLib.rootPath)

		if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
			csbuild.AddSourceDirectories(XenonLib.rootPath)

###################################################################################################

class LibXenonBase(object):
	projectName = "LibXenonBase"
	outputName = "libxenonbase"
	dependencies = [
		ExtNcps.projectName,
		ExtSkipProbe.projectName,
		ExtXxHash.projectName,
	]

with csbuild.Project(LibXenonBase.projectName, XenonLib.rootPath, LibXenonBase.dependencies, autoDiscoverSourceFiles=False):
	XenonLib.setCommonOptions(LibXenonBase.outputName)

	csbuild.AddSourceDirectories(f"{XenonLib.rootPath}/base")

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddExcludeDirectories(
			f"{XenonLib.rootPath}/compiler",
			f"{XenonLib.rootPath}/runtime",
		)
	else:
		csbuild.AddExcludeDirectories(
			f"{XenonLib.rootPath}/base/mutex-impl",
			f"{XenonLib.rootPath}/base/thread-impl",
		)

	with csbuild.Toolchain("msvc", "gcc", "clang"):
		csbuild.AddDefines("XENON_BUILD_BASE_LIB_EXPORT")

	with csbuild.Toolchain("msvc"):
		csbuild.AddSourceFiles(
			f"{XenonLib.rootPath}/base/*-impl/*Win32.cpp",
		)

	with csbuild.Toolchain("gcc", "clang", "ps4"):
		csbuild.AddSourceFiles(
			f"{XenonLib.rootPath}/base/*-impl/*Posix.cpp",
		)

###################################################################################################

class LibXenonCompiler(object):
	projectName = "LibXenonCompiler"
	outputName = "libxenoncompiler"
	dependencies = [
		LibXenonBase.projectName,
	]

with csbuild.Project(LibXenonCompiler.projectName, XenonLib.rootPath, LibXenonCompiler.dependencies, autoDiscoverSourceFiles=False):
	XenonLib.setCommonOptions(LibXenonCompiler.outputName)

	csbuild.SetSupportedToolchains("msvc", "gcc", "clang")
	csbuild.AddSourceDirectories(
		f"{XenonLib.rootPath}/common",
		f"{XenonLib.rootPath}/compiler",
	)

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddExcludeDirectories(
			f"{XenonLib.rootPath}/base",
			f"{XenonLib.rootPath}/runtime",
		)

	with csbuild.Scope(csbuild.ScopeDef.All):
		csbuild.AddDefines("XENON_LIB_COMPILER")

	with csbuild.Toolchain("msvc", "gcc", "clang"):
		csbuild.AddDefines("XENON_BUILD_MAIN_LIB_EXPORT")

###################################################################################################

class LibXenonRuntime(object):
	projectName = "LibXenonRuntime"
	outputName = "libxenonruntime"
	dependencies = [
		LibXenonBase.projectName,
	]

with csbuild.Project(LibXenonRuntime.projectName, XenonLib.rootPath, LibXenonRuntime.dependencies, autoDiscoverSourceFiles=False):
	XenonLib.setCommonOptions(LibXenonRuntime.outputName)

	csbuild.AddSourceDirectories(
		f"{XenonLib.rootPath}/common",
		f"{XenonLib.rootPath}/runtime",
	)

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddExcludeDirectories(
			f"{XenonLib.rootPath}/base",
			f"{XenonLib.rootPath}/compiler",
		)

	with csbuild.Scope(csbuild.ScopeDef.All):
		csbuild.AddDefines("XENON_LIB_RUNTIME")

	with csbuild.Toolchain("msvc", "gcc", "clang"):
		csbuild.AddDefines("XENON_BUILD_MAIN_LIB_EXPORT")

###################################################################################################

class XenonApp(object):
	rootPath = "src/app"

	@staticmethod
	def setCommonOptions(outputName):
		csbuild.SetOutput(outputName, csbuild.ProjectType.Application)

		with csbuild.Toolchain("msvc"):
			csbuild.SetMsvcSubsystem("CONSOLE")

		with csbuild.Toolchain("gcc", "clang"):
			csbuild.AddLibraries(
				"pthread",
			)

		with csbuild.Toolchain("ps4"):
			csbuild.AddLibraries(
				"ScePosix_stub_weak",
			)

###################################################################################################

class XenonRuntime(object):
	projectName = "XenonRuntime"
	outputName = "xenon"
	path = f"{XenonApp.rootPath}/runtime"
	dependencies = [
		LibXenonRuntime.projectName
	]

with csbuild.Project(XenonRuntime.projectName, XenonRuntime.path, XenonRuntime.dependencies):
	XenonApp.setCommonOptions(XenonRuntime.outputName)

###################################################################################################

class XenonCompiler(object):
	projectName = "XenonCompiler"
	outputName = "xenonc"
	path = f"{XenonApp.rootPath}/compiler"
	dependencies = [
		LibXenonCompiler.projectName
	]

with csbuild.Project(XenonCompiler.projectName, XenonCompiler.path, XenonCompiler.dependencies):
	XenonApp.setCommonOptions(XenonCompiler.outputName)

	csbuild.SetSupportedToolchains("msvc", "gcc", "clang")

###################################################################################################

class XenonUnitTest(object):
	projectName = "XenonUnitTest"
	outputName = "xenonunittest"
	path = f"{XenonApp.rootPath}/unit_test"
	dependencies = [
		ExtGoogleTest.projectName,
		LibXenonCompiler.projectName,
		LibXenonRuntime.projectName,
	]

with csbuild.Project(XenonUnitTest.projectName, XenonUnitTest.path, XenonUnitTest.dependencies):
	XenonApp.setCommonOptions(XenonUnitTest.outputName)

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
