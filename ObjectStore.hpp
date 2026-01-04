#pragma once

#include <map>
#include "BaseDefinedObject.hpp"

using namespace std;

namespace Lights
{
	/// @brief Used to store a mapping between object and BaseDefinedObject instance
	class ObjectStore
	{
	public:
		/// @brief Add an object to the map
		/// @param name
		/// @param object
		void AddObject(string name, BaseDefinedObject *definedObject);

		/// @brief Retrieve the specified object from the map
		/// @param name
		/// @return
		BaseDefinedObject *GetObject(string name);

	private:
		/// @brief Mapping from object name to object
		map<string, BaseDefinedObject *> definedObjects;
	};
}