#pragma once

#include <iostream>
#include <vector>
#include <memory>


// 3. Receiver 类 (例如，灯)
class Light {
public:
    void on() {
        std::cout << "Light is on" << std::endl;
    }
    void off() {
        std::cout << "Light is off" << std::endl;
    }
};

// 1. Command 接口
class Command {
    public:
        virtual ~Command() = default;
        virtual void execute() = 0;
    };

    // 2. ConcreteCommand 类 (例如，打开灯的命令)
    class LightOnCommand : public Command {
    public:
        LightOnCommand(Light& light) : light_(light) {}
        void execute() override {
            light_.on();
        }
    private:
        Light& light_;
};


// 4. Invoker 类
class RemoteControl {
public:
    void setCommand(std::unique_ptr<Command> command) {
        command_ = std::move(command);
    }
    void pressButton() {
        if (command_) {
            command_->execute();
        }
    }
private:
    std::unique_ptr<Command> command_;
};

