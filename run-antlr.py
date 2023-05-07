#!/usr/bin/env python3
#
# Copyright (c) 2023, Zoe J. Bare
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

import argparse
import os
import shutil
import subprocess
import sys
import tempfile

from abc import ABC, abstractmethod

########################################################################################################################

THIS_PATH = os.path.abspath(os.path.dirname(__file__))

########################################################################################################################

class Tool(ABC):
	_classPath = None

	def __init__(self, toolName):
		print(f"Selected '{toolName}' tool")

	@staticmethod
	def _getClassPath():
		if not Tool._classPath:
			antlrFilePath = os.path.abspath(os.path.join(THIS_PATH, "external", "antlr4", "antlr-4.12.0-complete.jar"))
			assert os.access(antlrFilePath, os.F_OK), "Antlr4 tool package does not exist"

			# Augment the 'CLASSPATH' env variable to include the path to the Antlr tool package.
			classPathKey = "CLASSPATH"
			classPathArgs = [x for x in [antlrFilePath, os.environ.get(classPathKey, None)] if x]

			Tool._classPath = os.path.pathsep.join(classPathArgs)

		return Tool._classPath

	@abstractmethod
	def invoke(self, grammarFilePath, sourceFilePath, ruleName):
		pass

########################################################################################################################

class GeneratorTool(Tool):
	def __init__(self, toolName):
		Tool.__init__(self, toolName)

	def invoke(self, grammarFilePath, _, __):
		outputPath = os.path.join(THIS_PATH, "src", "lib", "tool", "compiler", "generated")
		classPath = Tool._getClassPath()

		print(
			f"Grammer path: {grammarFilePath}\n"
			f"Output path:  {outputPath}\n"
		)

		if os.access(outputPath, os.F_OK):
			print("Removing old parser code ...")
			shutil.rmtree(outputPath, ignore_errors=True)

		print("Generating parser C++ source code ...")
		return subprocess.call([
			"java",
			"-cp", classPath,
			"org.antlr.v4.Tool",
			"-no-listener",
			"-visitor",
			"-Dlanguage=Cpp",
			"-o", outputPath,
			grammarFilePath
		])

########################################################################################################################

class ParseTool(Tool):
	def __init__(self, toolName):
		Tool.__init__(self, toolName)

		self._parseOption = {
			"parse-gui": "-gui",
			"parse-tree": "-tree",
			"parse-tokens": "-tokens",
			"parse-trace": "-trace",
		}.get(toolName, None)
		assert self._parseOption, f"Unknown parse tool: {toolName}"

	def invoke(self, grammarFilePath, sourceFilePath, ruleName):
		currentPath = os.getcwd()
		tempPath = tempfile.mkdtemp()
		classPath = os.path.pathsep.join([tempPath, Tool._getClassPath()])
		grammarName = os.path.splitext(os.path.basename(grammarFilePath))[0]

		print(
			f"Grammer path: {grammarFilePath}\n"
			f"Source path:  {sourceFilePath}\n"
			f"Temp path:    {tempPath}\n"
			f"Rule name:    {ruleName}\n"
		)

		try:
			print("Generating temporary parser ...")
			result = subprocess.call([
				"java",
				"-cp", classPath,
				"org.antlr.v4.Tool",
				"-no-listener",
				"-no-visitor",
				"-o", tempPath,
				grammarFilePath
			])

			if result == 0:
				try:
					# Move to the temporary path so we can compile the parser.
					os.chdir(tempPath)

					print("Compiling temporary parser ...")
					result = subprocess.call(["javac", "-cp", classPath, "*.java"])

				finally:
					# Move back to the original path.
					os.chdir(currentPath)

				if result == 0:
					print("Parsing source file ...")
					result = subprocess.call([
						"java",
						"-cp", classPath,
						"org.antlr.v4.gui.TestRig",
						grammarName,
						ruleName,
						self._parseOption,
						sourceFilePath
					])

		finally:
			# Always delete the temporary path after everything is done.
			# This should also be done when errors occur.
			shutil.rmtree(tempPath)

		return result

########################################################################################################################

def _verifyJavaIsInstalled():
	# Check the Java runtime is accessible.
	result = subprocess.check_call(["java", "-version"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	assert result == 0, "Failed to find Java runtime executable in system path"

	# Check the Java compiler is accessible.
	result = subprocess.check_call(["javac", "-version"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	assert result == 0, "Failed to find Java compiler executable in system path"

########################################################################################################################

def main():
	tools = {
		"generate": GeneratorTool,
		"parse-gui": ParseTool,
		"parse-tree": ParseTool,
		"parse-tokens": ParseTool,
		"parse-trace": ParseTool,
	}
	parser = argparse.ArgumentParser()

	parser.add_argument(
		"tool",
		action="store",
		type=str,
		choices=sorted(tools.keys()),
		help="Antlr tool to invoke",
	)
	parser.add_argument(
		"-s", "--source",
		action="store",
		type=str,
		help="File path to the source file which will be tested in the selected 'Parse' tool",
	)
	parser.add_argument(
		"-r", "--rule",
		action="store",
		type=str,
		help="Top level rule to begin with in the selected 'Parse' tool"
	)

	args = parser.parse_args()

	# Make sure the user has java installed before doing anything else.
	_verifyJavaIsInstalled()

	tool = tools.get(args.tool)(args.tool)
	grammarFilePath = os.path.join(THIS_PATH, "Harlequin.g4")
	sourceFilePath = os.path.abspath(args.source) if args.source else None
	ruleName = args.rule

	if isinstance(tool, ParseTool):
		assert sourceFilePath, "Source file path must be provided for all 'Parse' tools"
		assert ruleName, "Rule name must be provided for all 'Parse' tools"

	# Run the selected tool.
	result = tool.invoke(grammarFilePath, sourceFilePath, ruleName)
	if result == 0:
		print("... done!")

	return result

########################################################################################################################

if __name__ == "__main__":
	sys.exit(main())
