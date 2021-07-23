#include "gmock/gmock.h"
#include "ui/IScriptList.h"
#include "ui/Script.h"
#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockScriptList : public romi::IScriptList
{
    public:
        MOCK_CONST_METHOD0(json_scripts, std::string());
        MOCK_METHOD1(SubscriptOperator, romi::Script& (size_t index));
        MOCK_CONST_METHOD0(empty, bool());
        MOCK_CONST_METHOD0(size, size_t());
        virtual romi::Script& operator [] (size_t index) override { return SubscriptOperator(index); }
};
#pragma GCC diagnostic pop