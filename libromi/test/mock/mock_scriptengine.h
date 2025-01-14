#include "gmock/gmock.h"
#include "ui/IScriptEngine.h"
#include "rover/Rover.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockScriptEngine : public romi::IScriptEngine<romi::Rover>
{
public:
        MOCK_METHOD(int, get_next_event, (), (override));
        MOCK_METHOD(void, execute_script, (romi::Rover& target, size_t index), (override));
        MOCK_METHOD(romi::ScriptList&, scriptList, (), (override));
};
#pragma GCC diagnostic pop