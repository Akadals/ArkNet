#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <unordered_map>

#include <AkaNetCore/Logger.h>


namespace AkaNetCore
{
    class Command
    {
    private:
        std::unordered_map<std::string, std::function<void()>> commands;

    public:
        Command();
        template<typename F>
        void Register(std::string_view name, F&& func)
        {
            auto [it, inserted] =
                commands.emplace(name, std::function<void()>(std::forward<F>(func)));

            if (!inserted)
                LOG_ERROR(std::string(name) + " already registered", ANCERRGOOD);
        }
        void operator[] (const std::string& cmd);
        void Excute(const std::string& cmd);
    };
#pragma region Basic Commands
	void TestCmd();
    void ServerDown();
#pragma endregion
	inline Command command;
}