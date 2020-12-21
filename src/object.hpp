#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
namespace Alchemy {
    class Object {
        public:
            Object () : uuid(boost::uuids::random_generator()()) {};

            bool operator==(Object const& rhs) const {
                return uuid == rhs.uuid;
            }

            ~Object () {}
        private:
            boost::uuids::uuid uuid;
    };
}