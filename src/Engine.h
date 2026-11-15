#pragma once

#include "Wrapper.h"
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

class Engine {
private:
    std::map<std::string, WrapperBase*> m_commands;
    std::map<std::string, std::unique_ptr<WrapperBase>> m_owned_commands;

public:
    void register_command(WrapperBase* wrapper_ptr, const std::string& alias) {
        if (wrapper_ptr == nullptr) {
            throw std::invalid_argument("Cannot register a null command");
        }
        if (alias.empty()) {
            throw std::invalid_argument("Command alias must not be empty");
        }
        const auto inserted = m_commands.emplace(alias, wrapper_ptr);
        if (!inserted.second) {
            throw std::invalid_argument("Command is already registered: " + alias);
        }
    }

    void register_command(std::unique_ptr<WrapperBase> wrapper,
        const std::string& alias) {
        if (!wrapper) {
            throw std::invalid_argument("Cannot register a null command");
        }
        if (alias.empty()) {
            throw std::invalid_argument("Command alias must not be empty");
        }
        if (m_commands.find(alias) != m_commands.end()) {
            throw std::invalid_argument("Command is already registered: " + alias);
        }

        WrapperBase* command = wrapper.get();
        m_owned_commands.emplace(alias, std::move(wrapper));
        m_commands.emplace(alias, command);
    }

    std::string execute(const std::string& alias,
        const Arguments& arguments = {}) const {
        const auto it = m_commands.find(alias);
        if (it == m_commands.end()) {
            throw std::invalid_argument("Unknown command: " + alias);
        }
        return it->second->execute(arguments);
    }
};
