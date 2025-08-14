#include<iostream>
#include<fstream>
#include<person.pb.h>


int main() {

    // 创建 Person 对象
    com::example::Person person;
    person.set_name("John Doe");
    person.set_id(123);
    person.set_email("john.doe@example.com");

    // 序列化到字符串
    std::string serialized_string;
    person.SerializeToString(&serialized_string);

    // 打印序列化后的数据（二进制形式）
    std::cout << "Serialized data: " << serialized_string << std::endl;

    // 反序列化
    com::example::Person deserialized_person;
    deserialized_person.ParseFromString(serialized_string);

    // 打印反序列化后的数据
    std::cout << "Name: " << deserialized_person.name() << std::endl;
    std::cout << "ID: " << deserialized_person.id() << std::endl;
    std::cout << "Email: " << deserialized_person.email() << std::endl;


    // 序列化到文件
    std::fstream output("person.data", std::ios::out | std::ios::binary);
    if (!person.SerializeToOstream(&output)) {
        std::cerr << "Failed to write person data to file." << std::endl;
        return -1;
    }
    output.close();

    // 从文件反序列化
    std::fstream input("person.data", std::ios::in | std::ios::binary);
    com::example::Person file_person;
    if (!file_person.ParseFromIstream(&input)) {
        std::cerr << "Failed to read person data from file." << std::endl;
        return -1;
    }
    input.close();
        
    std::cout << "Name from file: " << file_person.name() << std::endl;
    std::cout << "ID from file: " << file_person.id() << std::endl;
    std::cout << "Email from file: " << file_person.email() << std::endl;

    return 0;
}