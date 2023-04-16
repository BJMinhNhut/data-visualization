#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP

#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

template <typename Resource, typename Identifier>
class ResourceHolder {
	public:
		void load(Identifier id, const std::string& filename);

		Resource& get(Identifier id);
		const Resource& get(Identifier id) const;

	private:
		void insertResource(Identifier id, std::unique_ptr<Resource> resource);

	private:
		std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include "ResoureHolder.inl"
#endif // RESOURCEHOLDER_HPP