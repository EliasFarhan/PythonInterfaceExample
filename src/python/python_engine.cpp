/*
 MIT License
 
 Copyright (c) 2017 SAE Institute Switzerland AG
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <engine/engine.h>
#include <python/python_engine.h>
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <regex>

#include <experimental/filesystem>
// for convenience
namespace fs = std::experimental::filesystem;

namespace sfge
{

PYBIND11_EMBEDDED_MODULE(SFGE, m)
{
	py::class_<GameObject> game_object(m, "GameObject");
	py::class_<Component, PyComponent> component(m, "Component");
	component.def(py::init<>());
	component.def("Init", &Component::Init);

}

py::object import(const std::string& module, const std::string& path, py::object& globals)
{
    py::dict locals;
    locals["module_name"] = py::cast(module); // have to cast the std::string first
    locals["path"]        = py::cast(path);

    py::eval<py::eval_statements>(            // tell eval we're passing multiple statements
        "import imp\n"
        "new_module = imp.load_module(module_name, open(path), path, ('py', 'U', imp.PY_SOURCE))\n",
        globals,
        locals);

    return locals["new_module"];
}

std::string module2class(std::string& module_name)
{

	std::istringstream iss(module_name);
	std::string token;
	std::string class_name;
	while (std::getline(iss, token, '_'))
	{
		if(token.size() > 0)
		{
			char first = token.at(0);
			class_name += std::toupper(first);
			class_name += token.substr(1, token.size());
		}
	}
	return class_name;
}

void PythonManager::Init()
{
	py::initialize_interpreter();
}

void PythonManager::LoadFile(std::string script_path)
{


	fs::path p = script_path;
	if(fs::is_regular_file(p))
	{
		try
		{
			std::string module_name = p.filename().replace_extension(fs::path());
			std::string class_name = module2class(module_name);
			py::object globals  = py::globals();
			py::object module   = import(module_name, script_path, globals);
			std::cout << "Module imported: "<< py::str(module).cast<std::string>() <<"\n";
			py::object ComponentClass = module.attr(class_name.c_str());
			std::cout << "Class name: "<< py::str(ComponentClass).cast<std::string>() <<"\n";
			py::object component = ComponentClass();
			std::cout << "Created new Component\n";
			component.attr("Init")();
		}
		catch(const py::error_already_set& e)
		{
		    std::cerr << ">>> Error! Uncaught exception: \n";
		    std::cerr << e.what() << "\n";
		    PyErr_Print();

		}
	}

}
void PythonManager::Destroy()
{
	py::finalize_interpreter();
}

}
