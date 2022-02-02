#include <iostream>
#include "thread_pool.h"

class Person
{
public:
    Person() = default;
    Person(const uint64_t id, const std::string& name, const uint16_t age)
        : m_id(id)
        , m_name(name)
        , m_age(age)
    {}

    void print_name(const std::string& msg) const 
    { 
        std::cout << m_name + msg << std::endl; 
    }

public:
    uint64_t m_id;
    std::string m_name;
    uint16_t m_age;
};

int main()
{
    /*Message m{ 1, std::tuple<std::string, uint16_t>{} };
    std::shared_ptr<MessageBase> pMsg2{ std::move(make_message<std::string, uint64_t, std::string>(2, "tom", 22, "China")) };

    Router r;
    r.register_handler(2, [](const std::string str, const uint64_t, const std::string&) {std::cout << str << std::endl; });
    r.dispach(pMsg2);

    Person p(1, "zhangsan", 22);
    r.register_handler(1, [](const Person& p, const std::string& msg) {
        std::cout << "id: " << p.m_id << std::endl;
        std::cout << "name: " << p.m_name << std::endl; 
        std::cout << "age: " << p.m_age << std::endl;
        std::cout << "msg: " << msg << std::endl;
        });
    std::shared_ptr<MessageBase> pMsg1{ make_message<Person, std::string>(1, p, "China") };
    r.dispach(pMsg1);

    r.register_handler(3, &Person::print_name, &p);
    std::shared_ptr<MessageBase> pMsg3{ make_message<std::string>(3,"China") };
    r.dispach(pMsg3);*/

    MultiplePool pool;
    pool.register_handler(1, [](const Person& p, const std::string& msg) {
        std::cout << "id: " << p.m_id << std::endl;
        std::cout << "name: " << p.m_name << std::endl;
        std::cout << "age: " << p.m_age << std::endl;
        std::cout << "msg: " << msg << std::endl;
        });

    pool.register_handler(2, [](const std::string str, const uint64_t, const std::string&) {std::cout << str << std::endl; });

    Person p(1, "zhangsan", 22);
    std::shared_ptr<Person> pPtr{ std::make_shared<Person>(2, "lisi", 23) };
    pool.register_handler(3, &Person::print_name, pPtr);

    std::shared_ptr<MessageBase> pMsg1{ make_message<Person, std::string>(1, p, "China") };
    std::shared_ptr<MessageBase> pMsg2{ std::move(make_message<std::string, uint64_t, std::string>(2, "tom", 22, "China")) };
    std::shared_ptr<MessageBase> pMsg3{ make_message<std::string>(3,"China") };

    pool.schedule_by_id(pMsg1);
    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    pool.schedule_by_id(pMsg2);
    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    pool.schedule_by_id(pMsg3);

    return 0;
}
