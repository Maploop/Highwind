#pragma once

#include <map>
#include <string>
#include <iostream>

class Mesh;
class CacheHolder
{
public:
	static std::map<std::string, Mesh*> cached_mesh_data;

	static Mesh* find_cached_mesh(std::string pathname)
	{
		return cached_mesh_data[pathname];
	}

	static bool is_mesh_cached(std::string pathname)
	{
		return find_cached_mesh(pathname) != nullptr;
	}

	static void cache_mesh_data(std::string path, Mesh* mesh)
	{
		cached_mesh_data.insert({path, mesh});
	}
};