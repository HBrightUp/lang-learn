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

    std::shared_ptr<CTarget>  t1 = std::make_shared<CAdapter>();
    t1->request();  

    std::unique_ptr<Adaptee> ada(new Adaptee());
    std::unique_ptr<CTarget> t2(new ObjectAdapter(ada.get()));
    t2->request(); 
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
    std::unique_ptr<Component>  decoratedComponentA(new ConcreteDecoratorA(component.get()));
    std::unique_ptr<Component>  decoratedComponentB(new ConcreteDecoratorB(decoratedComponentA.get()));

    decoratedComponentB->operation(); 

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
    std::unique_ptr<Implementor>  implA(new ConcreteImplementorA());
    std::unique_ptr<Abstraction>  absA(new RefinedAbstraction(implA.get()));
    absA->operation(); 
 
    std::unique_ptr<Implementor>  implB(new ConcreteImplementorA());
    std::unique_ptr<Abstraction>  absB(new RefinedAbstraction(implB.get()));
    absB->operation(); 
}

void call_composite() {

    std::unique_ptr<Leaf>  leaf1(new Leaf("Leaf1"));
    std::unique_ptr<Leaf>  leaf2(new Leaf("Leaf2"));

    std::unique_ptr<Composite>  composite(new Composite());
    composite->add(leaf1.get());
    composite->add(leaf2.get());

    std::unique_ptr<Composite>  composite2(new Composite());
    composite2->add(new Leaf("Leaf3"));
    composite->add(composite2.get());

    composite->operation();

}
void call_flyweight() {

    FlyWeightFactory fly;

    auto f1 = fly.GetFont("1");
    f1->show(); //  key: 1

    auto f2 = fly.GetFont("2");
    f2->show(); //  key: 2

    auto f3 = fly.GetFont("1");
    f3->show(); //  key: 1
}

void call_template() {
    std::unique_ptr<AbstractClass>  obj1(new ConcreteClass1());
    obj1->templateMethod();

    std::unique_ptr<AbstractClass>  obj2(new ConcreteClass2());
    obj2->templateMethod();
}

void call_command() {

    //create
    Light light;
    auto lightOn = std::make_unique<LightOnCommand>(light);

    // remote
    RemoteControl remote;
    remote.setCommand(std::move(lightOn));

    // execute
    remote.pressButton(); 
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
 
    std::unique_ptr<Subject> subject(new Subject());

    std::unique_ptr<Observer> observer1(new Observer(*subject.get()));
    std::unique_ptr<Observer> observer2(new Observer(*subject.get()));
    std::unique_ptr<Observer> observer3(new Observer(*subject.get()));

    subject->CreateMessage("Hello World! :D");
    observer3->RemoveMeFromTheList();

    subject->CreateMessage("The weather is hot today! :p");
    std::unique_ptr<Observer> observer4(new Observer(*subject.get()));

    observer2->RemoveMeFromTheList();
    std::unique_ptr<Observer> observer5(new Observer(*subject.get()));

    subject->CreateMessage("My new car is great! ;)");
    observer5->RemoveMeFromTheList();

    observer4->RemoveMeFromTheList();
    observer1->RemoveMeFromTheList();
}

void call_mediator(){
   
    std::unique_ptr<Component1> c1(new Component1());
    std::unique_ptr<Component2> c2(new Component2());
    std::unique_ptr<ConcreteMediator> mediator(new ConcreteMediator(c1.get(), c2.get()));
    
    c1->DoA();
    c2->DoD();
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
    std::unique_ptr<ContextA> context(new ContextA(new ConcreteStateA));

    context->Request1();
    context->Request2();
}

void call_visitor() {
    using namespace sv;

    std::array<const sv::Component *, 2> components = {new sv::ConcreteComponentA, new sv::ConcreteComponentB};

    std::unique_ptr<ConcreteVisitor1> visitor1(new ConcreteVisitor1());
    sv::ClientCode(components, visitor1.get());

    std::unique_ptr<ConcreteVisitor2> visitor2(new ConcreteVisitor2());
    ClientCode(components, visitor2.get());

    for (const sv::Component *comp : components) {
        delete comp;
    }
}

void call_responsibility() {
    //MonkeyHandler *monkey = new MonkeyHandler;
    std::unique_ptr<MonkeyHandler> monkey(new MonkeyHandler());
    std::unique_ptr<SquirrelHandler> squirrel(new SquirrelHandler());
    std::unique_ptr<DogHandler> dog(new DogHandler());
    //SquirrelHandler *squirrel = new SquirrelHandler;
    //DogHandler *dog = new DogHandler;

    monkey->SetNext(squirrel.get())->SetNext(dog.get());

    ClientCode(*monkey);
    ClientCode(*squirrel);
}

void call_examples() {
    // call_factory();
    // call_stratagy();
    // call_single();
    // call_adapter();
    // call_builder();
    // call_prototype();
    // call_decorator();
    // call_proxy();
    // call_facade();
    // call_bridge();
    // call_composite();
    // call_flyweight();
    // call_template();
    // call_command();
    // call_iterator();
    // call_observer();
    // call_mediator();
    // call_memento();
    // call_state();
    // call_visitor();
    call_responsibility();
}






