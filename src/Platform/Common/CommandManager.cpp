#include <AkaNetCore/CommandManager.h>

AkaNetCore::Command::Command()
{
	commands["Test"] = TestCmd;
}
void AkaNetCore::Command::operator[](const std::string& cmd)
{
	if (!commands.contains(cmd))
		LOG_WARNING(cmd + " is an unregistered command.");
	else commands[cmd]();
}
void AkaNetCore::Command::Excute(const std::string& cmd)
{

}
void AkaNetCore::TestCmd()
{
	LOG_INFO("Test");
}
