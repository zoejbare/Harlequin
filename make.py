#!/usr/bin/env python3
#
# Copyright (c) 2021, Zoe J. Bare
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
import sys

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

_PLATFORM_REPO_PATHS = [
	os.path.join(_REPO_ROOT_PATH, "..", "Harlequin-PS3"),
	os.path.join(_REPO_ROOT_PATH, "..", "Harlequin-PS4"),
	os.path.join(_REPO_ROOT_PATH, "..", "Harlequin-PS5"),
	os.path.join(_REPO_ROOT_PATH, "..", "Harlequin-PSVita"),
]
_PLATFORM_REPO_PATHS = [x for x in _PLATFORM_REPO_PATHS if os.access(x, os.F_OK)]

# Patch the sys path so we can search for modules in the platform support directories.
sys.path = _PLATFORM_REPO_PATHS + sys.path

###################################################################################################

class _MissingSetupLibModule(object):
	@staticmethod
	def setupHqGlobalOptions():
		pass

	@staticmethod
	def setupHqBaseLib():
		pass

	@staticmethod
	def setupHqRuntimeApp():
		pass

try:
	import _hq_setup_ps3
except:
	_hq_setup_ps3 = _MissingSetupLibModule

try:
	import _hq_setup_ps4
except:
	_hq_setup_ps4 = _MissingSetupLibModule

try:
	import _hq_setup_ps5
except:
	_hq_setup_ps5 = _MissingSetupLibModule

try:
	import _hq_setup_psvita
except:
	_hq_setup_psvita = _MissingSetupLibModule

###################################################################################################

outputSubPath = "{userData.outputRootName}/{targetName}"

csbuild.SetUserData("outputRootName", "{toolchainName}")
csbuild.SetOutputDirectory(f"_out/{outputSubPath}")
csbuild.SetIntermediateDirectory(f"_int/{outputSubPath}")

with csbuild.Toolchain("gcc", "clang", "msvc"):
	csbuild.SetUserData("outputRootName", f"{platform.system().lower()}-{{toolchainName}}-{{architectureName}}")

###################################################################################################

with csbuild.Target("debug"):
	csbuild.AddDefines("_DEBUG", "_HQ_CONFIG_DEBUG")
	csbuild.SetDebugLevel(csbuild.DebugLevel.ExternalSymbolsPlus)
	csbuild.SetOptimizationLevel(csbuild.OptimizationLevel.Disabled)
	csbuild.SetDebugRuntime(True)

with csbuild.Target("fastdebug"):
	csbuild.AddDefines("_DEBUG", "_HQ_CONFIG_FASTDEBUG")
	csbuild.SetDebugLevel(csbuild.DebugLevel.ExternalSymbolsPlus)
	csbuild.SetOptimizationLevel(csbuild.OptimizationLevel.Max)
	csbuild.SetDebugRuntime(True)

with csbuild.Target("release"):
	csbuild.AddDefines("NDEBUG", "_HQ_CONFIG_RELEASE")
	csbuild.SetDebugLevel(csbuild.DebugLevel.Disabled)
	csbuild.SetOptimizationLevel(csbuild.OptimizationLevel.Max)
	csbuild.SetDebugRuntime(False)

###################################################################################################

with csbuild.Platform("Windows"):
	with csbuild.Toolchain("gcc", "clang", "android-gcc", "android-clang"):
		csbuild.AddCompilerFlags("-fdiagnostics-format=msvc")

with csbuild.Platform("Linux"):
	with csbuild.Toolchain("clang"):
		csbuild.AddLinkerFlags("-stdlib=libstdc++")

with csbuild.Platform("Darwin"):
	with csbuild.Toolchain("clang"):
		csbuild.AddLinkerFlags("-stdlib=libc++")

with csbuild.Toolchain("msvc", "gcc", "clang", "android-gcc", "android-clang"):
	csbuild.SetCcLanguageStandard("c99")
	csbuild.SetCxxLanguageStandard("c++17")

with csbuild.Toolchain("android-gcc", "android-clang"):
	csbuild.SetAndroidTargetSdkVersion(26) # Android 7.0
	csbuild.SetAndroidStlLibType(AndroidStlLibType.LibCpp)

with csbuild.Toolchain("gcc", "clang", "android-gcc", "android-clang"):
	with csbuild.Architecture("x86", "x64"):
		csbuild.AddCompilerCxxFlags("-msse4.1")

	csbuild.AddCompilerFlags(
		"-fexceptions",
		"-fPIC",
	)

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

with csbuild.Toolchain("clang", "android-clang"):
	csbuild.AddCompilerFlags(
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

csbuild.AddDefines(
	"UNICODE",
	"_UNICODE",
)

csbuild.AddIncludeDirectories(
	# The library source root should be a global include path.
	f"{_REPO_ROOT_PATH}/src/lib",
)

_hq_setup_ps3.setupHqGlobalOptions()
_hq_setup_ps4.setupHqGlobalOptions()
_hq_setup_ps5.setupHqGlobalOptions()
_hq_setup_psvita.setupHqGlobalOptions()

###################################################################################################

def _setCommonLibOptions(libRootPath, outputName):
	def _applyStaticBuildOptions(outputName):
		csbuild.SetOutput(outputName, csbuild.ProjectType.StaticLibrary)

		with csbuild.Scope(csbuild.ScopeDef.All):
			csbuild.AddDefines("HQ_BUILD_STATIC_LIB")

	if os.getenv("HQ_FORCE_BUILD_STATIC"):
		# Detecting the environment override will force all platforms to build static libs.
		_applyStaticBuildOptions(outputName)

	else:
		# The libraries are built as DLLs by default.
		csbuild.SetOutput(outputName, csbuild.ProjectType.SharedLibrary)

		# Consoles should always build with static libs.
		with csbuild.Toolchain("ps3", "ps4", "ps5", "psvita"):
			_applyStaticBuildOptions(outputName)

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddSourceDirectories(libRootPath)

def _setCommonAppOptions(outputName):
	csbuild.SetOutput(outputName, csbuild.ProjectType.Application)

	with csbuild.Toolchain("msvc"):
		csbuild.SetMsvcSubsystem("CONSOLE")

	with csbuild.Toolchain("gcc", "clang"):
		with csbuild.Platform("Linux"):
			csbuild.AddLibraries(
				"pthread",
			)

def _setTestAppOptions(outputName):
	_setCommonAppOptions(outputName)

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

class ExtAntlr4Runtime(object):
	projectName = "ExtLib_Antlr4Runtime"
	outputName = "libantlr4"
	path = "external/antlr4/runtime/src"

with csbuild.Project(ExtAntlr4Runtime.projectName, ExtAntlr4Runtime.path, autoDiscoverSourceFiles=False):
	csbuild.SetOutput(ExtAntlr4Runtime.outputName, csbuild.ProjectType.SharedLibrary)

	csbuild.SetSupportedToolchains("msvc", "gcc", "clang")

	csbuild.AddSourceDirectories(ExtAntlr4Runtime.path)
	csbuild.AddIncludeDirectories(ExtAntlr4Runtime.path)
	csbuild.AddDefines("ANTLR4CPP_EXPORTS")

	with csbuild.Toolchain("msvc"):
		csbuild.AddCompilerFlags(
			"/wd4100", # 'foo': unreferenced formal parameter
			"/wd4244", # 'initializing': conversion from 'foo' to 'bar', possible loss of data
			"/wd4251", # class 'foo' needs to have dll-interface to be used by clients of class 'bar'
			"/wd4275", # non dll-interface class 'foo' used as base for dll-interface class 'bar'
			"/wd4996", # 'foo': Use the default constructor of bar instead
		)

	with csbuild.Scope(csbuild.ScopeDef.Children):
		csbuild.AddIncludeDirectories(ExtAntlr4Runtime.path)

###################################################################################################

class ExtGoogleTest(object):
	projectName = "ExtStub_gtest"
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
		)

###################################################################################################

class ExtXxHash(object):
	projectName = "ExtStub_xxhash"
	path = "external/xxHash"

with csbuild.Project(ExtXxHash.projectName, ExtXxHash.path, autoDiscoverSourceFiles=False):
	csbuild.SetOutput("{name}", csbuild.ProjectType.Stub)

	with csbuild.Scope(csbuild.ScopeDef.Children):
		csbuild.AddIncludeDirectories(ExtXxHash.path)

###################################################################################################

class HarlequinCommon(object):
	appRootPath = "src/app"
	libRootPath = "src/lib"
	testRootPath = "src/test"

###################################################################################################

class LibHarlequinBase(object):
	projectName = "HqLib_Base"
	outputName = "libhqbase"
	dependencies = [
		ExtXxHash.projectName,
	]

with csbuild.Project(LibHarlequinBase.projectName, HarlequinCommon.libRootPath, LibHarlequinBase.dependencies, autoDiscoverSourceFiles=False):
	_setCommonLibOptions(HarlequinCommon.libRootPath, LibHarlequinBase.outputName)

	_hq_setup_ps3.setupHqBaseLib()
	_hq_setup_ps4.setupHqBaseLib()
	_hq_setup_ps5.setupHqBaseLib()
	_hq_setup_psvita.setupHqBaseLib()

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddExcludeDirectories(
			f"{HarlequinCommon.libRootPath}/compiler",
			f"{HarlequinCommon.libRootPath}/runtime",
		)
	else:
		csbuild.AddSourceDirectories(f"{HarlequinCommon.libRootPath}/base")
		csbuild.AddExcludeDirectories(
			f"{HarlequinCommon.libRootPath}/base/clock-impl",
			f"{HarlequinCommon.libRootPath}/base/fiber-impl",
			f"{HarlequinCommon.libRootPath}/base/mutex-impl",
			f"{HarlequinCommon.libRootPath}/base/rwlock-impl",
			f"{HarlequinCommon.libRootPath}/base/system-impl",
			f"{HarlequinCommon.libRootPath}/base/thread-impl",
		)

	with csbuild.Toolchain("msvc", "gcc", "clang"):
		csbuild.AddDefines("HQ_BUILD_BASE_LIB_EXPORT")

	with csbuild.Toolchain("msvc"):
		csbuild.AddSourceFiles(
			f"{HarlequinCommon.libRootPath}/base/*-impl/*Win32.cpp",
		)

	with csbuild.Toolchain("gcc", "clang"):
		csbuild.AddSourceFiles(
			f"{HarlequinCommon.libRootPath}/base/*-impl/*Posix.cpp",
		)

	with csbuild.Platform("Darwin"):
		csbuild.AddDefines("_XOPEN_SOURCE=1")

	with csbuild.Scope(csbuild.ScopeDef.Final):
		with csbuild.Platform("Linux"):
			csbuild.AddLibraries("dl")

###################################################################################################

class LibHarlequinRuntime(object):
	projectName = "HqLib_Runtime"
	outputName = "libhqruntime"
	dependencies = [
		LibHarlequinBase.projectName,
	]

with csbuild.Project(LibHarlequinRuntime.projectName, HarlequinCommon.libRootPath, LibHarlequinRuntime.dependencies, autoDiscoverSourceFiles=False):
	_setCommonLibOptions(HarlequinCommon.libRootPath, LibHarlequinRuntime.outputName)

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddExcludeDirectories(
			f"{HarlequinCommon.libRootPath}/base",
			f"{HarlequinCommon.libRootPath}/common",
			f"{HarlequinCommon.libRootPath}/compiler",
		)

	else:
		csbuild.AddSourceDirectories(
			f"{HarlequinCommon.libRootPath}/runtime",
		)

	with csbuild.Scope(csbuild.ScopeDef.All):
		csbuild.AddDefines("HQ_LIB_RUNTIME")

	with csbuild.Toolchain("msvc", "gcc", "clang"):
		csbuild.AddDefines("HQ_BUILD_MAIN_LIB_EXPORT")

###################################################################################################

class LibHarlequinTool(object):
	projectName = "HqLib_Tool"
	outputName = "libhqtool"
	dependencies = [
		LibHarlequinBase.projectName,
		ExtAntlr4Runtime.projectName,
	]

with csbuild.Project(LibHarlequinTool.projectName, HarlequinCommon.libRootPath, LibHarlequinTool.dependencies, autoDiscoverSourceFiles=False):
	_setCommonLibOptions(HarlequinCommon.libRootPath, LibHarlequinTool.outputName)

	csbuild.SetSupportedToolchains("msvc", "gcc", "clang")

	with csbuild.Toolchain("msvc"):
		csbuild.AddCompilerFlags(
			"/wd4251", # class 'foo' needs to have dll-interface to be used by clients of class 'bar'
			"/wd4275", # non dll-interface class 'foo' used as base for dll-interface class 'bar'
		)

	if csbuild.GetRunMode() == csbuild.RunMode.GenerateSolution:
		csbuild.AddExcludeDirectories(
			f"{HarlequinCommon.libRootPath}/base",
			f"{HarlequinCommon.libRootPath}/common",
			f"{HarlequinCommon.libRootPath}/runtime",
		)

	else:
		csbuild.AddSourceDirectories(
			f"{HarlequinCommon.libRootPath}/compiler",
		)

	with csbuild.Scope(csbuild.ScopeDef.All):
		csbuild.AddDefines("HQ_LIB_TOOL")

	with csbuild.Toolchain("msvc", "gcc", "clang"):
		csbuild.AddDefines("HQ_BUILD_MAIN_LIB_EXPORT")

###################################################################################################

class HarlequinCompiler(object):
	projectName = "HqApp_Compiler"
	outputName = "hqc"
	path = f"{HarlequinCommon.appRootPath}/compiler"
	dependencies = [
		LibHarlequinTool.projectName
	]

with csbuild.Project(HarlequinCompiler.projectName, HarlequinCompiler.path, HarlequinCompiler.dependencies):
	_setCommonAppOptions(HarlequinCompiler.outputName)

	csbuild.SetSupportedToolchains("msvc", "gcc", "clang")

###################################################################################################

class HarlequinRuntime(object):
	projectName = "HqApp_Runtime"
	outputName = "hq"
	path = f"{HarlequinCommon.appRootPath}/runtime"
	dependencies = [
		LibHarlequinRuntime.projectName
	]

with csbuild.Project(HarlequinRuntime.projectName, HarlequinRuntime.path, HarlequinRuntime.dependencies):
	_setCommonAppOptions(HarlequinRuntime.outputName)

	_hq_setup_ps3.setupHqRuntimeApp()
	_hq_setup_ps4.setupHqRuntimeApp()
	_hq_setup_ps5.setupHqRuntimeApp()
	_hq_setup_psvita.setupHqRuntimeApp()

###################################################################################################

class HarlequinUnitTest(object):
	projectName = "HqTest_UnitTest"
	outputName = "hq_unit_test"
	dependencies = [
		ExtGoogleTest.projectName,
		LibHarlequinTool.projectName,
		LibHarlequinRuntime.projectName,
	]

with csbuild.Project(HarlequinUnitTest.projectName, HarlequinCommon.testRootPath, HarlequinUnitTest.dependencies, autoDiscoverSourceFiles=False):
	_setTestAppOptions(HarlequinUnitTest.outputName)

	csbuild.AddSourceFiles(
		f"{HarlequinCommon.testRootPath}/Main.cpp",
	)
	csbuild.AddSourceDirectories(
		f"{HarlequinCommon.testRootPath}/common",
		f"{HarlequinCommon.testRootPath}/unit_test"
	)

###################################################################################################

class HarlequinFunctionalTest(object):
	projectName = "HqTest_FunctionalTest"
	outputName = "hq_func_test"
	dependencies = [
		ExtGoogleTest.projectName,
		LibHarlequinTool.projectName,
		LibHarlequinRuntime.projectName,
	]

with csbuild.Project(HarlequinFunctionalTest.projectName, HarlequinCommon.testRootPath, HarlequinFunctionalTest.dependencies, autoDiscoverSourceFiles=False):
	_setTestAppOptions(HarlequinFunctionalTest.outputName)

	csbuild.AddSourceFiles(
		f"{HarlequinCommon.testRootPath}/Main.cpp",
	)
	csbuild.AddSourceDirectories(
		f"{HarlequinCommon.testRootPath}/common",
		f"{HarlequinCommon.testRootPath}/functional_test",
	)
	csbuild.AddExcludeDirectories(
		f"{HarlequinCommon.testRootPath}/functional_test/op_codes/native",
	)

###################################################################################################

class HarlequinTest_OpCodesNative(object):
	projectName = "HqTest_NativeImpl_OpCodes"
	outputName = "libTestOpCodes"
	path = f"{HarlequinCommon.testRootPath}/functional_test/op_codes/native"
	dependencies = [
		ExtGoogleTest.projectName,
		LibHarlequinRuntime.projectName,
	]

with csbuild.Project(HarlequinTest_OpCodesNative.projectName, HarlequinTest_OpCodesNative.path, HarlequinTest_OpCodesNative.dependencies):
	csbuild.SetOutput(HarlequinTest_OpCodesNative.outputName, csbuild.ProjectType.SharedLibrary)
	csbuild.SetSupportedToolchains("msvc", "gcc", "clang")

###################################################################################################
