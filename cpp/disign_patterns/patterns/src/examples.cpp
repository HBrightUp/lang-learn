#include<iostream>
#include<memory>
#include"../include/examples.h"
#include"../include/factory.hpp"
#include"../include/stratagy.h"
#include"../include/single.h"
#include"../include/adapter.h"
#include"../include/builder.h"
#include"../include/prototype.h"
#include"../include/decorator.h"
#include"../include/proxy.h"
#include"../include/facade.h"
#include"../include/bridge.h"
#include"../include/composite.h"
#include"../include/flyweight.h"
#include"../include/template.h"
#include"../include/command.h"
#include"../include/iterator.h"
#include"../include/observer.h"
#include"../include/mediator.h"
#include"../include/memento.h"
#include"../include/state.h"
#include"../include/visitor.h"
#include"../include/responsibility.h"

void call_factory() {

   ProductRegistrar<Shoes, NiKeShoes> nikeShoes("nike");
   std::shared_ptr<Shoes> pNiKeShoes(ProductFactory<Shoes>::Instance().GetProduct("nike"));
   pNiKeShoes->Show();


   ProductRegistrar<Clothe, UniqloClothe> adidasShoes("uniqlo");
   std::unique_ptr<Clothe> pUniqloClothe(ProductFactory<Clothe>::Instance().GetProduct("uniqlo"));
   pUniqloClothe->Show();
}

void call_stratagy() {
       
    Context context(std::make_unique<ConcreteStrategyA>());
    std::cout << context.executeStrategy("data") << std::endl;

    context.setStrategy(std::make_unique<ConcreteStrategyB>());
    std::cout << context.executeStrategy("data") << std::endl;
}

void call_single() {
    Singleton& instance_1 = Singleton::get_instance();
    Singleton& instance_2 = Singleton::get_instance();
}
void call_adapter() {

    // using methon-1
    std::shared_ptr<Target>  t1 = std::make_shared<ClassAdapter>();
    t1->request(); //  Adaptee::specificRequest()

    std::unique_ptr<Adaptee> ada(new Adaptee());
    std::unique_ptr<Target> t2(new ObjectAdapter(ada.get()));
    t2->request(); // Adaptee::specificRequest()
}

void call_builder() {
    
    std::unique_ptr<Director> director(new Director);

    //product
	House *house;
 
    // builder A
	std::unique_ptr<AbstractBuilder>  abuilder(new ConcreteBuilderA());
	director->setBuilder(abuilder.get());
	house = director->construct();
	house->printfHouseInfo();
 
    // build B
	std::unique_ptr<AbstractBuilder> bbuilder(new ConcreteBuilderB());
	director->setBuilder(bbuilder.get());
	house = director->construct();
	house->printfHouseInfo();

}

void call_prototype(){
    std::unique_ptr<ConcretePrototypeA> prototypeA = std::make_unique<ConcretePrototypeA>("prototype_A");
    std::unique_ptr<Prototype> cloneA = prototypeA->clone();

    prototypeA->show();
    cloneA->show();
}

void call_decorator() {
    std::unique_ptr<Component>  component(new ConcreteComponent());
    //Component* decoratedComponentA = new ConcreteDecoratorA(component);
    //Component* decoratedComponentB = new ConcreteDecoratorB(decoratedComponentA);
    std::unique_ptr<Component>  decoratedComponentA(new ConcreteDecoratorA(component.get()));
    std::unique_ptr<Component>  decoratedComponentB(new ConcreteDecoratorB(decoratedComponentA.get()));

    decoratedComponentB->operation(); // 调用时会按照装饰器的顺序依次执行功能
    // ...
    
  
}

void call_proxy() {
    RealProxy proxy("user", "pwd");
    proxy.Run();
}

void call_facade() {
     Facade facade;
    facade.Operation();
}

void call_bridge() {
    Implementor* implA = new ConcreteImplementorA();
    Abstraction* absA = new RefinedAbstraction(implA);
    absA->operation(); // 输出: ConcreteImplementorA operation

    Implementor* implB = new ConcreteImplementorB();
    Abstraction* absB = new RefinedAbstraction(implB);
    absB->operation(); // 输出: ConcreteImplementorB operation

    delete implA;
    delete absA;
    delete implB;
    delete absB;
}

void call_composite() {
    // 创建叶子节点
    Leaf* leaf1 = new Leaf("Leaf1");
    Leaf* leaf2 = new Leaf("Leaf2");

    // 创建容器节点
    Composite* composite = new Composite();
    composite->add(leaf1);
    composite->add(leaf2);

    // 创建另一个容器节点
    Composite* composite2 = new Composite();
    composite2->add(new Leaf("Leaf3"));
    composite->add(composite2);

    // 客户端使用
    composite->operation();

    // 释放内存 (实际开发中需要更完善的内存管理)
    delete leaf1;
    delete leaf2;
    delete composite;
    delete composite2;

}
void call_flyweight() {
    //std::map<std::string, Font *> fontPool;
    FlyWeightFactory fly;

    auto f1 = fly.GetFont("1");
    f1->show(); //  key: 1

    auto f2 = fly.GetFont("2");
    f2->show(); //  key: 2

    auto f3 = fly.GetFont("1");
    f3->show(); //  key: 1
}

void call_template() {
    AbstractClass* obj1 = new ConcreteClass1();
    obj1->templateMethod();
    delete obj1;

    AbstractClass* obj2 = new ConcreteClass2();
    obj2->templateMethod();
    delete obj2;

}

void call_command() {

    // command receiver
    Light light;
    // create a command
    auto lightOn = std::make_unique<LightOnCommand>(light);
    // 创建调用者 (遥控器)
    RemoteControl remote;
    // 将命令设置到遥控器
    remote.setCommand(std::move(lightOn));
    // 按下按钮 (执行命令)
    remote.pressButton(); // 输出: Light is on
}

void call_iterator() {
    List list;
    list.add(1);
    list.add(2);
    list.add(3);

    auto iterator = list.create_iterator();

    while (!iterator->is_done()) {
        std::cout << iterator->next() << " ";
    }
    std::cout << std::endl; // Output: 1 2 3

}

void call_observer() {
    Subject *subject = new Subject;
    Observer *observer1 = new Observer(*subject);
    Observer *observer2 = new Observer(*subject);
    Observer *observer3 = new Observer(*subject);
    Observer *observer4;
    Observer *observer5;

    subject->CreateMessage("Hello World! :D");
    observer3->RemoveMeFromTheList();

    subject->CreateMessage("The weather is hot today! :p");
    observer4 = new Observer(*subject);

    observer2->RemoveMeFromTheList();
    observer5 = new Observer(*subject);

    subject->CreateMessage("My new car is great! ;)");
    observer5->RemoveMeFromTheList();

    observer4->RemoveMeFromTheList();
    observer1->RemoveMeFromTheList();

    delete observer5;
    delete observer4;
    delete observer3;
    delete observer2;
    delete observer1;
    delete subject;
}

void call_mediator(){
    Component1 *c1 = new Component1;
    Component2 *c2 = new Component2;
    ConcreteMediator *mediator = new ConcreteMediator(c1, c2);
    std::cout << "Client triggers operation A.\n";
    c1->DoA();
    std::cout << "\n";
    std::cout << "Client triggers operation D.\n";
    c2->DoD();

    delete c1;
    delete c2;
    delete mediator;
}

void call_memento() {
    Originator originator;
    Caretaker caretaker;

    originator.setState("State #1");
    caretaker.addMemento(originator.createMemento());

    originator.setState("State #2");
    caretaker.addMemento(originator.createMemento());

    originator.setState("State #3");
    caretaker.addMemento(originator.createMemento());

    std::cout << "Current State: " << originator.getState() << std::endl;

    originator.restoreMemento(caretaker.getMemento(1));
    std::cout << "Restored to State: " << originator.getState() << std::endl;

    originator.restoreMemento(caretaker.getMemento(0));
    std::cout << "Restored to State: " << originator.getState() << std::endl;
}

void call_state() {
    ContextA *context = new ContextA(new ConcreteStateA);
    context->Request1();
    context->Request2();
    delete context;
}

void call_visitor() {
    using namespace sv;

    
    std::array<const sv::Component *, 2> components = {new sv::ConcreteComponentA, new sv::ConcreteComponentB};
    std::cout << "The client code works with all visitors via the base Visitor interface:\n";
    ConcreteVisitor1 *visitor1 = new ConcreteVisitor1;
    sv::ClientCode(components, visitor1);
    std::cout << "\n";
    std::cout << "It allows the same client code to work with different types of visitors:\n";
    ConcreteVisitor2 *visitor2 = new ConcreteVisitor2;
    ClientCode(components, visitor2);

    for (const sv::Component *comp : components) {
    delete comp;
    }
    delete visitor1;
    delete visitor2;
}

void call_responsibility() {
    MonkeyHandler *monkey = new MonkeyHandler;
    SquirrelHandler *squirrel = new SquirrelHandler;
    DogHandler *dog = new DogHandler;
    monkey->SetNext(squirrel)->SetNext(dog);

    /**
     * The client should be able to send a request to any handler, not just the
     * first one in the chain.
     */
    std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
    ClientCode(*monkey);
    std::cout << "\n";
    std::cout << "Subchain: Squirrel > Dog\n\n";
    ClientCode(*squirrel);

    delete monkey;
    delete squirrel;
    delete dog;
}

void call_examples() {
    //call_factory();
    //call_stratagy();
    //call_single();
    //call_adapter();
    //call_builder();
    //call_prototype();
    //call_decorator();
    //call_proxy();
    //call_facade();
    //call_bridge();
    //call_composite();
    //call_flyweight();
    //call_template();
    //call_command();
    //call_iterator();
    //call_observer();
    //call_mediator();
    //call_memento();
    //all_state();
    //call_visitor();
    call_responsibility();
}






