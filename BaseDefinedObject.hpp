#pragma once
#include <string>
#include <pico/time.h>

using namespace std;

namespace Lights
{
	/// @brief This is the base class for all script defined objects.
	/// It is used to allow all such objects to be stored and accessed
	class BaseDefinedObject
	{
	public:
		inline BaseDefinedObject() {};

		/// @brief Copy constructor
		/// @param rhs
		inline BaseDefinedObject(const BaseDefinedObject &rhs) { name = rhs.name; }

		inline virtual ~BaseDefinedObject() {};

		inline string Name() { return name; }
		inline void Name(string objectName) { name = objectName; }

	private:
		/// @brief Name given to this object
		string name;
	};
}