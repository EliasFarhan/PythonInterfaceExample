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

#ifndef SFGE_PYENGINE_H
#define SFGE_PYENGINE_H

#include <engine/engine.h>

#include <python/python_engine.h>
#include <pybind11/embed.h>// everything needed for embedding
namespace py = pybind11;

namespace sfge
{

class PyComponent : public Component
{
public:
	using Component::Component;

	void Init() override
	{
		PYBIND11_OVERLOAD(
			void,
			Component,
			Init,
		);
	}
	void Update() override
	{
		PYBIND11_OVERLOAD(
			void,
			Component,
			Update,
		);
	}
};

/**
* \brief Manage the python interpreter
*/
class PythonManager
{
public:
	/**
	* \brief Initialize the python interpreter
	*/
	void Init();

	py::object LoadFile(std::string script_name);
	/**
	* \brief Finalize the python interpreter
	*/
	void Destroy();
private:

};
}
#endif /* SFGE_PYENGINE_H */
