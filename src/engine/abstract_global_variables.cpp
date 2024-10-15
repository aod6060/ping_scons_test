#include "sys.h"

namespace engine {
        // Getters
        int32_t AbstractGlobalVariables::getIntVar(std::string name) {
            return this->int_var.at(name);
        }

        uint32_t AbstractGlobalVariables::getUIntVar(std::string name) {
            return this->uint_var.at(name);
        }

        float AbstractGlobalVariables::getFloatVar(std::string name) {
            return this->float_var.at(name);
        }

        bool AbstractGlobalVariables::getBoolVar(std::string name) {
            return this->bool_var.at(name);
        }

        std::string AbstractGlobalVariables::getStringVar(std::string name) {
            return this->string_var.at(name);
        }

        // Setters
        void AbstractGlobalVariables::setIntVar(std::string name, int32_t value) {
            this->int_var[name] = value;
        }

        void AbstractGlobalVariables::setUIntVar(std::string name, uint32_t value) {
            this->uint_var[name] = value;
        }

        void AbstractGlobalVariables::setFloatVar(std::string name, float value) {
            this->float_var[name] = value;
        }

        void AbstractGlobalVariables::setBoolVar(std::string name, bool value) {
            this->bool_var[name] = value;
        }

        std::string AbstractGlobalVariables::getStringVar(std::string name, std::string value) {
            this->string_var[name] = value;
        }


}