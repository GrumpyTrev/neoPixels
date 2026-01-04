#include "ObjectStore.hpp"

using namespace std;

namespace Lights
{
	/// @brief Add an object to the map
	/// @param name
	/// @param object
	void ObjectStore::AddObject(string name, BaseDefinedObject *object)
	{
		object->Name(name);
		definedObjects[name] = object;
	}

	/// @brief Retrieve the specified object from the map
	/// @param name
	/// @return
	BaseDefinedObject *ObjectStore::GetObject(string name)
	{
		map<string, BaseDefinedObject *>::iterator it = definedObjects.find(name);
		return (it != definedObjects.end()) ? it->second : nullptr;
	}
}